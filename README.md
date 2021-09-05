# What's this?

A CHIP8 Interpreter written in C and the NCurses Library.

### Progress

  * All CHIP-8 opcodes are implemented
  * Input mostly function in OpenGL version, bit laggy though
  * Display and keyboard support implemented with OpenGL and GLFW.
  * Interpreter can run at selected clock cycles between 1 and 100 per frame. (Adjust accordingly to your display refresh rate)
  * CHIP-8, HiRES and Super-CHIP8 support.


Changelog: https://github.com/Skryptonyte/CHIP8-Interpreter/blob/opengl/CHANGELOG.md
### What works?

All CHIP-8 games and SUPER CHIP-8 games ive tested so far work

### TODO
  * Add enhancements for CHIP-8
    * Mega CHIP-8 Instruction set
    * Add some tweaks for certains games with different behaviour
  * Option to disassemble to a file
  

### Requirements for compilation and use

  * GCC/Clang ( or MinGW on Windows )
  * OpenGL, GLFW library


To compile: 
`
gcc driver.c -lglfw -lpthread -lGLEW -lGL 
`
