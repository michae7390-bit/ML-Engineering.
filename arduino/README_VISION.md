Vision and companion camera integration

This folder contains code and instructions to add simple object detection to the mini-humanoid project using an OpenMV cam (or other UART-capable vision module).

Files added:
- arduino/Vision.h, Vision.cpp : Arduino-side parser & API for simple comma/colon messages coming from the vision module.
- arduino/gripper.h, gripper.cpp  : Simple gripper control using the PCA9685 PWM driver already in your project.
- openmv/object_detection.py : Example OpenMV script which detects red blobs and writes detection lines over UART.

How it works (high level):
1) Run the openmv/object_detection.py on an OpenMV Cam. Connect the OpenMV UART TX/RX to your microcontroller UART (e.g. Serial1 pins on a Mega or Serial on boards with single USB serial). Use common ground.
2) The OpenMV script will emit lines like: label:ball,cx:120,cy:80,w:40,h:40,conf:0.92\n
3) On the Arduino side Vision.update() should be called periodically (e.g. in loop()). Vision.get() returns the latest detection.
4) You can use the detected object's centroid (cx,cy) to estimate direction and then point the neck/shoulder servos and try to grasp by calling Gripper::close().

Notes & next steps:
- The OpenMV script uses a very simple red color threshold. Tune thresholds for your objects/environment.
- If you prefer the Pixy2 camera or an ESP32-CAM, implement a small adapter that emits the same key:value line format over serial; the Arduino-side parser will remain the same.
- For full object classification you can use the OpenMV's built-in Haar/Cascade or neural network support — change the OpenMV script to label objects accordingly (e.g. label:cup or label:ball).
