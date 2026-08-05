"""
simulate.py - simple PyBullet simulator for the mini humanoid URDF

This script loads sim/robot.urdf and plays a simple gait while listening for keyboard input
or simulated detections. It's intended for quick visualization and testing of kinematics and behavior
without hardware.

Dependencies: pybullet, numpy

Run:
  python3 sim/simulate.py

"""

import time
import pybullet as p
import pybullet_data
import os
import numpy as np

URDF = os.path.join(os.path.dirname(__file__), 'robot.urdf')

# joint name to index mapping for convenience
JOINT_NAMES = [
    'l_hip_roll', 'l_hip_pitch', 'l_knee',
    'r_hip_roll', 'r_hip_pitch', 'r_knee',
    'l_shoulder', 'l_elbow', 'r_shoulder', 'r_elbow', 'neck_joint'
]

# Basic demo gait target positions in radians for the joints above (excluding gripper)
GAIT_SEQUENCE = [
    [0.1, 0.35, -0.6, -0.1, 0.35, -0.6, -0.3, 0.3, 0.3, 0.3, 0.0],
    [0.1, 0.1, -0.3, -0.1, 0.6, -1.0, -0.3, 0.3, 0.3, 0.3, 0.0],
    [0.0, 0.35, -0.6, 0.0, 0.35, -0.6, -0.3, 0.3, 0.3, 0.3, 0.0],
    [-0.1, 0.6, -1.0, 0.1, 0.1, -0.3, -0.3, 0.3, 0.3, 0.3, 0.0]
]

def main():
    physicsClient = p.connect(p.GUI)
    p.setAdditionalSearchPath(pybullet_data.getDataPath())
    p.setGravity(0,0,-9.81)
    planeId = p.loadURDF("plane.urdf")

    robot = p.loadURDF(URDF, [0,0,0.5], useFixedBase=False)

    # build name->index map
    jmap = {}
    for i in range(p.getNumJoints(robot)):
        info = p.getJointInfo(robot, i)
        name = info[1].decode('utf-8')
        jmap[name] = i

    # collect controllable joint indices
    controllable = []
    for name in JOINT_NAMES:
        if name in jmap:
            controllable.append(jmap[name])
        else:
            controllable.append(None)

    t = 0

    seq_i = 0
    try:
        while True:
            targets = GAIT_SEQUENCE[seq_i]
            # simple interpolation
            steps = 100
            for s in range(steps):
                alpha = s / float(steps)
                for ji, idx in enumerate(controllable):
                    if idx is None: continue
                    cur = p.getJointState(robot, idx)[0]
                    tgt = targets[ji]
                    cmd = cur + (tgt - cur) * 0.1
                    p.setJointMotorControl2(robot, idx, p.POSITION_CONTROL, targetPosition=cmd, force=50)
                p.stepSimulation()
                time.sleep(1.0/240.0)
            seq_i = (seq_i + 1) % len(GAIT_SEQUENCE)
            t += 1
    except KeyboardInterrupt:
        p.disconnect()

if __name__ == '__main__':
    main()
