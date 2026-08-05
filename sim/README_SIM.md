Simulation and ROS notes

I created a simple URDF (sim/robot.urdf) and a PyBullet simulation script (sim/simulate.py) so you can
visualize and test kinematics and gait behavior without hardware.

How to run:
1) Install pybullet (pip install pybullet)
2) python3 sim/simulate.py

What this simulation gives you:
- A simplified 3D model of the robot with joints matching the Arduino joint naming convention.
- A basic gait sequence to visualize motion and validate ranges and timings.

Next steps to improve simulation:
- Convert URDF to xacro and add inertial properties per link.
- Add joint transmission tags and effort/velocity controllers for use in Gazebo/ROS2.
- Implement behavior FSM inside the simulator to test approach/grasp logic with simulated vision.
- Add a depth camera in the simulation and a small vision node to test perception pipeline.

If you want ROS2 integration next, I can generate an ament package with robot_state_publisher and controller configuration.
