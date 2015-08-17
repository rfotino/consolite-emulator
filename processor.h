#ifndef EMU_PROCESSOR_H
#define EMU_PROCESSOR_H

#include <unistd.h>
#include <string>
#include "window.h"
#include "defs.h"

class EmuProcessor {
 public:
  EmuProcessor(EmuWindow *window, const std::string& infile_name);
  void execute();
  bool hasError() { return _error; }
  void setRunning(bool running) { _running = running; }

 private:
  void _push(const uint16_t& val);
  uint16_t _pop();
  void _setInstructionPointer(const uint16_t& ip);
  void _setFlags(const uint32_t& dest,
                 const uint32_t& src,
                 const uint32_t& result);
  EmuWindow *_window;
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
