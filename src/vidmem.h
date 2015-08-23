#ifndef EMU_VIDMEM_H
#define EMU_VIDMEM_H

#include <cairo/cairo.h>
#include <stdint.h>
#include "defs.h"

class EmuVideoMemory {
 public:
  EmuVideoMemory();
  ~EmuVideoMemory();

  void initImage(cairo_surface_t *window_surface);
  int getWidth() { return VIDEO_WIDTH; }
  int getHeight() { return VIDEO_HEIGHT; }
  cairo_surface_t *getBuffer() { return _buffer; }

  void set(const uint8_t& x, const uint8_t& y, const uint8_t& color);

 private:
  cairo_surface_t *_buffer;
  cairo_t *_cairo;
};

#endif
