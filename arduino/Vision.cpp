#include "Vision.h"

static HardwareSerial *visionSerial = nullptr;
static DetectedObject latest = {"",0,0,0,0,0.0,false,false};
static String lineBuf = "";

void Vision::begin(HardwareSerial &ser) {
  visionSerial = &ser;
}

// Simple parser for lines in the form:
// label:ball,cx:120,cy:80,w:40,h:40,conf:0.85,sharp:0\n
void Vision::update() {
  if (!visionSerial) return;
  while (visionSerial->available()) {
    char c = (char)visionSerial->read();
    if (c == '\r') continue;
    if (c == '\n') {
      if (lineBuf.length() > 0) {
        // parse
        DetectedObject obj;
        obj.valid = false;
        obj.sharp = false;
        int start = 0;
        while (start < lineBuf.length()) {
          int comma = lineBuf.indexOf(',', start);
          if (comma == -1) comma = lineBuf.length();
          String token = lineBuf.substring(start, comma);
          int colon = token.indexOf(':');
          if (colon > 0) {
            String key = token.substring(0, colon);
            String val = token.substring(colon+1);
            key.trim(); val.trim();
            if (key == "label") obj.label = val;
            else if (key == "cx") obj.cx = val.toInt();
            else if (key == "cy") obj.cy = val.toInt();
            else if (key == "w") obj.w = val.toInt();
            else if (key == "h") obj.h = val.toInt();
            else if (key == "conf") obj.conf = val.toFloat();
            else if (key == "sharp") obj.sharp = (val.toInt() != 0);
          }
          start = comma + 1;
        }
        // basic validation
        if (obj.label.length() > 0) {
          obj.valid = true;
          latest = obj;
        }
      }
      lineBuf = "";
    } else {
      lineBuf += c;
      // guard: drop very long lines
      if (lineBuf.length() > 200) lineBuf = "";
    }
  }
}

DetectedObject Vision::get() {
  return latest;
}
