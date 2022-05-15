#ifndef H_TETREON
#define H_TETREON

#include <tice.h>

/* Function prototypes */
void DrawTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, int8_t x, int8_t y);
void DrawPlayfield(uint8_t playfield[20][10]);
int CheckCollision(enum tetrimino hand_tetrimino, enum rotation test_rotation, int8_t test_x, int8_t test_y, uint8_t playfield[20][10]);
int RotateAndKick(enum tetrimino test_tetrimino, enum rotation from_rot, enum rotation* to_rot, int8_t* start_x, int8_t* start_y, uint8_t playfield[20][10]);
void PlaceTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, int8_t x, int8_t y, uint8_t playfield[20][10]);
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