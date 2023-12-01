#ifndef H_TETREON
#define H_TETREON

#include <tice.h>
#include "tetriminos.h"

/* Function prototypes */
void DrawTetrimino(tetrimino_t hand_tetrimino, rotation_t hand_rotation, int8_t x, int8_t y);
void DrawPlayfield(uint8_t playfield[20][10]);
int CheckCollision(tetrimino_t hand_tetrimino, rotation_t test_rotation, int8_t test_x, int8_t test_y, uint8_t playfield[20][10]);
int RotateAndKick(tetrimino_t test_tetrimino, rotation_t from_rot, rotation_t* to_rot, int8_t* start_x, int8_t* start_y, uint8_t playfield[20][10]);
void PlaceTetrimino(tetrimino_t hand_tetrimino, rotation_t hand_rotation, int8_t x, int8_t y, uint8_t playfield[20][10]);
void ClearLines(uint8_t playfield[20][10]);

#define ctrl_Up    (1<<0)
#define ctrl_Down  (1<<1)
#define ctrl_Left  (1<<2)
#define ctrl_Right (1<<3)
#define ctrl_RotL  (1<<4)
#define ctrl_RotR  (1<<5)
#define ctrl_Hold  (1<<6)
#define ctrl_Pause (1<<7)

#define HOLDING_UP kb_Data[7] & kb_Up
#define HOLDING_DOWN kb_Data[7] & kb_Down
#define HOLDING_LEFT kb_Data[7] & kb_Left
#define HOLDING_RIGHT kb_Data[7] & kb_Right

typedef struct {
  int8_t x;
  int8_t y;
} vector_t;

vector_t tests[4] = {
  { 1, 0 },
  { 1, 1 },
  { 0, 2 },
  { 1, 2 }
};

#endif