#include <string.h>
#include "vidmem.h"

EmuVideoMemory::EmuVideoMemory() {
  _buffer = cairo_image_surface_create(CAIRO_FORMAT_RGB24,
                                       getWidth(),
                                       getHeight());
  _cairo = cairo_create(_buffer);
}

EmuVideoMemory::~EmuVideoMemory() {
  cairo_destroy(_cairo);
  cairo_surface_destroy(_buffer);
}

void EmuVideoMemory::set(const uint8_t& x,
                         const uint8_t& y,
                         const uint8_t& color) {
  // Decode the color. The most significant three bits represent
  // red, the middle three bits represent green, and the lowest
  // two bits represent blue. We want the color as a double from
  // 0.0 to 1.0 for cairo.
  double red = (color & 0xe0) / 255.0;
  double green = ((color & 0x1c) << 3) / 255.0;
  double blue = ((color & 0x3) << 6) / 255.0;
  // Paint the pixel specified by (x, y) to the buffer
  cairo_set_source_rgb(_cairo, red, green, blue);
  cairo_rectangle(_cairo, x, y, 1, 1);
  cairo_fill(_cairo);
}
