# OpenMV object detection script
# Place this on an OpenMV Cam (https://openmv.io) and connect the OpenMV UART to your microcontroller.
# It looks for blobs of color (simple) and will send simple CSV-like lines over serial:
# label:ball,cx:120,cy:80,w:40,h:40,conf:0.92

import sensor, image, time
from pyb import UART

# Adjust these thresholds to your objects / environment
# Example thresholds for a red object in LAB color space
red_threshold = (30, 80, 20, 60, 15, 60)  # (Lmin,Lmax, Amin,Amax, Bmin,Bmax)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA) # 320x240
sensor.skip_frames(time = 2000)

uart = UART(3, 115200, timeout_char=1000)
clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()
    blobs = img.find_blobs([red_threshold], pixels_threshold=200, area_threshold=200)
    if blobs:
        # pick largest blob
        blobs.sort(key=lambda b: b.pixels(), reverse=True)
        b = blobs[0]
        cx = b.cx()
        cy = b.cy()
        w = b.w()
        h = b.h()
        # confidence: normalized area
        conf = min(1.0, (b.pixels()/ (img.width()*img.height())) * 50)
        line = "label:ball,cx:%d,cy:%d,w:%d,h:%d,conf:%.2f\n" % (cx, cy, w, h, conf)
        uart.write(line)
    else:
        # send no-detection occasionally to indicate alive
        uart.write("label:none\n")
    # small delay
    time.sleep_ms(100)
