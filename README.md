# What's this?

A CHIP8 Interpreter written in C and the NCurses Library.

### Progress

  * Most opcodes are implemented
  * Most applications will start up, however I would not recommend it for general use yet
  * Display and keyboard support implemented using NCurses
  * Interpreter runs at 500Hz clock speed.

### What works?

TETRIS, MAZE and KALEID works...


Other games and apps go in-game but suffer from issues like corrupted graphics and weird behaviour. This is more than likely due to improperly implemented opcodes and the fact that none of the instructions affect the flag register except for DRW.


BLINKY loads but is not anywhere near the playable range yet.

### TODO
  * Implement Sound Timer
  * Complete rewrite of the DRW Instruction
  * Fix opcodes to improve compatibility
  * Option to disassemble to a file
  * Super CHIP-8 Support ( Far future )
  

### Requirements for compilation and use

  * GCC/Clang ( or MinGW on Windows )
  * NCurses (on UNIX based systems / PDCurses (for Windows)
  * A terminal that supports Unicode


To compile: `gcc driver.c -lncursesw`
