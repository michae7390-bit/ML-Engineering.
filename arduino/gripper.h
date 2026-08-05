# Gripper control using a PCA9685 channel (shared with other servos through PWM driver)
#ifndef GRIPPER_H
#define GRIPPER_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

// Provide the pwm driver instance from main. The user must call Gripper::begin(&pwm, channel)
namespace Gripper {
  void begin(Adafruit_PWMServoDriver *drv, uint8_t channel);
  void open();
  void close();
  bool isClosed();
}

#endif
