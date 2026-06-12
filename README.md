# float v2.0

v2.0保留状态机的逻辑，同时根据控制逻辑和硬件配置的改变进行了冗余部分的删除和修改。

## floatManager.h/.cpp

状态机，核心接口是`run()`，每次主循环都会调用一次。

`run()`会先更新一遍mqtt的状态，然后检查有没有新的命令msg

## stepperDriver.h/.cpp

除构造函数，只提供`void posControl(uint8_t dir, uint32_t clk)`函数实现对步进电机的位置控制，控制排水量