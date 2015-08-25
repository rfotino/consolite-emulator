/**
 * Consolite Emulator
 * Copyright (c) 2015 Robert Fotino, All Rights Reserved
 */

#include <X11/XKBlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "window.h"

EmuWindow::EmuWindow(EmuVideoMemory *vid_mem,
                     const std::string& keymap_filename)
                    : _vidMem(vid_mem),
                      _error(false),
                      _width(DEFAULT_WINDOW_WIDTH),
                      _height(DEFAULT_WINDOW_HEIGHT) {
  // Read key mapping into memory
  _loadKeyMap(keymap_filename);

  // Open a connection to the X server.
  _display = XOpenDisplay(nullptr);
  if (nullptr == _display) {
    std::cerr << "Error: Cannot open connection to the X server."
              << std::endl;
    _error = true;
    return;
  }

  // Create a window on the default screen.
  int screen_num = DefaultScreen(_display);
  Window window = XCreateSimpleWindow(
      _display,                         // Connection to the X server
      DefaultRootWindow(_display),      // Parent window
      0,                                // X position
      0,                                // Y position
      _width,                           // Width
      _height,                          // Height
      0,                                // Border width
      BlackPixel(_display, screen_num), // Border color
      BlackPixel(_display, screen_num)  // Background color
  );

  // Create the cairo surface we will use for drawing to the window
  Visual *visual = DefaultVisual(_display, screen_num);
  _surface = cairo_xlib_surface_create(
      _display,     // Connection to the X server
      window,       // A Drawable, the window in this case
      visual,       // A Visual, the screen in this case
      _width,       // The surface width
      _height       // The surface height
  );
  if (nullptr == _surface) {
    _error = true;
    std::cerr << "Error: Cannot get cairo Xlib surface." << std::endl;
    return;
  }

  // Create the cairo object from the window surface
  _cairo = cairo_create(_surface);
  
  // Intercept the WM_DELETE_WINDOW message from the window
  // manager so that we can close the window when the user hits
  // the exit button.
  _wmDeleteMessage = XInternAtom(_display, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(_display, window, &_wmDeleteMessage, 1);
  // Select the kind of window events we want to intercept.
  // StructureNotifyMask lets us intercept window resize events.
  XSelectInput(_display, window, ExposureMask | StructureNotifyMask |
                                 KeyPressMask | KeyReleaseMask);
  // "Map" (show) the window.
  XMapWindow(_display, window);
  // Suppress auto-repeating KeyRelease events
  XkbSetDetectableAutoRepeat(_display, 1, 0);
  // Flush any buffered commands to the X server
  XFlush(_display);
}

EmuWindow::~EmuWindow() {
  // Destroy the cairo object
  cairo_destroy(_cairo);
  // Destroy the cairo Xlib surface
  cairo_surface_destroy(_surface);
  // Close the connection to the X server
  XCloseDisplay(_display);
}

void EmuWindow::_loadKeyMap(const std::string& keymap_filename) {
  std::ifstream keyMapFile(keymap_filename);
  if (!keyMapFile.good()) {
    _error = true;
    std::cerr << "Error: Failed to open keymap '"
              << keymap_filename << "'." << std::endl;
    return;
  }
  // Key map should be in the form
  // KEY INPUT_ID
  std::string line;
  int line_num = 0;
  while (std::getline(keyMapFile, line)) {
    line_num++;
    std::istringstream iss(line);
    std::string keyStr;
    int inputId;
    if (!(iss >> keyStr >> inputId)) {
      continue;
    }
    KeySym keysym = XStringToKeysym(keyStr.c_str());
    if (!keysym) {
      std::cerr << "Warning: Key '" << keyStr << "' not recognized in "
                << keymap_filename << " on line " << line << "."
                << std::endl;
      continue;
    }
    _keyMap[inputId] = keysym;
  }
}

void EmuWindow::_draw() {
  // Scale and paint the video memory's buffer to the window
  double scaleX = (double)_width / _vidMem->getWidth();
  double scaleY = (double)_height / _vidMem->getHeight();
  cairo_identity_matrix(_cairo);
  cairo_scale(_cairo, scaleX, scaleY);
  cairo_set_source_surface(_cairo, _vidMem->getBuffer(), 0, 0);
  cairo_pattern_set_filter(cairo_get_source(_cairo), CAIRO_FILTER_NEAREST);
  cairo_paint(_cairo);
  cairo_surface_flush(_surface);
  XFlush(_display);
}

void EmuWindow::setPixel(const uint8_t& x,
                         const uint8_t& y,
                         const uint8_t& color) {
  // Set the pixel's value in video memory
  _vidMem->set(x, y, color);
}

uint16_t EmuWindow::getInput(const uint16_t& input_id) {
  // Return current input status. If this input_id is
  // not registered to a key or button, return 0.
  auto keySymPtr = _keyMap.find(input_id);
  if (_keyMap.end() == keySymPtr) {
    // This input isn't mapped to anything.
    return 0;
  }
  auto statePtr = _keyState.find(keySymPtr->second);
  if (_keyState.end() == statePtr) {
    // This key hasn't been pressed or released.
    return 0;
  }
  return statePtr->second;
}

void EmuWindow::_updateKeyState(const XKeyEvent& event) {
  // Filter out auto repeat key events
  if (event.type == KeyRelease && XPending(_display)) {
    XEvent nev;
    XPeekEvent(_display, &nev);
    if (nev.type == KeyPress &&
        nev.xkey.time == event.time &&
        nev.xkey.keycode == event.keycode) {
      // Key wasn't really released, get rid of the
      // pending KeyPress event and return
      XNextEvent(_display, &nev);
      return;
    }
  }
  int keysyms_per_keycode_return;
  KeySym *keysym = XGetKeyboardMapping(_display,
                                       event.keycode,
                                       1,
                                       &keysyms_per_keycode_return);
  uint16_t status = KeyPress == event.type ? 1 : 0;
  _keyState[keysym[0]] = status;
  XFree(keysym);
}

void EmuWindow::eventLoop() {
  // Get the file descriptor for the connection to the X server,
  // so that we can listen for input on a timer instead of blocking
  // indefinitely on XNextEvent. The reason for this is we need to wake
  // up every 1/60 of a second to repaint.
  int x11_fd = ConnectionNumber(_display);
  fd_set event_fds;
  struct timeval timer;
  // The running variable is set to false when the user
  // closes the window, which breaks out of the event loop.
  bool running = true;
  while (running) {
    // Set up the set of file descriptors for select()
    FD_ZERO(&event_fds);
    FD_SET(x11_fd, &event_fds);
    // Set timer to 1/60 of a second
    timer.tv_usec = 16667;
    timer.tv_sec = 0;
    if (0 == select(x11_fd + 1, &event_fds, 0, 0, &timer)) {
      // Timer expired
      _draw();
      continue;
    }
    // We received an event so handle it here
    XEvent event;
    XNextEvent(_display, &event);
    switch (event.type) {
    case KeyPress:
    case KeyRelease:
      _updateKeyState(event.xkey);
      break;
    case Expose:
      // Draw to the window
      _draw();
      break;
    case ConfigureNotify:
      // The window was resized, redraw scaled
      if (event.xconfigure.width != _width ||
          event.xconfigure.height != _height) {
        _width = event.xconfigure.width;
        _height = event.xconfigure.height;
        cairo_xlib_surface_set_size(_surface, _width, _height);
        _draw();
      }
      break;
    case ClientMessage:
      // Hit the exit button
      if ((unsigned)event.xclient.data.l[0] == _wmDeleteMessage) {
        running = false;
      }
      break;
    default:
      break;
    }
  }
}
