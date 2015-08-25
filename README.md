# Consolite Emulator

This program takes in a binary file created with the
[Consolite Assembler](https://github.com/rfotino/consolite-assembler)
and executes it, emulating the display, main memory, registers, and
input. The display is emulated with an X window, and keys on your
keyboard can be mapped to input_ids with a keymap file.

## What Is Consolite?

Consolite (coming from "Console Lite") is the name I've given to my design of a
hobbyist microprocessor and associated toolchain. Consolite's instruction set
architecture (ISA) has assembly level instructions for setting colors, drawing
pixels on the screen, and receiving input. This makes it suitable for writing
games, which is what makes it a "console".

### Consolite Specs

* Display: 256 x 192 pixels, 8-bit color
* Registers: 16 general purpose, instruction pointer, color, flags
* Memory: 64KiB main memory, 48KiB video memory

## Building and Dependencies

This version of the emulator is written in C++ and makes use of the Xlib
and Cairo graphics libraries. You will need to have the development versions
of both of these libraries so that you can include the proper header files.
After Xlib and Cairo have been installed, you should be able to build with
`make`. I have only tested this on my development machine, which runs
Fedora 22. You may need to edit the Makefile to get it to build properly
on other systems, but the Makefile for this project is fairly straightforward.

## Usage

```./emu INFILE [KEYMAP]```

`KEYMAP` is an optional key mapping file, if this argument is not supplied
then the default "keys.txt" file will be used. The format of this file is
`KEYNAME INPUT_ID`. `KEYNAME` is a string recognized by Xlib, essentially
any of the defines in [keysymdef.h](http://www.cl.cam.ac.uk/~mgk25/ucs/keysymdef.h)
without the 'XK_' prefix. `INPUT_ID` is an integer ID used by instructions
in the emulator. For example, you might want a key press from ID 0 to start
the game, so you map the spacebar to ID 0 in the keymap file.

## Copyright

Copyright (c) 2015 Robert Fotino, All Rights Reserved
