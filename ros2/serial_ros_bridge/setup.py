from setuptools import setup
import os
from glob import glob

package_name = 'serial_ros_bridge'

setup(
    name=package_name,
    version='0.1.0',
    packages=[package_name],
    data_files=[
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['pyserial'],
    zip_safe=True,
    author='michae7390',
    description='ROS2 bridge for serial vision and MCU commands',
    entry_points={
        'console_scripts': [
            'serial_ros_bridge = serial_ros_bridge.serial_ros_bridge:main'
        ],
    },
)
