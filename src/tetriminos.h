
#include <tice.h>

#ifndef H_TETRIMINOS
#define H_TETRIMINOS

/* Graphics */

uint16_t palette[] = {
  1023,  // 0b0.00000.11111.11111
  32736, // 0b0.11111.11111.00000
  31775, // 0b0.11111.00000.11111
  992,   // 0b0.00000.11111.00000
  31744, // 0b0.11111.00000.00000
  31,    // 0b0.00000.00000.11111
  32256, // 0b0.11111.10000.00000

  0,      // playfield bg
  0x2529, // side
  0x7FFF, // text
};
#define PALETTE_SIZE sizeof(palette)

#define COLOR_BG 7
#define COLOR_SIDE COLOR_BG+1
#define COLOR_TEXT COLOR_BG+2


/* Physics */

#define PLAYFIELD_WIDTH 10
#define PLAYFIELD_HEIGHT 20

enum tetrimino { I, O, T, S, Z, J, L, None }; // none is for the hold slot, which is empty by default
enum rotation { Up, Right, Down, Left };  // based on the pointy bit of the T

#define TETRIMINO_COUNT     7
#define TETRIMINO_ROTATIONS 4
#define TETRIMINO_SIZE      4*4

// ew but works
uint8_t rotations[] = {
  // I
  0,0,0,0,
  0,0,0,0,
  1,1,1,1,
  0,0,0,0,

  0,1,0,0,
  0,1,0,0,
  0,1,0,0,
  0,1,0,0,

  0,0,0,0,
  1,1,1,1,
  0,0,0,0,
  0,0,0,0,

  0,0,1,0,
  0,0,1,0,
  0,0,1,0,
  0,0,1,0,

  // O
  0,0,0,0,
  0,1,1,0,
  0,1,1,0,
  0,0,0,0,

  0,0,0,0,
  0,1,1,0,
  0,1,1,0,
  0,0,0,0,

  0,0,0,0,
  0,1,1,0,
  0,1,1,0,
  0,0,0,0,

  0,0,0,0,
  0,1,1,0,
  0,1,1,0,
  0,0,0,0,

  // T
  0,0,0,0,
  0,1,0,0,
  1,1,1,0,
  0,0,0,0,

  0,0,0,0,
  0,1,0,0,
  0,1,1,0,
  0,1,0,0,

  0,0,0,0,
  0,0,0,0,
  1,1,1,0,
  0,1,0,0,

  0,0,0,0,
  0,1,0,0,
  1,1,0,0,
  0,1,0,0,

  // S
  0,0,0,0,
  0,1,1,0,
  1,1,0,0,
  0,0,0,0,

  0,0,0,0,
  0,1,0,0,
  0,1,1,0,
  0,0,1,0,

  0,0,0,0,
  0,0,0,0,
  0,1,1,0,
  1,1,0,0,

  0,0,0,0,
  1,0,0,0,
  1,1,0,0,
  0,1,0,0,

  // Z
  0,0,0,0,
  0,0,0,0,
  1,1,0,0,
  0,1,1,0,

  0,0,0,0,
  0,0,1,0,
  0,1,1,0,
  0,1,0,0,

  0,0,0,0,
  1,1,0,0,
  0,1,1,0,
  0,0,0,0,

  0,0,0,0,
  0,1,0,0,
  1,1,0,0,
  1,0,0,0,

  // J
  0,0,0,0,
  1,0,0,0,
  1,1,1,0,
  0,0,0,0,

  0,0,0,0,
  0,1,1,0,
  0,1,0,0,
  0,1,0,0,

  0,0,0,0,
  0,0,0,0,
  1,1,1,0,
  0,0,1,0,

  0,0,0,0,
  0,1,0,0,
  0,1,0,0,
  1,1,0,0,

  // L
  0,0,0,0,
  0,0,1,0,
  1,1,1,0,
  0,0,0,0,

  0,0,0,0,
  0,1,0,0,
  0,1,0,0,
  0,1,1,0,

  0,0,0,0,
  0,0,0,0,
  1,1,1,0,
  1,0,0,0,

  0,0,0,0,
  1,1,0,0,
  0,1,0,0,
  0,1,0,0
};

#endif