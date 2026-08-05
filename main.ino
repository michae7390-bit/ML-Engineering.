// Mini Humanoid - basic gait + IMU stabilization + vision-based object approach/grasp
// Requires: Adafruit_PWMServoDriver (for PCA9685), MPU6050 library, companion vision module (OpenMV) connected over UART
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <MPU6050.h>

#include "Vision.h"
#include "gripper.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // default addr 0x40
MPU6050 imu;

const int SERVOMIN = 150; // pulse length out of 4096
const int SERVOMAX = 600;

// Servo channel mapping (example)
// Legs: left hip roll, left hip pitch, left knee, right hip roll, right hip pitch, right knee
// Arms + neck: next channels...
enum {
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
  GRIPPER = 11
};

float degToPulse(float deg) {
  // convert -90..90 (or 0..180) deg to PWM pulse
  // assume 500us..2500us -> map to SERVOMIN..SERVOMAX
  float pulse = map((long)deg, -90, 90, SERVOMIN, SERVOMAX);
  return pulse;
}

void setServoDeg(uint8_t channel, float deg) {
  uint16_t pulse = (uint16_t)degToPulse(deg);
  pwm.setPWM(channel, 0, pulse);
}

// Basic home pose (angles in degrees)
float homePose[12] = {
  0,   // LH_ROLL
  20,  // LH_PITCH
  -40, // LH_KNEE
  0,   // RH_ROLL
  20,  // RH_PITCH
  -40, // RH_KNEE
  -20, // LA_SHOULDER
  10,  // LA_ELBOW
  20,  // RA_SHOULDER
  10,  // RA_ELBOW
  0,   // NECK
  20   // GRIPPER (open)
};

// Simple two-step gait keyframes (left support, right support)
struct Keyframe {
  float angles[12];
  int duration; // ms
};

Keyframe gait[] = {
  // Shift COM right (prepare left swing)
  { { 8, 18, -40, -8, 22, -40, -20,10,20,10,0,20 }, 400 },
  // Left swing forward
  { { 8, 8, -20, -8, 28, -60, -20,10,20,10,0,20 }, 600 },
  // Center (both feet)
  { { 0, 20, -40, 0, 20, -40, -20,10,20,10,0,20 }, 400 },
  // Shift COM left (prepare right swing)
  { { -8, 22, -60, 8, 8, -20, -20,10,20,10,0,20 }, 600 },
  // Right swing forward -> back to center
  { { 0, 20, -40, 0, 20, -40, -20,10,20,10,0,20 }, 400 }
};
const int gaitLen = sizeof(gait) / sizeof(gait[0]);

// IMU stabilization gains
float Kp_roll = 1.5; // degrees -> servo degrees
float Kp_pitch = 1.0;

// Vision / behavior

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50); // 50 Hz for servos

  // init IMU
  imu.initialize();
  if (!imu.testConnection()) {
    Serial.println("MPU6050 not connected!");
  } else {
    Serial.println("MPU6050 OK");
  }

  // Initialize vision - attempt to use Serial1 if available, fall back to Serial
#if defined(Serial1)
  Vision::begin(Serial1);
  Serial.println("Vision: using Serial1");
#else
  Vision::begin(Serial);
  Serial.println("Vision: using Serial (single-serial board)");
#endif

  // Gripper init
  Gripper::begin(&pwm, GRIPPER);

  // move to home pose
  for (int i = 0; i < 12; ++i) setServoDeg(i, homePose[i]);
  delay(500);
}

void applyPoseWithIMU(const float *pose) {
  // read IMU: get roll/pitch (approx)
  int16_t ax, ay, az, gx, gy, gz;
  imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  // convert accelerometer raw to angles (very rough)
  float accelX = ax / 16384.0;
  float accelY = ay / 16384.0;
  float accelZ = az / 16384.0;
  float roll = atan2(accelY, accelZ) * 180.0 / PI; // degrees
  float pitch = atan2(-accelX, sqrt(accelY*accelY + accelZ*accelZ)) * 180.0 / PI;

  // compute corrections (P only)
  float rollCorr = -Kp_roll * roll;   // apply opposite to stabilize
  float pitchCorr = -Kp_pitch * pitch;

  // apply to hip roll and hip pitch servos (distribute corrections)
  float corrected[12];
  for (int i = 0; i < 12; ++i) corrected[i] = pose[i];

  // apply roll correction to both hip roll servos in opposite directions
  corrected[LH_ROLL] += rollCorr * 0.5;
  corrected[RH_ROLL] -= rollCorr * 0.5;

  // apply pitch correction to both hip pitch
  corrected[LH_PITCH] += pitchCorr * 0.5;
  corrected[RH_PITCH] += pitchCorr * 0.5;

  // send to servos
  for (int i = 0; i < 12; ++i) setServoDeg(i, corrected[i]);
}

bool approachAndGraspIfBall() {
  DetectedObject obj = Vision::get();
  if (!obj.valid) return false;
  if (obj.label == "none") return false;
  if (obj.label != "ball") return false; // only handle ball for now
  if (obj.conf < 0.4) return false;

  Serial.print("Detected "); Serial.print(obj.label); Serial.print(" at cx="); Serial.println(obj.cx);

  // Simple strategy: point neck towards object's horizontal position.
  // For QVGA 320x240, cx 0..320 -> map to neck angle -45..45
  int imgW = 320;
  float norm = ((float)obj.cx / imgW) * 2.0 - 1.0; // -1..1
  float neckTarget = norm * 45.0; // degrees

  // lift arm a bit and point neck
  float pose[12];
  for (int i=0;i<12;++i) pose[i] = homePose[i];
  pose[NECK] = neckTarget;
  pose[LA_SHOULDER] = -10;
  pose[LA_ELBOW] = 30;

  // move to pose
  for (int s=0;s<25;++s) {
    float t = (float)s/25.0;
    float interp[12];
    for (int i=0;i<12;++i) interp[i] = homePose[i] + (pose[i]-homePose[i])*t;
    applyPoseWithIMU(interp);
    delay(30);
  }

  // close gripper
  Gripper::close();
  delay(500);

  // indicate success
  Serial.println("Grasp attempt done");
  return true;
}

void loop() {
  // update vision parser
  Vision::update();

  // check for object and handle grasping behavior
  static bool grabbed = false;
  if (!grabbed) {
    if (approachAndGraspIfBall()) {
      grabbed = true; // don't re-grasp immediately
    }
  }

  // simple loop over gait keyframes (only if not grabbed)
  if (!grabbed) {
    for (int k = 0; k < gaitLen; ++k) {
      int steps = max(1, gait[k].duration / 20);
      // interpolate from current to target
      for (int s = 0; s <= steps; ++s) {
        float t = (float)s / steps;
        float interp[12];
        // read current angles is coarse: use homePose as baseline for simplicity
        for (int i = 0; i < 12; ++i) {
          float start = homePose[i];
          float target = gait[k].angles[i];
          interp[i] = start + (target - start) * t;
        }
        applyPoseWithIMU(interp);
        delay(20);
        // allow vision updates during gait
        Vision::update();
        // if an object appears mid-walk, break out to handle it
        DetectedObject obj = Vision::get();
        if (obj.valid && obj.label == "ball" && obj.conf > 0.5) {
          Serial.println("Object detected during gait, interrupting walk");
          k = gaitLen; // break outer loop
          break;
        }
      }
      // after keyframe, update homePose to last target (so interpolation continues correctly)
      for (int i = 0; i < 12; ++i) homePose[i] = gait[k].angles[i];
    }
  }

  delay(10);
}
