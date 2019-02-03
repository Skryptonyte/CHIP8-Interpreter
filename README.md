# What's this?

A CHIP8 Interpreter written in C and the NCurses Library.

### Progress

  * Most opcodes are implemented
  * Most applications will start up, however I would not recommend it for general use yet
  * Display and keyboard support implemented using NCurses
  * Interpreter runs at 500Hz clock speed.

### What works?

Most games now work ( or at least appear to ). Most of the game breaking problems was because of improper writing of switch statement but with hardcore debugging, I finally made it through E17 meaning my emulator now passes all 17 tests of the TEST ROM with a flying BON.


### TODO
  * Implement Sound Timer
  * Add quirk options to account for games following different CHIP-8 specs.
  * Option to disassemble to a file
  * Super CHIP-8 Support ( Far future )
  

### Requirements for compilation and use

  * GCC/Clang ( or MinGW on Windows )
  * NCurses (on UNIX based systems / PDCurses (for Windows)
  * A terminal that supports Unicode


To compile: `gcc driver.c -lncursesw`
