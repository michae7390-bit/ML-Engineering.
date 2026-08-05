#include "Behavior.h"
#include "Kinematics.h"

// Simple state machine states
enum BehaviorState {
  STATE_IDLE = 0,
  STATE_WALK,
  STATE_APPROACH,
  STATE_ALIGN,
  STATE_GRASP,
  STATE_HOLD,
  STATE_RECOVER
};

static BehaviorState state = STATE_IDLE;
static unsigned long stateEnteredAt = 0;
static bool initialized = false;

void Behavior::begin() {
  state = STATE_WALK;
  stateEnteredAt = millis();
  initialized = true;
}

static void enterState(BehaviorState s) {
  state = s;
  stateEnteredAt = millis();
}

// Helper to copy default pose into targetPose
static void copyDefault(const float *defaultPose, float *targetPose) {
  for (int i=0;i<JOINT_COUNT;++i) targetPose[i] = defaultPose[i];
}

bool Behavior::tick(const float *defaultPose, const DetectedObject &det, float *targetPose, uint8_t *gripperAction) {
  // default: no behavior provided
  if (!initialized) begin();
  *gripperAction = 0;

  // Simple decision-making "thought process" description:
  // - Continuously monitor vision for objects.
  // - If a valid non-sharp target (a 'ball') appears with sufficient confidence, interrupt walking and enter APPROACH.
  // - In APPROACH, orient head (neck) to center on object, raise arm and compute arm IK to place hand above object.
  // - Then ALIGN to fine-tune, then GRASP by commanding gripper close. After grasp, enter HOLD and then RECOVER to walk away.
  // The state machine below implements that logic with timing and fallbacks.

  // Start with copying default
  copyDefault(defaultPose, targetPose);

  unsigned long now = millis();

  // Evaluate sensory condition
  bool seeBall = false;
  if (det.valid && det.label.length() > 0 && det.label != "none" && det.conf > 0.35 && !det.sharp) {
    if (det.label == "ball" || det.label == "cup") seeBall = true; // simple whitelist
  }

  switch (state) {
    case STATE_WALK:
      // when walking, if we see a ball -> APPROACH
      if (seeBall) {
        enterState(STATE_APPROACH);
        // fallthrough to compute first approach pose
      } else {
        // no special pose; let caller use gait
        return false;
      }
      // no break
    case STATE_APPROACH: {
      // compute neck angle from cx (image width assumed 320)
      int imgW = 320;
      float neck = 0;
      if (det.valid) {
        float norm = ((float)det.cx / imgW) * 2.0 - 1.0; // -1..1
        neck = norm * 45.0; // map to degrees
      }
      // plan an arm pose: we'll try to place hand forward of chest
      // use simple arm IK: target at x forward 25..40 cm, z up 10..20 cm
      float armX = 25.0; // cm in front of shoulder
      float armZ = 10.0; // cm up
      // approximate link lengths (cm)
      const float upper = 12.0;
      const float fore = 12.0;
      float shoulderDeg=0, elbowDeg=0;
      bool ok = Kinematics::armIK(armX, armZ, upper, fore, shoulderDeg, elbowDeg);
      if (!ok) {
        shoulderDeg = -20; elbowDeg = 60; // fallback moderate pose
      }
      copyDefault(defaultPose, targetPose);
      targetPose[NECK] = clampf(neck, JOINT_MIN[NECK], JOINT_MAX[NECK]);
      targetPose[LA_SHOULDER] = clampf(shoulderDeg, JOINT_MIN[LA_SHOULDER], JOINT_MAX[LA_SHOULDER]);
      targetPose[LA_ELBOW] = clampf(elbowDeg, JOINT_MIN[LA_ELBOW], JOINT_MAX[LA_ELBOW]);

      // advance to ALIGN after short pause
      if (now - stateEnteredAt > 600) {
        enterState(STATE_ALIGN);
      }
      return true;
    }
    case STATE_ALIGN: {
      // hold a refined pose and check that object is still present; if lost -> RECOVER
      if (!seeBall) {
        // lost object; return to walking
        enterState(STATE_RECOVER);
        return true;
      }
      // small tracking movement: nudge neck and shoulder toward object's cx
      int imgW = 320;
      float norm = ((float)det.cx / imgW) * 2.0 - 1.0;
      float neck = norm * 45.0;
      copyDefault(defaultPose, targetPose);
      targetPose[NECK] = clampf(neck, JOINT_MIN[NECK], JOINT_MAX[NECK]);
      targetPose[LA_SHOULDER] -= 10; // reach slightly
      // after a short time, go to GRASP
      if (now - stateEnteredAt > 800) {
        enterState(STATE_GRASP);
      }
      return true;
    }
    case STATE_GRASP: {
      // Issue gripper close action and move arm to grasp pose
      copyDefault(defaultPose, targetPose);
      targetPose[NECK] = 0;
      targetPose[LA_SHOULDER] = -5;
      targetPose[LA_ELBOW] = 45;
      // request gripper close
      *gripperAction = 2; // close
      enterState(STATE_HOLD);
      return true;
    }
    case STATE_HOLD: {
      // keep the pose for a while, then back off
      copyDefault(defaultPose, targetPose);
      targetPose[LA_SHOULDER] = -10;
      targetPose[LA_ELBOW] = 60;
      if (now - stateEnteredAt > 1200) {
        enterState(STATE_RECOVER);
      }
      return true;
    }
    case STATE_RECOVER: {
      // open gripper slightly as we back off
      copyDefault(defaultPose, targetPose);
      targetPose[LA_SHOULDER] = -20;
      targetPose[LA_ELBOW] = 30;
      *gripperAction = 1; // open
      // after short time, return to walk
      if (now - stateEnteredAt > 800) {
        enterState(STATE_WALK);
      }
      return true;
    }
    default:
      return false;
  }
}
