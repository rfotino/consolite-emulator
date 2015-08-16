#ifndef EMU_VIDMEM_H
#define EMU_VIDMEM_H

#include <stdint.h>
#include "defs.h"

class EmuVideoMemory {
 public:
  EmuVideoMemory();

  uint8_t get(const uint8_t& x, const uint8_t& y) const;
  uint8_t getRed(const uint8_t& x, const uint8_t& y) const;
  uint8_t getGreen(const uint8_t& x, const uint8_t& y) const;
  uint8_t getBlue(const uint8_t& x, const uint8_t& y) const;
  int getWidth() { return VIDEO_WIDTH; }
  int getHeight() { return VIDEO_HEIGHT; }

  void set(const uint8_t& x, const uint8_t& y, const uint8_t& color);

 private:
  uint8_t _mem[VIDEO_WIDTH][VIDEO_HEIGHT];
};

#endif
