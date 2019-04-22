# What's this?

A CHIP8 Interpreter written in C and the NCurses Library.

### Progress

  * All CHIP-8 opcodes are implemented
  * Most applications will work, however I would not recommend it for general use yet mainly due to input system needing improvement
  * Display and keyboard support implemented using NCurses
  * Interpreter runs at 1000Hz clock speed.
  * HiRes and CHIP-8 mode support.


Changelog: https://github.com/Skryptonyte/CHIP8-Interpreter/blob/master/CHANGELOG.md
### What works?

All CHIP-8 games work. SUPER CHIP-8 games may also work as long as there is no scrolling.


### TODO
  * Add enhancements for CHIP-8
    * Add scrolling CHIP-8 Instructions
    * Mega CHIP-8 Instruction set
  * Option to disassemble to a file
  

### Requirements for compilation and use

  * GCC/Clang ( or MinGW on Windows )
  * NCurses (on UNIX based systems) / PDCurses (for Windows/UNIX based systems)
  * A terminal that supports Unicode


To compile: `gcc driver.c -lncursesw`

To compile with PDCurses: `gcc driver.c -lpdcurses`

PDCurses is essentially an SDL wrapper for NCurses
