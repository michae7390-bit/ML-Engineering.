# Behavior planner and "thought process" state machine

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <Arduino.h>
#include "RobotModel.h"
#include "Vision.h"

// Behavior module: produces target pose arrays (length JOINT_COUNT) based on sensor inputs (vision, time)
// It implements a small finite-state machine and exposes a single tick() function that the main loop
// can call each cycle.

namespace Behavior {

  // Initialize behavior module (call in setup)
  void begin();

  // tick: given the current 'defaultPose' (e.g., homePose), and the latest DetectedObject from vision,
  // produce a targetPose (array of JOINT_COUNT floats). If the behavior requests a gripper action,
  // gripperAction will be set: 0=no action, 1=open, 2=close.
  // Returns true if a behavior has provided a targetPose to apply; false means caller should continue regular gait.
  bool tick(const float *defaultPose, const DetectedObject &det, float *targetPose, uint8_t *gripperAction);

}

#endif
