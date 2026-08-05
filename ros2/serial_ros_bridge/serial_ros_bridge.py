"""
serial_ros_bridge/serial_ros_bridge.py

Simple ROS2 node that reads the Arduino-style key:value detection lines from a serial port and
publishes them as a std_msgs/String message on /vision/detection (payload is the raw line).
It also subscribes to /robot/command (std_msgs/String), forwarding the string contents to the serial port.

Run:
  ros2 run serial_ros_bridge serial_ros_bridge --serial /dev/ttyUSB0 --baud 115200

"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import argparse
import serial
import threading

class SerialROSBridge(Node):
    def __init__(self, serial_port=None, baud=115200):
        super().__init__('serial_ros_bridge')
        self.pub = self.create_publisher(String, 'vision/detection', 10)
        self.sub = self.create_subscription(String, 'robot/command', self.cmd_cb, 10)
        self.serial_port = serial_port
        self.baud = baud
        self.ser = None
        self._stop = False
        if serial_port:
            try:
                self.ser = serial.Serial(serial_port, baud, timeout=0.2)
                self.get_logger().info(f'Opened serial {serial_port} @ {baud}')
            except Exception as e:
                self.get_logger().warn(f'Failed to open serial {serial_port}: {e}')
                self.ser = None
        # start reader thread
        self.thread = threading.Thread(target=self.reader_thread, daemon=True)
        self.thread.start()

    def cmd_cb(self, msg: String):
        data = msg.data
        if self.ser:
            try:
                if not data.endswith('\n'):
                    data = data + '\n'
                self.ser.write(data.encode('utf-8'))
            except Exception as e:
                self.get_logger().warn(f'Error writing to serial: {e}')
        else:
            self.get_logger().info('Serial not open; would send: ' + data)

    def reader_thread(self):
        buf = ''
        while rclpy.ok() and not self._stop:
            if self.ser and self.ser.in_waiting:
                try:
                    c = self.ser.read(1).decode('utf-8', errors='ignore')
                    if c == '\r':
                        continue
                    if c == '\n':
                        if buf:
                            msg = String()
                            msg.data = buf
                            self.pub.publish(msg)
                            buf = ''
                    else:
                        buf += c
                        if len(buf) > 512:
                            buf = ''
                except Exception as e:
                    self.get_logger().warn(f'Serial read error: {e}')
            else:
                # sleep briefly
                import time
                time.sleep(0.01)

    def destroy_node(self):
        self._stop = True
        if self.thread.is_alive():
            self.thread.join(timeout=0.5)
        if self.ser:
            try:
                self.ser.close()
            except:
                pass
        super().destroy_node()


def main(args=None):
    rclpy.init(args=args)
    parser = argparse.ArgumentParser()
    parser.add_argument('--serial', type=str, default=None)
    parser.add_argument('--baud', type=int, default=115200)
    parsed, unknown = parser.parse_known_args()

    node = SerialROSBridge(serial_port=parsed.serial, baud=parsed.baud)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()
