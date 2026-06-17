#ifndef STEPPER_DRIVER_H
#define STEPPER_DRIVER_H
#include "config.h"

class stepperDriver {
  public:
    stepperDriver() {
        Serial.begin(115200);
    }
    void posControl(uint8_t dir, uint32_t clk) {
      // 待完善，电机进行相对运动来调整排水量
      uint16_t vel = VELOCITY;
      uint8_t cmd[16] = {0};
      // 装载命令
      cmd[0]  =  1;                      // 地址
      cmd[1]  =  0xFD;                      // 功能码
      cmd[2]  =  dir;                       // 方向
      cmd[3]  =  (uint8_t)(vel >> 8);       // 速度(RPM)高8位字节
      cmd[4]  =  (uint8_t)(vel >> 0);       // 速度(RPM)低8位字节 
      cmd[5]  =  0;                       // 加速度，注意：0是直接启动
      cmd[6]  =  (uint8_t)(clk >> 24);      // 脉冲数(bit24 - bit31)
      cmd[7]  =  (uint8_t)(clk >> 16);      // 脉冲数(bit16 - bit23)
      cmd[8]  =  (uint8_t)(clk >> 8);       // 脉冲数(bit8  - bit15)
      cmd[9]  =  (uint8_t)(clk >> 0);       // 脉冲数(bit0  - bit7 )
      cmd[10] =  0;                      // 相位/绝对标志，false为相对运动，true为绝对值运动
      cmd[11] =  0;                      // 多机同步运动标志，false为不启用，true为启用
      cmd[12] =  0x6B;                      // 校验字节
      // 发送命令
      Serial.write(cmd, 13);
    };
    void reset() {
      // 待完善，电机进行绝对运动使针筒吸水量为零以复位
      uint8_t dir = 0;
      uint16_t vel = VELOCITY;
      uint32_t clk = RESET_POSITION;
      uint8_t cmd[16] = {0};
      // 装载命令
      cmd[0]  =  1;                      // 地址
      cmd[1]  =  0xFD;                      // 功能码
      cmd[2]  =  dir;                       // 方向
      cmd[3]  =  (uint8_t)(vel >> 8);       // 速度(RPM)高8位字节
      cmd[4]  =  (uint8_t)(vel >> 0);       // 速度(RPM)低8位字节 
      cmd[5]  =  0;                       // 加速度，注意：0是直接启动
      cmd[6]  =  (uint8_t)(clk >> 24);      // 脉冲数(bit24 - bit31)
      cmd[7]  =  (uint8_t)(clk >> 16);      // 脉冲数(bit16 - bit23)
      cmd[8]  =  (uint8_t)(clk >> 8);       // 脉冲数(bit8  - bit15)
      cmd[9]  =  (uint8_t)(clk >> 0);       // 脉冲数(bit0  - bit7 )
      cmd[10] =  1;                      // 相位/绝对标志，false为相对运动，true为绝对值运动
      cmd[11] =  0;                      // 多机同步运动标志，false为不启用，true为启用
      cmd[12] =  0x6B;                      // 校验字节
      // 发送命令
      Serial.write(cmd, 13);
    };
};

#endif