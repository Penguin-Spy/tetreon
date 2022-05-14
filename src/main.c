/*
 *--------------------------------------
 * Program Name: Tetreon
 * Author: Penguin_Spy
 * License: MIT
 * Description: Tetris clone with modern tetris rules
 *--------------------------------------
*/

#include <tice.h>
#include <keypadc.h>
#include <graphx.h>

#include "tetreon.h"
#include "tetriminos.h"

int main(void) {
  uint8_t controls_cur = 0;  // current state of controls
  uint8_t controls_prev = 0; // previous state of controls
  uint8_t controls = 0;      // set for one frame when key pressed

  const char* erase_string = "";

  /* game state variables */
  uint8_t playfield[20][10] = {
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0 },
    { 0,0,0,0,2,2,0,0,0,0 },
    { 0,0,0,0,0,2,2,2,0,0 },
    { 0,0,0,0,0,0,2,2,2,2 },
    { 0,4,4,4,0,0,0,2,2,2 },
    { 0,4,4,0,0,0,2,2,2,2 },
    { 4,4,4,4,0,0,2,2,2,2 },
    { 4,4,4,4,4,0,2,2,2,2 }
  };
  int8_t hand_x = 0, hand_y = 0;
  enum tetrimino hand_tetrimino = J;
  enum rotation  hand_rotation = Up;
  enum tetrimino hold_tetrimino = None;

  // start up a timer for tetrimino gravity & locking, do not move:
  /*timer_Control = TIMER1_DISABLE | TIMER2_DISABLE;*/
  timer_2_Counter = timer_2_ReloadValue = 32768;

  //timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_NOINT | TIMER1_UP   |   TIMER2_ENABLE | TIMER2_32K | TIMER2_0INT  | TIMER2_DOWN;
  timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_NOINT | TIMER1_UP   |   TIMER2_ENABLE | TIMER2_32K | TIMER2_NOINT  | TIMER2_UP;
  timer_1_Counter = timer_1_ReloadValue = 32768;
  timer_2_Counter = timer_2_ReloadValue = 32768;

  gfx_Begin();
  gfx_SetDrawBuffer();

  // Load palette
  gfx_SetPalette(palette, PALETTE_SIZE, 0);

  // Setup screen
  gfx_FillScreen(COLOR_SIDE);
  gfx_SetTextFGColor(COLOR_TEXT);
  gfx_SetTextBGColor(COLOR_BG);

  gfx_SetColor(COLOR_BG);
  gfx_FillRectangle(100, 0, 120, 240);

  /*while(os_GetCSC());
  while(!os_GetCSC());

  gfx_End();
  return 0;*/

  do {
    /* Update kb_Data */
    kb_Scan();

    controls_cur = 0;
    controls_cur |= kb_Data[7] & kb_Up ? ctrl_Up : 0;
    controls_cur |= kb_Data[7] & kb_Down ? ctrl_Down : 0;
    controls_cur |= kb_Data[7] & kb_Left ? ctrl_Left : 0;
    controls_cur |= kb_Data[7] & kb_Right ? ctrl_Right : 0;
    controls_cur |= kb_Data[1] & kb_2nd ? ctrl_RotL : 0;
    controls_cur |= kb_Data[2] & kb_Alpha ? ctrl_RotR : 0;
    controls_cur |= kb_Data[1] & kb_Mode ? ctrl_Pause : 0;
    controls_cur |= kb_Data[3] & kb_GraphVar || kb_Data[1] & kb_Yequ ? ctrl_Hold : 0;

    controls = controls_cur & (~controls_prev);
    controls_prev = controls_cur;

    /* Print the current arrow key input */
    gfx_FillRectangle(228, 0, 40, 8*8);
    gfx_PrintStringXY(controls & ctrl_Up    ? "Up"    : erase_string, 228, 0);
    gfx_PrintStringXY(controls & ctrl_Down  ? "Down"  : erase_string, 228, 1*8);
    gfx_PrintStringXY(controls & ctrl_Left  ? "Left"  : erase_string, 228, 2*8);
    gfx_PrintStringXY(controls & ctrl_Right ? "Right" : erase_string, 228, 3*8);
    gfx_PrintStringXY(controls & ctrl_RotL  ? "RotL"  : erase_string, 228, 4*8);
    gfx_PrintStringXY(controls & ctrl_RotR  ? "RotR"  : erase_string, 228, 5*8);
    gfx_PrintStringXY(controls & ctrl_Hold  ? "Hold"  : erase_string, 228, 6*8);
    gfx_PrintStringXY(controls & ctrl_Pause ? "Pause" : erase_string, 228, 7*8);


    /* Tetrimino shifting */
    if(controls & ctrl_Left) {
      timer_1_Counter = 0;
      if(CheckCollision(hand_tetrimino, hand_rotation, --hand_x, hand_y, playfield)) {
        hand_x++;
      }
    }
    if(controls & ctrl_Right) {
      timer_1_Counter = 0;
      if(CheckCollision(hand_tetrimino, hand_rotation, ++hand_x, hand_y, playfield)) {
        hand_x--;
      }
    }
    if(controls & ctrl_Up) {
      //TODO: hard drop
      if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, --hand_y, playfield)) {
        hand_y++;
      }
    }
    if(controls & ctrl_Down) {
      timer_1_Counter = 0;
      //TODO: soft drop
      if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, ++hand_y, playfield)) {
        hand_y--;
      }
    }

    /* Tetrimino rotation */
    if(controls & ctrl_RotL) {
      enum rotation test_rotation = hand_rotation;

      timer_1_Counter = 0;
      test_rotation--;
      if(test_rotation < Up) test_rotation = Left;

      if(RotateAndKick(hand_tetrimino, hand_rotation, &test_rotation, &hand_x, &hand_y, playfield)) {
        hand_rotation = test_rotation;
      }
    }
    if(controls & ctrl_RotR) {
      enum rotation test_rotation = hand_rotation;

      timer_1_Counter = 0;

      test_rotation++;
      if(test_rotation > Left) test_rotation = Up;

      if(RotateAndKick(hand_tetrimino, hand_rotation, &test_rotation, &hand_x, &hand_y, playfield)) {
        hand_rotation = test_rotation;
      }
    }
    if(hand_rotation > Left) hand_rotation = Up;
    if(hand_rotation < Up) hand_rotation = Left;



    if(controls & ctrl_Hold) {
      hand_tetrimino += 1;
      if(hand_tetrimino >= None) hand_tetrimino = I;
    }

    /* Tetrimino gravity */
    if(timer_2_Counter > 16384) {
      timer_2_Counter = 0;

      if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, ++hand_y, playfield)) {
        hand_y--;
      }
    }
    gfx_SetTextXY(0, 224);
    gfx_PrintUInt(timer_Get(1), 1);
    gfx_SetTextXY(0, 232);
    gfx_PrintUInt(timer_Get(2), 1);

    /* Tetrimino locking */
    if(timer_1_Counter > 16384) {
      timer_1_Counter = 0;

      if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, ++hand_y, playfield)) {
        PlaceTetrimino(hand_tetrimino, hand_rotation, hand_x, --hand_y, playfield);

        hand_tetrimino += 1;
        if(hand_tetrimino >= None) hand_tetrimino = I;

        hand_x = 4;
        hand_y = 0;
      } else {
        hand_y--;
      }
    }






    gfx_SetTextXY(228, 8*8);
    gfx_PrintInt(hand_tetrimino, 1);
    gfx_SetTextXY(244, 8*8);
    gfx_PrintInt(hand_rotation, 1);

    DrawPlayfield(playfield);

    DrawTetrimino(hand_tetrimino, hand_rotation, hand_x, hand_y);

    gfx_BlitBuffer();

  } while(kb_Data[6] != kb_Clear);

  gfx_End();

  return 0;
}

int CheckCollision(enum tetrimino hand_tetrimino, enum rotation test_rotation, int8_t test_x, int8_t test_y, uint8_t playfield[20][10]) {
  uint8_t r, c, collide = 0;
  int8_t check_x = 0, check_y = 0;

  // Fancy pointer math to get shape of tetrimino
  uint8_t* shape = rotations;
  shape += hand_tetrimino*TETRIMINO_SIZE*TETRIMINO_ROTATIONS +test_rotation*TETRIMINO_SIZE;

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
      if(*(shape+(r*4)+c) == 1) {
        check_x = test_x + c;
        if(check_x < 0 || check_x > PLAYFIELD_WIDTH-1) collide = 1;

        check_y = test_y + r;
        if(check_y < 0 || check_y > PLAYFIELD_HEIGHT-1) collide = 1;

        if(playfield[test_y+r][test_x+c]) {
          collide = 1;
        }
      }
    }
  }
  return collide;
}

// rotates & wallkicks a tetrimino
// the parameters to_rot, start_x, and start_y are pointers, the correct state of the tetrimino after the attempted rotation is returned in those variables

int RotateAndKick(enum tetrimino test_tetrimino, enum rotation from_rot, enum rotation* to_rot, int8_t* start_x, int8_t* start_y, uint8_t playfield[20][10]) {
  int8_t i, test_x, test_y;
  vector_t test;

  if(!CheckCollision(test_tetrimino, *to_rot, *start_x, *start_y, playfield)) {
    return 1; // This tetrimino can rotate as-is, leave params alone and return success
  }

  // literal magic

  for(i=0; i<4; i++) {
    test.x = tests[i].x;
    test.y = tests[i].y;

    if(from_rot == Left || *to_rot == Right) {
      test.x *= -1;
    }

    //if(from_rot == Up || from_rot == Down) {
    if(from_rot == Right || from_rot == Left) {
      test.y *= -1;
    }
    if(i == 1) { // test #3
      test.y *= -1;
    }

    gfx_SetTextXY(*start_x + i*32, *start_y);
    gfx_PrintInt(test.x, 1);
    gfx_PrintChar(',');
    gfx_PrintInt(test.y, 1);


    // collision checks with offset by the test vector
    test_x = *start_x + test.x;
    test_y = *start_y + test.y;

    if(!CheckCollision(test_tetrimino, *to_rot, test_x, test_y, playfield)) {
      // This test found a position this tetrimino can fit in, modify position params and return success
      *start_x = test_x;
      *start_y = test_y;
      return 1;
    }
  }

  // If we reach here, no tests found a position the tetrimino could fit in, reset rotation and return failure
  *to_rot = from_rot;
  return 0;

}

void PlaceTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, int8_t x, int8_t y, uint8_t playfield[20][10]) {
  uint8_t r, c;

  // Fancy pointer math to get shape of tetrimino
  uint8_t* shape = rotations;
  shape += hand_tetrimino*TETRIMINO_SIZE*TETRIMINO_ROTATIONS +hand_rotation*TETRIMINO_SIZE;

  gfx_SetColor(hand_tetrimino);

  // for i=15,0,-1 do    *(shape + (1<<i) ) == 1

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
      if(*(shape+(r*4)+c) == 1) {
        playfield[y+r][x+c] = hand_tetrimino + 1;
      }
    }
  }
}

/* x, y are the position in the playfield of the top-left corner of the tetrimino */
void DrawTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, int8_t x, int8_t y) {
  uint8_t r, c;

  // Fancy pointer math to get shape of tetrimino
  uint8_t* shape = rotations;
  shape += hand_tetrimino*TETRIMINO_SIZE*TETRIMINO_ROTATIONS +hand_rotation*TETRIMINO_SIZE;

  gfx_SetColor(hand_tetrimino);

  // for i=15,0,-1 do    *(shape + (1<<i) ) == 1

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
      if(*(shape+(r*4)+c) == 1) {
        gfx_FillRectangle(100 + 12*c + 12*x, 12*r + 12*y, 12, 12);
      }
    }
  }
}

/* Values of the array = type of tetrimino square that is at that location (used for which color to draw) */
void DrawPlayfield(uint8_t playfield[20][10]) {
  uint8_t r, c;

  // Clear playfield
  gfx_SetColor(COLOR_BG);
  gfx_FillRectangle(100, 0, 120, 240);

  // Loop through matrix
  for(r = 0; r < PLAYFIELD_HEIGHT; r++) {
    for(c = 0; c < PLAYFIELD_WIDTH; c++) {
      if(playfield[r][c] != 0) {
        gfx_SetColor(playfield[r][c] - 1);
        gfx_FillRectangle_NoClip(100 + 12*c, 12*r, 12, 12);
      }
    }
  }
}