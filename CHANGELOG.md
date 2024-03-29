

# Release 1.2.0
  * Scroll functions implemented properly (except half scroll), making SCHIP-8 support 99% complete
  * Input system implemented for OpenGL version, Q W E R A S D F Z X C V will act as the emulated keypad.
  * Option to adjust cycles per frame
  

# Release 1.1.0

  * RND function now seeded at the start of execution to make it more random
  * Vertical scrolling implemented ( I don't even know whether this works ). Half pixel scroll (Scrolling in non-extended mdoe) doesn't and probably never will due to the limitations of the implementation of my display. I'd need to do a complete rewrite of the display to get 64x32 to scale to a 128x64 display size.
  * Massive code clean up
  * 10 byte fontset added
  * HiRes mode initialization fixed.

  * New OpenGL branch. (EXPERIMENTAL and UNOPTIMIZED!)

# Release 1.0.1a
  * DRW Instruction had a bug where sprites would appear to have vertical scanlines. This was causes by forgetting to draw the 8th bit of each byte of the sprite.
  * DRW instruction's SCHIP extension turned out to be severely broken. I've fixed that.

# Release 1.0.1

  * RPL User flags added
  * New instructions implemented:
    * 0xFE: Disable Extended mode (128x64)
    * 0xFF: Enable Extended mode
    * Fx75 and Fx85: Move data from registers to RPL flags and vice versa
  * DRW opcode extended: DXY0 will draw 16x16 sprites
  * Delay/Sound Timer now runs at the correct clock-rate. The timers will update exactly every 16.667ms on a separate thread

# Release 1.0.0

  * Display of all the CHIP 8 registers below the display output
  * Instruction Fx18 implemented. Sound Timer however is effectively a stub and behaves just like the Delay Timer for the time being until I figure out low level audio programming.
  * HiRes mode added (64x64 display)
  * Preliminary Super Chip-8 addition: 128x64 display mode
