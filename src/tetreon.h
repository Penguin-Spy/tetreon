#ifndef H_TETREON
#define H_TETREON

#include <tice.h>

/* Function prototypes */
void DrawTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, int8_t x, int8_t y);
void DrawPlayfield(uint8_t playfield[20][10]);
int CheckCollision(enum tetrimino hand_tetrimino, enum rotation test_rotation, int8_t test_x, int8_t test_y, uint8_t playfield[20][10]);


#define ctrl_Up    (1<<0)
#define ctrl_Down  (1<<1)
#define ctrl_Left  (1<<2)
#define ctrl_Right (1<<3)
#define ctrl_RotL  (1<<4)
#define ctrl_RotR  (1<<5)
#define ctrl_Hold  (1<<6)
#define ctrl_Pause (1<<7)


#endif