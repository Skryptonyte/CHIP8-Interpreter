# What's this?

A CHIP8 Interpreter written in C and the NCurses Library.

### Progress

  * Most opcodes are implemented
  * Most applications will start up, however I would not recommend it for general use yet
  * Display and keyboard support implemented using NCurses
  * Interpreter runs at 500Hz clock speed.

### What works?

TETRIS works...


Other games and apps go in-game but suffer from issues like corrupted graphics, segfaults (TICTAC and UFO in particular) etc. TICTAC in particular seems to suffer from a Heisenbug.


BLINKY loads but is not anywhere near the playable range yet.

### TODO
  * Implement Sound Timer
  * Complete rewrite of the DRW Instruction
  * Fix opcodes to improve compatibility
  * Support for Windows WITH PDCurses
  * Option to disassemble to a file
  * Super CHIP-8 Support ( Far future )
  

### Requirements for compilation and use

  * GCC/Clang
  * NCurses
  * A terminal that supports Unicode


To compile: `gcc driver.c -lncursesw`
