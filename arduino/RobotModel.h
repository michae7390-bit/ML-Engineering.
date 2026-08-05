# Robot model definitions and joint limits

#ifndef ROBOTMODEL_H
#define ROBOTMODEL_H

#include <Arduino.h>

// Keep joint indices consistent with main.ino
enum JointIndex {
  LH_ROLL = 0,
  LH_PITCH = 1,
  LH_KNEE = 2,
  RH_ROLL = 3,
  RH_PITCH = 4,
  RH_KNEE = 5,
  LA_SHOULDER = 6,
  LA_ELBOW = 7,
  RA_SHOULDER = 8,
  RA_ELBOW = 9,
  NECK = 10,
  GRIPPER = 11,
  JOINT_COUNT = 12
};

// Simple clamp helper
static inline float clampf(float v, float lo, float hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// Joint limits (degrees) - tune to your servos/mechanics
static const float JOINT_MIN[JOINT_COUNT] = {
  -45, // LH_ROLL
  -30, // LH_PITCH
  -120, // LH_KNEE
  -45, // RH_ROLL
  -30, // RH_PITCH
  -120, // RH_KNEE
  -90, // LA_SHOULDER
  -10, // LA_ELBOW
  -90, // RA_SHOULDER
  -10, // RA_ELBOW
  -90, // NECK
  0    // GRIPPER (open pos as min)
};

static const float JOINT_MAX[JOINT_COUNT] = {
  45, // LH_ROLL
  60, // LH_PITCH
  0, // LH_KNEE
  45, // RH_ROLL
  60, // RH_PITCH
  0, // RH_KNEE
  90, // LA_SHOULDER
  120, // LA_ELBOW
  90, // RA_SHOULDER
  120, // RA_ELBOW
  90, // NECK
  180 // GRIPPER (closed pos as max)
};

#endif
