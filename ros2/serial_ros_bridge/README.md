Simple ROS2 bridge README

This ROS2 package publishes serial vision detection lines and forwards commands to the microcontroller.

Usage:
- Build/install into your ROS2 Python environment (or run with ros2 run if installed):
  cd ros2/serial_ros_bridge
  pip install -e .

- Run the node:
  ros2 run serial_ros_bridge serial_ros_bridge --serial /dev/ttyUSB0 --baud 115200

- Subscribes to: /robot/command (std_msgs/String) — send simple key:value lines to MCU
- Publishes:  /vision/detection (std_msgs/String) — raw detection line as received from serial

This bridge is intentionally minimal and avoids custom ROS messages so it remains simple to use.
