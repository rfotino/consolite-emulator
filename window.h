#ifndef EMU_WINDOW_H
#define EMU_WINDOW_H

#include <X11/Xlib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include "vidmem.h"
#include "defs.h"

class EmuWindow {
 public:
  EmuWindow(EmuVideoMemory *vid_mem);
  ~EmuWindow();
  void eventLoop();
  void setPixel(const uint8_t& x,
                const uint8_t& y,
                const uint8_t& color);
  uint8_t getInput(const uint16_t& input_id);
  bool hasError() { return _error; }

 private:
  void _draw();
  cairo_surface_t *_surface;
  Display *_display;
  Atom _wmDeleteMessage;
  EmuVideoMemory *_vidMem;
  bool _error;
  int _width;
  int _height;
};

#endif
