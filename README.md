# What's this?

A CHIP8 Interpreter written in C and the NCurses Library.

### Progress

  * All CHIP-8 opcodes are implemented
  * Most applications will work, however I would not recommend it for general use yet mainly due to input system needing improvement
  * Display and keyboard support implemented using NCurses
  * Interpreter runs at 1000Hz clock speed.

Changelog: https://github.com/Skryptonyte/CHIP8-Interpreter/blob/master/CHANGELOG.md
### What works?

Most games now work. Most of the game breaking problems was because of improper writing of switch statement but with hardcore debugging, I finally made it through E17 meaning my emulator now passes all 17 tests of the TEST ROM with a flying BON.

The test ROM (https://slack-files.com/T3CH37TNX-F3RKEUKL4-b05ab4930d) makes it up to E17. 
### TODO
  * Add enhancements for CHIP-8
    * Super CHIP-8 Instruction set
    * Mega CHIP-8 Instruction set
  * Option to disassemble to a file
  

### Requirements for compilation and use

  * GCC/Clang ( or MinGW on Windows )
  * NCurses (on UNIX based systems) / PDCurses (for Windows/UNIX based systems)
  * A terminal that supports Unicode


To compile: `gcc driver.c -lncursesw`

To compile with PDCurses: `gcc driver.c -lpdcurses`

PDCurses is essentially an SDL wrapper for NCurses
