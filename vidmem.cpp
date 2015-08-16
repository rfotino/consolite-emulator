#include <string.h>
#include "vidmem.h"

EmuVideoMemory::EmuVideoMemory() {
  // Zero out the video memory to start
  memset(_mem, 0, sizeof(_mem));
}

uint8_t EmuVideoMemory::get(const uint8_t& x,
                            const uint8_t& y) const {
  if (y < VIDEO_HEIGHT) {
    return _mem[x][y];
  } else {
    return 0;
  }
}

uint8_t EmuVideoMemory::getRed(const uint8_t& x,
                               const uint8_t& y) const {
  if (y < VIDEO_HEIGHT) {
    // The upper three bits are red
    return _mem[x][y] & 0xe0;
  } else {
    return 0;
  }
}

uint8_t EmuVideoMemory::getGreen(const uint8_t& x,
                                 const uint8_t& y) const {
  if (y < VIDEO_HEIGHT) {
    // The middle three bits are green
    return (_mem[x][y] & 0x1c) << 3;
  } else {
    return 0;
  }
}

uint8_t EmuVideoMemory::getBlue(const uint8_t& x,
                                const uint8_t& y) const {
  if (y < VIDEO_HEIGHT) {
    // The bottom two bits are blue
    return (_mem[x][y] & 0x3) << 6;
  } else {
    return 0;
  }
}

void EmuVideoMemory::set(const uint8_t& x,
                         const uint8_t& y,
                         const uint8_t& color) {
  if (y < VIDEO_HEIGHT) {
    _mem[x][y] = color;
  }
}
