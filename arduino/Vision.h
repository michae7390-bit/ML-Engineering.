# Vision parser header (updated with 'sharp')

#ifndef VISION_H
#define VISION_H

#include <Arduino.h>

struct DetectedObject {
  String label;
  int cx;
  int cy;
  int w;
  int h;
  float conf;
  bool sharp; // new: whether object appears sharp / hazardous
  bool valid;
};

namespace Vision {
  void begin(HardwareSerial &ser);
  void update();
  DetectedObject get();
}

#endif
