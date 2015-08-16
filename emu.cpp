#include <X11/Xlib.h>
#include <pthread.h>
#include <iostream>
#include "vidmem.h"
#include "window.h"
#include "processor.h"

void usage(std::string program_name) {
  std::cerr << "Usage: " << program_name << " INFILE" << std::endl;
}

void *win_thread_start(void *win_void_ptr) {
  EmuWindow *window = (EmuWindow *)win_void_ptr;
  window->eventLoop();
  return nullptr;
}

void *proc_thread_start(void *proc_void_ptr) {
  EmuProcessor *processor = (EmuProcessor *)proc_void_ptr;
  processor->execute();
  return nullptr;
}

int main(int argc, char **argv) {
  if (2 != argc) {
    usage(argv[0]);
    return 1;
  }
  
  EmuVideoMemory vidMem;
  EmuWindow window(&vidMem);
  EmuProcessor processor(&vidMem, argv[1]);
  if (window.hasError() || processor.hasError()) {
    return 1;
  }

  // Start up separate threads for the UI and processor
  pthread_t winThread;
  pthread_t procThread;
  if (0 != pthread_create(&winThread, nullptr, win_thread_start, &window)) {
    std::cerr << "Error: Failed to start window thread, exiting now."
              << std::endl;
    return 1;
  }
  if (0 != pthread_create(&procThread, nullptr, proc_thread_start, &processor)) {
    std::cerr << "Error: Failed to start processor thread, exiting now."
              << std::endl;
    return 1;
  }

  // Join the threads
  if (0 != pthread_join(winThread, nullptr)) {
    std::cerr << "Error: Failed to join window thread, exiting now."
              << std::endl;
    return 1;
  }
  processor.setRunning(false);
  if (0 != pthread_join(procThread, nullptr)) {
    std::cerr << "Error: Failed to join processor thread, exiting now."
              << std::endl;
    return 1;
  }
  
  return 0;
}
