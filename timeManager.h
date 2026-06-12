#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H
/*
  基本作用是，在误差超出容许值时，重置计时器的起始时间；
  当误差回到容许范围内时，不对起始时间做修改，并比较当前时间与起始时间的差值是否达到设定的稳定时间。
  如果达到设定的稳定时间，则认为状态稳定，isReady返回true。
*/
class timeManager {
  public:
    timeManager()
        : startMs(0), timeoutMs(0), running(false) {}
    // 初始化计时器，设置超时时间，并开始计时
    void init(unsigned long timeout) {
      timeoutMs = timeout;
      startMs = millis();
      running = true;
    }
    // 计时器重置并停止
    void resetAndStop() {
      running = false;
      timeoutMs = 0;
    }
    // 在误差超出容许值时调用，重置计时器起始时间
    void reset() {startMs = millis();}
    // 检查计时器是否达到设定的稳定时间
    bool isReady() const {
      unsigned long nowMs = millis();
      return running && (nowMs - startMs) >= timeoutMs;
    }
    bool isRunning() const {return running;}
  private:
    unsigned long startMs;
    unsigned long timeoutMs;
    bool running;
};

#endif
