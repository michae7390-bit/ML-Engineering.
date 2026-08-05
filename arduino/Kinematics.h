# Simple kinematics helpers for leg and arm (planar approximations)

#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <Arduino.h>
#include "RobotModel.h"

namespace Kinematics {
  // Leg (2D planar) inverse kinematics for a thigh-shin chain: returns hip_pitch and knee angles (degrees)
  // Inputs: desired foot position in sagittal plane relative to hip: x (forward), z (downwards)
  // lengths: thigh (L1) and shank (L2)
  // returns true on success
  bool legIK(float x, float z, float L1, float L2, float &hipPitchDeg, float &kneeDeg);

  // Simple arm planar IK (shoulder pitch + elbow) to reach forward (x: forward, z: up), lengths upper/forearm
  bool armIK(float x, float z, float L1, float L2, float &shoulderDeg, float &elbowDeg);
}

#endif
