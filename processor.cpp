#include <iostream>
#include <fstream>
#include <string.h>
#include "processor.h"

EmuProcessor::EmuProcessor(EmuWindow *window,
                           const std::string& infile_name)
                           : _window(window),
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

void EmuProcessor::_push(const uint16_t& val) {
  uint16_t sp = _registers[REG_SP];
  _mainMem[sp] = val >> 8;
  _mainMem[sp+1] = val & 0xff;
  _registers[REG_SP] += 2;
}

uint16_t EmuProcessor::_pop() {
  uint16_t sp = _registers[REG_SP];
  uint16_t val = *((uint16_t *)&_mainMem[sp]);
  _registers[REG_SP] -= 2;
  return val;
}

void EmuProcessor::_setInstructionPointer(const uint16_t& ip) {
  // Clear out the bottom two bits, because the instruction
  // pointer must be 4-byte aligned.
  _instructionPointer = ip & 0xfffc;
}

void EmuProcessor::execute() {
  while (_running) {
    // Execute next instruction
    uint8_t *inst = &_mainMem[_instructionPointer];
    uint8_t opcode = inst[0];
    uint8_t arg1 = inst[1];
    uint8_t arg2 = inst[2];
    uint16_t argA = *((uint16_t *)&inst[1]);
    uint16_t argB = *((uint16_t *)&inst[2]);
    uint16_t nextInstPtr = _instructionPointer + INST_SIZE;
    switch (opcode) {
    case OPCODE_NOP:
    default:
      // Do nothing
      break;
    case OPCODE_INPUT:
      // TODO: Implement input
      break;
    case OPCODE_CALL:
      // CALL ADDR
      // Push the instruction pointer onto the stack and jump to
      // address within the current instruction.
      _push(_instructionPointer);
      nextInstPtr = argA;
      break;
    case OPCODE_RET:
      // RET NUM
      // Pop the instruction pointer off the stack, add the provided
      // argument, and jump to it.
      nextInstPtr = _pop() + arg1 + INST_SIZE;
      break;
    case OPCODE_LOAD:
      // LOAD DEST SRC
      // Load the memory pointed to by SRC and put it in DEST.
      _registers[arg1] = _mainMem[_registers[arg2]];
      break;
    case OPCODE_LOADI:
      // LOADI DEST ADDR
      // Load the memory at ADDR and put it in DEST.
      _registers[arg1] = _mainMem[argB];
      break;
    case OPCODE_MOV:
      // MOV DEST SRC
      _registers[arg1] = _registers[arg2];
      break;
    case OPCODE_MOVI:
      // MOV DEST VALUE
      _registers[arg1] = argB;
      break;
    case OPCODE_PUSH:
      // PUSH REG
      _push(_registers[arg1]);
      break;
    case OPCODE_POP:
      // POP REG
      _registers[arg1] = _pop();
      break;
    case OPCODE_ADD:
      // ADD DEST SRC
      _registers[arg1] += _registers[arg2];
      break;
    case OPCODE_SUB:
      // SUB DEST SRC
      _registers[arg1] -= _registers[arg2];
      break;
    case OPCODE_MUL:
      // MUL DEST SRC
      _registers[arg1] *= _registers[arg2];
      break;
    case OPCODE_DIV:
      // DIV DEST SRC
      _registers[arg1] /= _registers[arg2];
      break;
    case OPCODE_AND:
      // AND DEST SRC
      _registers[arg1] &= _registers[arg2];
      break;
    case OPCODE_OR:
      // OR DEST SRC
      _registers[arg1] |= _registers[arg2];
      break;
    case OPCODE_XOR:
      // XOR DEST SRC
      _registers[arg1] ^= _registers[arg2];
      break;
    case OPCODE_SHL:
      // SHL DEST SRC
      _registers[arg1] <<= _registers[arg2];
      break;
    case OPCODE_SHRA:
      // SHRA DEST SRC
      // Arithmetic right shift, we have to make sure to sign extend
      _registers[arg1] = (uint8_t)((int8_t)_registers[arg1] >> _registers[arg2]);
      break;
    case OPCODE_SHRL:
      // SHRL DEST SRC
      // Logical right shift, no sign extend
      _registers[arg1] >>= _registers[arg2];
      break;
    case OPCODE_CMP:
      // CMP DEST SRC
      // Do DEST - SRC and set the flags, but discard the result.
      // TODO: Implement CMP
      break;
    case OPCODE_TST:
      // TST DEST SRC
      // Do DEST & SRC and set the flags, but discard the result.
      // TODO: Implement TST
      break;
    case OPCODE_COLOR:
      // COLOR REG
      // Sets the value of the color register equal to the value
      // of the argument register.
      _colorRegister = (uint8_t)_registers[arg1];
      break;
    case OPCODE_PIXEL:
      // PIXEL X Y
      // Sets the point (X, Y) equal to the value of the color register
      _window->setPixel(arg1, arg2, _colorRegister);
      break;
    case OPCODE_JMP:
      // JMP REG
      // Jumps to the address value in REG.
      nextInstPtr = _registers[arg1];
      break;
    case OPCODE_JMPI:
      // JMPI ADDR
      // Unconditional jump to ADDR.
      nextInstPtr = argA;
      break;
    case OPCODE_JEQ:
      // JEQ ADDR
      if (_zeroFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JNE:
      // JNE ADDR
      if (!_zeroFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JG:
      // JG ADDR
      if (!_zeroFlag && _signFlag == _overflowFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JGE:
      // JGE ADDR
      if (_signFlag == _overflowFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JA:
      // JA ADDR
      if (!_carryFlag && !_zeroFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JAE:
      // JAE ADDR
      if (!_carryFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JL:
      // JL ADDR
      if (_signFlag != _overflowFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JLE:
      // JLE ADDR
      if (_signFlag != _overflowFlag || _zeroFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JB:
      // JB ADDR
      if (_carryFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JBE:
      // JBE ADDR
      if (_carryFlag || _zeroFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JO:
      // JO ADDR
      if (_overflowFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JNO:
      // JNO ADDR
      if (!_overflowFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JS:
      // JS ADDR
      if (_signFlag) {
        nextInstPtr = argA;
      }
      break;
    case OPCODE_JNS:
      // JNS
      if (!_signFlag) {
        nextInstPtr = argA;
      }
      break;
    }

    // Set the instruction pointer to its new position
    _setInstructionPointer(nextInstPtr);
  }
}
