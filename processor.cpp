#include <iostream>
#include <fstream>
#include <string.h>
#include "processor.h"

EmuProcessor::EmuProcessor(EmuVideoMemory *vid_mem,
                           const std::string& infile_name)
                           : _vidMem(vid_mem),
                             _error(false),
                             _running(true) {
  // Open up the input file
  std::ifstream input(infile_name, std::ifstream::binary);
  if (!input.good()) {
    _error = true;
    std::cerr << "Error: Failed to read input file '"
              << infile_name << "'." << std::endl;
    return;
  }

  // Make sure the file meets the size requirements
  input.seekg(0, input.end);
  int file_size = input.tellg();
  input.seekg(0, input.beg);
  if (0 == file_size) {
    _error = true;
    std::cerr << "Error: Empty input file '" << infile_name
              << "'." << std::endl;
    return;
  } else if (MAIN_MEMORY_SIZE < file_size) {
    _error = true;
    std::cerr << "Error: File size of " << file_size << " bytes is "
              << "larger than main memory. Input file has a max size of "
              << MAIN_MEMORY_SIZE << " bytes." << std::endl;
    return;
  }

  // Read file contents into main memory
  memset(_mainMem, 0, sizeof(_mainMem));
  input.get((char *)_mainMem, file_size);

  // Initialize registers and other data
  memset(_registers, 0, sizeof(_registers));
  _instructionPointer = 0;
  _colorRegister = 0;
  _overflowFlag = false;
  _carryFlag = false;
  _zeroFlag = false;
  _signFlag = false;
}

void EmuProcessor::execute() {
  while (_running) {
    // TODO: Execute next instruction
  }
}
