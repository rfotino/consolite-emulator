#include <X11/Xlib.h>
#include <thread>
#include <iostream>
#include "vidmem.h"
#include "window.h"
#include "processor.h"

void usage(std::string program_name) {
  std::cerr << "Usage: " << program_name << " INFILE" << std::endl;
}

void win_thread_start(EmuWindow *window) {
  window->eventLoop();
}

void proc_thread_start(EmuProcessor *processor) {
  processor->execute();
}

int main(int argc, char **argv) {
  if (2 != argc) {
    usage(argv[0]);
    return 1;
  }
  
  EmuVideoMemory vidMem;
  EmuWindow window(&vidMem);
  EmuProcessor processor(&window, argv[1]);
  if (window.hasError() || processor.hasError()) {
    return 1;
  }

  // Start up separate threads for the UI and processor
  std::thread winThread(win_thread_start, &window);
  std::thread procThread(proc_thread_start, &processor);

  // Join the threads
  winThread.join();
  processor.setRunning(false);
  procThread.join();
  
  return 0;
}
