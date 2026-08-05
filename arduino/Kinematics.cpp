#include "Kinematics.h"
#include <math.h>

namespace Kinematics {

bool legIK(float x, float z, float L1, float L2, float &hipPitchDeg, float &kneeDeg) {
  // Solve planar 2-link inverse kinematics for (x,z)
  // Distance from hip to foot
  float D = sqrt(x*x + z*z);
  if (D < 1e-6) return false;

  // check reach
  if (D > (L1 + L2) || D < fabs(L1 - L2)) return false;

  // law of cosines
  float cos_knee = (L1*L1 + L2*L2 - D*D) / (2*L1*L2);
  cos_knee = clampf(cos_knee, -1.0, 1.0);
  float knee_rad = acos(cos_knee);

  // angle from thigh to target
  float cos_alpha = (L1*L1 + D*D - L2*L2) / (2*L1*D);
  cos_alpha = clampf(cos_alpha, -1.0, 1.0);
  float alpha = acos(cos_alpha);

  float theta_to_target = atan2(-z, x); // x forward, z downward -> negative z is up

  float hip_rad = theta_to_target - alpha;

  // convert to degrees
  hipPitchDeg = hip_rad * 180.0 / M_PI;
  kneeDeg = -knee_rad * 180.0 / M_PI; // knee convention: negative when folded

  return true;
}

bool armIK(float x, float z, float L1, float L2, float &shoulderDeg, float &elbowDeg) {
  // arm IK similar to legIK but elbow bends positive
  float D = sqrt(x*x + z*z);
  if (D < 1e-6) return false;
  if (D > (L1 + L2) || D < fabs(L1 - L2)) return false;

  float cos_elbow = (L1*L1 + L2*L2 - D*D) / (2*L1*L2);
  cos_elbow = clampf(cos_elbow, -1.0, 1.0);
  float elbow_rad = M_PI - acos(cos_elbow); // inner angle at elbow

  float cos_alpha = (L1*L1 + D*D - L2*L2) / (2*L1*D);
  cos_alpha = clampf(cos_alpha, -1.0, 1.0);
  float alpha = acos(cos_alpha);

  float theta_to_target = atan2(z, x); // z up
  float shoulder_rad = theta_to_target - alpha;

  shoulderDeg = shoulder_rad * 180.0 / M_PI;
  elbowDeg = elbow_rad * 180.0 / M_PI;
  return true;
}

