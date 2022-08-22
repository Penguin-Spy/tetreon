
#include <tice.h>

#ifndef H_TETRIMINOS
#define H_TETRIMINOS

/* Graphics */

uint16_t palette[] = {
  0,      // playfield bg

  1023,  // 0b0.00000.11111.11111
  32736, // 0b0.11111.11111.00000
  31775, // 0b0.11111.00000.11111
  992,   // 0b0.00000.11111.00000
  31744, // 0b0.11111.00000.00000
  31,    // 0b0.00000.00000.11111
  32256, // 0b0.11111.10000.00000

  0x2529, // side
  0x7FFF, // text
};
#define PALETTE_SIZE sizeof(palette)

#define COLOR_NONE 0
#define COLOR_SIDE 8

#define COLOR_BG 0
#define COLOR_TEXT COLOR_SIDE+1

#define COLOR_CLEAR COLOR_TEXT


/* Physics */

#define PLAYFIELD_WIDTH 10
#define PLAYFIELD_HEIGHT 20

enum tetrimino { None, I, O, T, S, Z, J, L }; // none is for the hold slot, which is empty by default, as well as empty spots on the playfield
enum rotation { Up, Right, Down, Left };  // based on the pointy bit of the T

#define TETRIMINO_COUNT     7
#define TETRIMINO_ROTATIONS 4
#define TETRIMINO_SIZE      4*4

// fancy pointer math. tetrimino-1 is BC rotations[0] = I, but the enum I = 1
#define GET_TETRIMINO_SHAPE(tetrimino, rotation) rotations + (tetrimino-1)*TETRIMINO_SIZE*TETRIMINO_ROTATIONS + rotation*TETRIMINO_SIZE;

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