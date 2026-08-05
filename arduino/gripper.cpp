#include "gripper.h"

static Adafruit_PWMServoDriver *pwmdrv = nullptr;
static uint8_t gripChannel = 11;
static bool closed = false;
static const int GRIP_OPEN = 220; // pulse (approx)
static const int GRIP_CLOSED = 420;

void Gripper::begin(Adafruit_PWMServoDriver *drv, uint8_t channel) {
  pwmdrv = drv;
  gripChannel = channel;
  // set to open by default
  if (pwmdrv) pwmdrv->setPWM(gripChannel, 0, GRIP_OPEN);
  closed = false;
}

void Gripper::open() {
  if (!pwmdrv) return;
  pwmdrv->setPWM(gripChannel, 0, GRIP_OPEN);
  closed = false;
}

void Gripper::close() {
  if (!pwmdrv) return;
  pwmdrv->setPWM(gripChannel, 0, GRIP_CLOSED);
  closed = true;
}

bool Gripper::isClosed() {
  return closed;
}
