Behavior & thought-process documentation

This module implements a small behavior planner for the humanoid. It is intentionally simple but written
in a way that documents the "thought process" and allows easy extension.

Design goals:
- Keep decisions explainable: state machine with clear transitions and timing
- Use vision input to trigger behaviors (approach and grasp)
- Avoid grasping hazardous objects by checking a 'sharp' indicator from vision
- Be conservative on movement durations and allow fallback if the object is lost

States:
- WALK: default locomotion. The existing gait runs when Behavior::tick returns false.
- APPROACH: object detected; orient head and set a reach pose.
- ALIGN: refine heading and arm pose to center on object.
- GRASP: close the gripper and assume object acquired.
- HOLD: maintain grasp posture briefly.
- RECOVER: back off and return to WALK.

Extending this planner:
- Add distance estimation (depth camera) to step forward until within grasp range.
- Add a model-predictive controller or QP whole-body controller to compute safe transitions for walking and manipulation simultaneously.
- Add confidence tracking and object persistence (track detections across frames before committing to a grasp).
- Integrate collision checking with a kinematic model before commanding gripper/arm movement.

