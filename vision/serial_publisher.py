"""
vision/serial_publisher.py

Runs a YOLO model (Ultralytics) on a USB camera (or file) and publishes compact key:value lines over a serial port
matching the Arduino parser in arduino/Vision.cpp (label,cx,cy,w,h,conf,sharp).

Example output lines:
label:cup,cx:120,cy:80,w:40,h:40,conf:0.92,sharp:0\n
If serial is not provided, the detection lines are printed to stdout for testing.

Usage:
python3 serial_publisher.py --model yolov8n.pt --camera 0 --serial /dev/ttyUSB0 --baud 115200

"""

import argparse
import time
import sys
import cv2
import numpy as np
from ultralytics import YOLO
import serial


def is_sharp(roi_gray):
    """Heuristic sharpness detection on a grayscale ROI.
    Returns 1 if the ROI likely contains a sharp object (blade-like edges), else 0.
    Method: Canny edge density + Hough line presence + aspect ratio heuristic.
    """
    if roi_gray is None or roi_gray.size == 0:
        return 0
    # resize small to speed up
    h, w = roi_gray.shape[:2]
    if h < 8 or w < 8:
        return 0
    small = cv2.resize(roi_gray, (max(32, w//2), max(32, h//2)))
    # edge detection
    edges = cv2.Canny(small, 50, 150)
    edge_density = np.count_nonzero(edges) / float(edges.size)

    # Hough lines for strong linear edges
    lines = cv2.HoughLinesP(edges, rho=1, theta=np.pi/180.0, threshold=20, minLineLength=min(small.shape)//4, maxLineGap=10)
    line_count = 0 if lines is None else len(lines)

    # aspect ratio
    ar = max(w, h) / float(min(w, h) + 1e-6)

    # heuristics: if edge density and a few long lines or very elongated shape -> sharp
    if edge_density > 0.06 and (line_count >= 2 or ar > 3.0):
        return 1
    if edge_density > 0.12:
        return 1
    return 0


def build_line(label, bbox, conf, sharp):
    # bbox in xyxy (x1,y1,x2,y2)
    x1, y1, x2, y2 = bbox
    w = int(x2 - x1)
    h = int(y2 - y1)
    cx = int(x1 + w/2)
    cy = int(y1 + h/2)
    line = f"label:{label},cx:{cx},cy:{cy},w:{w},h:{h},conf:{conf:.2f},sharp:{int(sharp)}\n"
    return line


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', type=str, default='yolov8n.pt', help='Ultralytics model path or name (yolov8n.pt)')
    parser.add_argument('--camera', type=int, default=0, help='camera index or video file path')
    parser.add_argument('--serial', type=str, default=None, help='serial device to write detection lines to (e.g. /dev/ttyUSB0)')
    parser.add_argument('--baud', type=int, default=115200)
    parser.add_argument('--conf', type=float, default=0.4, help='min confidence to report')
    parser.add_argument('--max-results', type=int, default=5, help='max detections to report per frame')
    args = parser.parse_args()

    print("Loading model...", file=sys.stderr)
    model = YOLO(args.model)

    print("Opening camera...", file=sys.stderr)
    cap = cv2.VideoCapture(args.camera)
    if not cap.isOpened():
        print(f"ERROR: cannot open camera {args.camera}", file=sys.stderr)
        return

    ser = None
    if args.serial:
        try:
            ser = serial.Serial(args.serial, args.baud, timeout=0.2)
            time.sleep(0.1)
            print(f"Serial open: {args.serial} @ {args.baud}", file=sys.stderr)
        except Exception as e:
            print(f"WARNING: could not open serial {args.serial}: {e}", file=sys.stderr)
            ser = None

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                print("No frame, exiting...", file=sys.stderr)
                break

            # run model (Ultralytics YOLO returns results list)
            results = model.predict(frame, imgsz=640, conf=args.conf, max_det=args.max_results, verbose=False)
            # results is a list with one element per image
            if len(results) == 0:
                # nothing
                if ser:
                    ser.write(b"label:none\n")
                else:
                    print("label:none")
                time.sleep(0.05)
                continue

            res = results[0]
            # boxes: xyxy, conf, cls
            if res.boxes is None or len(res.boxes) == 0:
                if ser:
                    ser.write(b"label:none\n")
                else:
                    print("label:none")
                time.sleep(0.05)
                continue

            # process detections (sorted by confidence)
            dets = []
            for box in res.boxes:
                x1, y1, x2, y2 = box.xyxy[0].tolist()
                conf = float(box.conf[0])
                cls = int(box.cls[0])
                label = res.names.get(cls, str(cls))
                dets.append((conf, label, (int(x1), int(y1), int(x2), int(y2))))
            dets.sort(reverse=True)

            # report top detection(s)
            for i, (conf, label, bbox) in enumerate(dets[:args.max_results]):
                x1, y1, x2, y2 = bbox
                # extract ROI for sharpness detection
                h, w = frame.shape[:2]
                x1c = max(0, x1)
                y1c = max(0, y1)
                x2c = min(w-1, x2)
                y2c = min(h-1, y2)
                if x2c <= x1c or y2c <= y1c:
                    sharp = 0
                else:
                    roi = frame[y1c:y2c, x1c:x2c]
                    roi_gray = cv2.cvtColor(roi, cv2.COLOR_BGR2GRAY)
                    sharp = is_sharp(roi_gray)

                line = build_line(label, bbox, conf, sharp)
                # send line
                if ser:
                    try:
                        ser.write(line.encode('utf-8'))
                    except Exception as e:
                        print(f"Serial write error: {e}", file=sys.stderr)
                else:
                    print(line.strip())

            # small sleep to avoid pegging CPU
            time.sleep(0.02)
    except KeyboardInterrupt:
        print("Interrupted by user", file=sys.stderr)
    finally:
        cap.release()
        if ser:
            ser.close()


if __name__ == '__main__':
    main()
