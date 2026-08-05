# Vision interface for Arduino to parse simple detection messages from an OpenMV / companion camera

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
  bool valid;
};

namespace Vision {
  void begin(HardwareSerial &ser);
  void update();
  DetectedObject get();
}

#endif
