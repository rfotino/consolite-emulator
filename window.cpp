#include <iostream>
#include "window.h"

EmuWindow::EmuWindow(EmuVideoMemory *vid_mem) 
                    : _vidMem(vid_mem),
                      _error(false),
                      _width(DEFAULT_WINDOW_WIDTH),
                      _height(DEFAULT_WINDOW_HEIGHT) {
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
  
  // Intercept the WM_DELETE_WINDOW message from the window
  // manager so that we can close the window when the user hits
  // the exit button.
  _wmDeleteMessage = XInternAtom(_display, "WM_DELETE_WINDOW", false);
  XSetWMProtocols(_display, window, &_wmDeleteMessage, 1);
  // Select the kind of window events we want to intercept.
  // StructureNotifyMask lets us intercept window resize events.
  XSelectInput(_display, window, ExposureMask | StructureNotifyMask);
  // "Map" (show) the window.
  XMapWindow(_display, window);
}

EmuWindow::~EmuWindow() {
  // Destroy the cairo Xlib surface
  cairo_surface_destroy(_surface);
  // Close the connection to the X server
  XCloseDisplay(_display);
}

void EmuWindow::_draw() {
  // TODO: Draw window from video memory
}

void EmuWindow::eventLoop() {
  // The running variable is set to false when the user
  // closes the window, which breaks out of the event loop.
  bool running = true;
  while (running) {
    XEvent event;
    XNextEvent(_display, &event);
    switch (event.type) {
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
