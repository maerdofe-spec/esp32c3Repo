#include "taskExecuter.h"

#include <math.h>

void taskExecuter::init() {
  depthTimer.resetAndStop();
  hoverTimer.resetAndStop();
  hardResetter.resetAndStop();
}

void taskExecuter::idle() {
  // 在待机状态下，保持机械复位状态，并重置PID控制器
  stepper.posControl(0, 0);
  pid.resetIntegral();
  pid.setLastDepth(sensor.getDepth());
  // 避免esp长时间通电后上传数据的时间值过大
  startTime = millis();
}

// toDepth会检查深度误差是否在容许范围内，并不断控制步进电机位置，若达到稳定时间则返回true
bool taskExecuter::toDepth(timeManager &recorderTimer) {
  // currentDepth测试值
  float currentDepth = 100.0f+goalDepth+sensor.getDepth();
  float error = goalDepth - currentDepth;


  //测试段
  if (!hardResetter.isRunning()) {
    hardResetter.init(5000);
  }


  // 如果计时器还没有初始化，则初始化计时器
  if (!depthTimer.isRunning()) {
    depthTimer.init(TRANSITION_STABLE_DURATION);
  }
  //定期记录数据
  if(recorderTimer.isReady()) {
    recorder.record(currentDepth, millis() - startTime);
    recorderTimer.reset();
  }
  // 如果误差在容许范围内，检查计时器是否达到设定的稳定时间，如果达到则返回true
  if (fabsf(error) <= MAX_DEPTH_OFFSET) {
    if(depthTimer.isReady()) {
      depthTimer.resetAndStop();
      return true;
    }
  }
  // 否则，误差超出容许范围，重置计时器
  else depthTimer.reset();
  // 进行PID控制
  Control(currentDepth, error);


  //测试端，强制返回并重置计时器
  if(hardResetter.isReady()) {
    hardResetter.resetAndStop();
    depthTimer.resetAndStop();
    return true;
  }


  // 返回false表示尚未达到目标深度
  return false;
}


// 逻辑类同toDepth，但使用hoverTimer和HOVER_DURATION，且不会重置计时器，仅检查是否达到稳定时间
bool taskExecuter::hover(timeManager &recorderTimer) {
  // currentDepth测试值
  float currentDepth = goalDepth+sensor.getDepth();
  float error = goalDepth - currentDepth;


  //测试段
  if (!hardResetter.isRunning()) {
    hardResetter.init(5000);
  }


  // 如果计时器还没有初始化，则初始化计时器
  if (!hoverTimer.isRunning()) {
    hoverTimer.init(HOVER_DURATION);
  }
  //定期记录数据
  if(recorderTimer.isReady()) {
    recorder.record(currentDepth, millis() - startTime);
    recorderTimer.reset();
  }

  // 如果误差超过比赛要求的最大任务深度偏移，重置计时器
  if (fabsf(error) > MAX_DEPTH_OFFSET) {
    hoverTimer.reset();
  }

  // 检查计时器是否达到设定的稳定时间，如果达到则返回true
  if (hoverTimer.isReady()) {
    hoverTimer.resetAndStop();
    return true;
    }  // 否则，还没有悬停足够时间，继续进行PID控制
  Control(currentDepth, error);


  //测试端，强制返回并重置计时器
  if(hardResetter.isReady()) {
    hardResetter.resetAndStop();
    hoverTimer.resetAndStop();
    return true;
  }


  // 返回false表示尚未悬停足够时间
  return false;
}

bool taskExecuter::recovery(timeManager &recorderTimer) {
  // 待完善回收逻辑，目前先简单地上升到0.1米并认为回收完成
  setGoalDepth(0.1f);
  return(toDepth(recorderTimer));
}

void taskExecuter::Control(float currentDepth, float error) {
  int32_t output = pid.pidControl(currentDepth, error);
  /*
  脉冲数为0时理论上处于最大排水的机械复位状态
  但有可能起始状态非机械复位状态，此时脉冲数可能会是负数，则需要绝对位置反向控制
  */
  uint8_t dir = (output >= 0) ? 0 : 1;
  uint32_t clk = (uint32_t)abs(output);
  stepper.posControl(dir, clk);
}
