#ifndef EMU_DEFS_H
#define EMU_DEFS_H

#define VIDEO_WIDTH 256
#define VIDEO_HEIGHT 192
#define DEFAULT_WINDOW_SCALE 3
#define DEFAULT_WINDOW_WIDTH (VIDEO_WIDTH * DEFAULT_WINDOW_SCALE)
#define DEFAULT_WINDOW_HEIGHT (VIDEO_HEIGHT * DEFAULT_WINDOW_SCALE)

#define MAIN_MEMORY_SIZE 65536
#define NUM_REGISTERS 16
#define INST_SIZE 4

#define KEYMAP_FILENAME "keys.txt"

#define OPCODE_NOP   0x00
#define OPCODE_INPUT 0x01
#define OPCODE_CALL  0x02
#define OPCODE_RET   0x03
#define OPCODE_LOAD  0x04
#define OPCODE_LOADI 0x05
#define OPCODE_MOV   0x06
#define OPCODE_MOVI  0x07
#define OPCODE_PUSH  0x08
#define OPCODE_POP   0x09
#define OPCODE_ADD   0x0A
#define OPCODE_SUB   0x0B
#define OPCODE_MUL   0x0C
#define OPCODE_DIV   0x0D
#define OPCODE_AND   0x0E
#define OPCODE_OR    0x0F
#define OPCODE_XOR   0x10
#define OPCODE_SHL   0x11
#define OPCODE_SHRA  0x12
#define OPCODE_SHRL  0x13
#define OPCODE_CMP   0x14
#define OPCODE_TST   0x15
#define OPCODE_COLOR 0x16
#define OPCODE_PIXEL 0x17
#define OPCODE_JMP   0x30
#define OPCODE_JMPI  0x31
#define OPCODE_JEQ   0x32
#define OPCODE_JNE   0x33
#define OPCODE_JG    0x34
#define OPCODE_JGE   0x35
#define OPCODE_JA    0x36
#define OPCODE_JAE   0x37
#define OPCODE_JL    0x38
#define OPCODE_JLE   0x39
#define OPCODE_JB    0x3A
#define OPCODE_JBE   0x3B
#define OPCODE_JO    0x3C
#define OPCODE_JNO   0x3D
#define OPCODE_JS    0x3E
#define OPCODE_JNS   0x3F

#define REG_SP 0x0
#define REG_FP 0x1
#define REG_A  0x2
#define REG_B  0x3
#define REG_C  0x4
#define REG_D  0x5
#define REG_E  0x6
#define REG_F  0x7
#define REG_G  0x8
#define REG_H  0x9
#define REG_I  0xA
#define REG_J  0xB
#define REG_K  0xC
#define REG_L  0xD
#define REG_M  0xE
#define REG_N  0xF

#endif
