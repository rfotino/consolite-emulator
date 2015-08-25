/**
 * Consolite Emulator
 * Copyright (c) 2015 Robert Fotino, All Rights Reserved
 */

#ifndef EMU_WINDOW_H
#define EMU_WINDOW_H

#include <X11/Xlib.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <map>
#include "vidmem.h"
#include "defs.h"

class EmuWindow {
 public:
  EmuWindow(EmuVideoMemory *vid_mem, const std::string& keymap_filename);
  ~EmuWindow();
  void eventLoop();
  void setPixel(const uint8_t& x,
                const uint8_t& y,
                const uint8_t& color);
  uint16_t getInput(const uint16_t& input_id);
  bool hasError() { return _error; }

 private:
  void _loadKeyMap(const std::string& keymap_filename);
  void _draw();
  void _updateKeyState(const XKeyEvent& event);

  cairo_surface_t *_surface;
  cairo_t *_cairo;
  Display *_display;
  Atom _wmDeleteMessage;
  EmuVideoMemory *_vidMem;
  bool _error;
  int _width;
  int _height;
  // input_id, KeySym
  std::map<uint16_t, KeySym> _keyMap;
  // KeySym, state
  std::map<KeySym, uint16_t> _keyState;
};

#endif
