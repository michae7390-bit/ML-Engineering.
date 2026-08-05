# Vision integration README

This folder contains a Jetson/PC Python vision publisher that runs a YOLO model and emits compact
key:value detection lines over serial for the Arduino-side parser (arduino/Vision.cpp).

Files:
- serial_publisher.py : main script. Uses ultralytics YOLO API and OpenCV to capture frames, run detections,
  compute a simple "sharp" heuristic, and send lines over serial like:
    label:cup,cx:120,cy:80,w:40,h:40,conf:0.92,sharp:0\n
- requirements.txt : pip requirements (torch installation may be platform-dependent).

How to use:
1) Install dependencies into a Python environment. On Jetson use the Jetson-specific torch wheel.
   pip install -r vision/requirements.txt
   (On Jetson, install torch + torchvision per Jetson docs before installing ultralytics.)

2) Run the script with your model and serial device. Example:
   python3 vision/serial_publisher.py --model yolov8n.pt --camera 0 --serial /dev/ttyUSB0 --baud 115200

3) If you don't have a serial device during testing, omit --serial and detection lines will be printed to stdout.

Tuning:
- The "sharp" field is a heuristic combining edge density and line detection. It's useful as a quick filter
  to avoid grasping hazardous or blade-like objects. It's not guaranteed — for production use train a
  classifier on labeled data.

- Adjust --conf for minimum detection threshold and --max-results for number of boxes per frame.

Notes:
- Ultraytics YOLO models (yolov8n, yolov8s, etc.) work well for common objects; fine-tune on your dataset for
  improved results.
- For real-time performance on Jetson, use a smaller model (yolov8n) and enable TensorRT/FP16 where available.
