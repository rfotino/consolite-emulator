#ifndef EMU_PROCESSOR_H
#define EMU_PROCESSOR_H

#include <unistd.h>
#include <string>
#include "vidmem.h"
#include "defs.h"

class EmuProcessor {
 public:
  EmuProcessor(EmuVideoMemory *vid_mem, const std::string& infile_name);
  void execute();
  bool hasError() { return _error; }
  void setRunning(bool running) { _running = running; }

 private:
  EmuVideoMemory *_vidMem;
  uint8_t _mainMem[MAIN_MEMORY_SIZE];
  uint16_t _registers[NUM_REGISTERS];
  uint16_t _instructionPointer;
  uint8_t _colorRegister;
  bool _overflowFlag;
  bool _carryFlag;
  bool _zeroFlag;
  bool _signFlag;
  bool _error;
  bool _running;
};

#endif
