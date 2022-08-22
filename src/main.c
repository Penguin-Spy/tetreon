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
#include "counter.h"

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
  int8_t hand_x = 3, hand_y = 0;
  enum tetrimino hand_tetrimino = J;
  enum rotation  hand_rotation = Up;
  enum tetrimino hold_tetrimino = None;

  // start up a timer for tick counters, do not move:
  timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_NOINT | TIMER1_UP;
  timer_1_Counter = timer_1_ReloadValue = 0;

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

    /* ----- Tetrimino Movement ----- */

    /* Tetrimino shifting */
    if(HOLDING_LEFT && check_counter_move) {
      if(CheckCollision(hand_tetrimino, hand_rotation, --hand_x, hand_y, playfield)) {
        hand_x++;
      } else {  // move successful, coordinate already changed by if statement
        if(controls & ctrl_Left) counter_move_set_start();
        else counter_move_set_hold();

        counter_lock_reset();
      }
    }
    if(HOLDING_RIGHT && check_counter_move) {
      if(CheckCollision(hand_tetrimino, hand_rotation, ++hand_x, hand_y, playfield)) {
        hand_x--;
      } else {  // move successful, coordinate already changed by if statement
        if(controls & ctrl_Right) counter_move_set_start();
        else counter_move_set_hold();

        counter_lock_reset();
      }
    }
    // Hard drop
    if(controls & ctrl_Up) {
      uint8_t i;

      for(i = hand_y, i < PLAYFIELD_HEIGHT; i++;) {
        if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, i, playfield)) {
          // we collide here, lock piece
          hand_y = i - 1;
          counter_lock_set();
          break;
        }
      }
    }
    // Soft drop
    if(HOLDING_DOWN && check_counter_move) {
      counter_gravity_reset();  // make sure gravity doesn't interfere w/ soft dropping the piece
      counter_lock_reset();     // regardless of if we move or hit the ground, reset the lock delay

      if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, ++hand_y, playfield)) {
        hand_y--;

      } else {  // move successful, coordinate already changed by if statement
        counter_move_set_hold();
      }
    }

    /* Tetrimino rotation */
    if(controls & ctrl_RotL) {
      enum rotation test_rotation = hand_rotation;

      test_rotation--;
      if(test_rotation < Up) test_rotation = Left;

      if(RotateAndKick(hand_tetrimino, hand_rotation, &test_rotation, &hand_x, &hand_y, playfield)) {
        hand_rotation = test_rotation;

        counter_lock_reset();
      }
    }
    if(controls & ctrl_RotR) {
      enum rotation test_rotation = hand_rotation;

      test_rotation++;
      if(test_rotation > Left) test_rotation = Up;

      if(RotateAndKick(hand_tetrimino, hand_rotation, &test_rotation, &hand_x, &hand_y, playfield)) {
        hand_rotation = test_rotation;

        counter_lock_reset();
      }
    }


    /* Tetrimino holding (just incs piece for now) */
    if(controls & ctrl_Hold) {
      hand_tetrimino += 1;
      if(hand_tetrimino > TETRIMINO_COUNT) hand_tetrimino = I;
    }


    /* Tetrimino gravity */
    if(check_counter_gravity) {
      counter_gravity_reset();

      if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, ++hand_y, playfield)) {
        hand_y--;
      } else {  // move successful, coordinate already changed by if statement
        counter_lock_reset();
      }
    }

    /* Tetrimino locking */
    if(check_counter_lock) {
      if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, ++hand_y, playfield)) {
        PlaceTetrimino(hand_tetrimino, hand_rotation, hand_x, --hand_y, playfield);

        // new tetrimino
        hand_tetrimino += 1;
        if(hand_tetrimino > TETRIMINO_COUNT) hand_tetrimino = I;
        counter_lock_reset();
        counter_gravity_reset();
        counter_move_set_hold();

        hand_x = 3;
        hand_y = 0;

        DrawPlayfield(playfield);
        gfx_BlitBuffer();
        ClearLines(playfield);

      } else {
        hand_y--;
      }
    }


    gfx_SetTextXY(228, 8*8);
    gfx_PrintInt(hand_tetrimino, 1);
    gfx_SetTextXY(244, 8*8);
    gfx_PrintInt(hand_rotation, 1);

    /* ----- Render ----- */
    DrawPlayfield(playfield);
    DrawTetrimino(hand_tetrimino, hand_rotation, hand_x, hand_y);

    gfx_BlitBuffer();

    /* ----- Counters ----- */

    // decrement all counters every 1/16 second (1 "tick")
    if(timer_1_Counter > COUNTER_TICK) {
      timer_1_Counter = 0;
      decrement_counters();
    }

    gfx_SetTextXY(0, 200);
    gfx_PrintUInt(counter_move, 1);
    gfx_SetTextXY(0, 208);
    gfx_PrintUInt(counter_gravity, 1);
    gfx_SetTextXY(0, 216);
    gfx_PrintUInt(counter_lock, 1);

  } while(kb_Data[6] != kb_Clear);

  gfx_End();

  return 0;
}

int CheckCollision(enum tetrimino hand_tetrimino, enum rotation test_rotation, int8_t test_x, int8_t test_y, uint8_t playfield[20][10]) {
  uint8_t r, c;
  int8_t check_x = 0, check_y = 0;

  uint8_t* shape = GET_TETRIMINO_SHAPE(hand_tetrimino, test_rotation);

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
      if(*(shape+(r*4)+c) == 1) {
        check_x = test_x + c;
        if(check_x < 0 || check_x > PLAYFIELD_WIDTH-1) return 1;

        check_y = test_y + r;
        if(check_y < 0 || check_y > PLAYFIELD_HEIGHT-1) return 1;

        if(playfield[test_y+r][test_x+c]) {
          return 1;
        }
      }
    }
  }
  return 0;
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

  uint8_t* shape = GET_TETRIMINO_SHAPE(hand_tetrimino, hand_rotation);

  gfx_SetColor(hand_tetrimino);

  // for i=15,0,-1 do    *(shape + (1<<i) ) == 1

  for(r = 0; r < 4; r++) {
    for(c = 0; c < 4; c++) {
      if(*(shape+(r*4)+c) == 1) {
        playfield[y+r][x+c] = hand_tetrimino;
      }
    }
  }
}

void ClearLines(uint8_t playfield[20][10]) {
  uint8_t r, c, gap, i, r2;

  // Loop through matrix
  for(r = 0; r < PLAYFIELD_HEIGHT; r++) {
    gap = 0;

    for(c = 0; c < PLAYFIELD_WIDTH; c++) {
      if(playfield[r][c] == 0) gap = 1;
    }

    if(!gap) { // the full row is solid, play clear animation
      gfx_SetColor(COLOR_CLEAR);
      gfx_SetDrawScreen();

      i = 3;
      do {
        gfx_FillRectangle(100, 12*r, 12*PLAYFIELD_WIDTH, 12);
        delay(75);
        gfx_BlitBuffer();
        delay(75);
      } while(--i);

      gfx_SetDrawBuffer();

      // then actually clear the line & shift everything down
      for(c = 0; c < PLAYFIELD_WIDTH; c++) {
        for(r2 = r; r2 > 1; r2--) {
          playfield[r2][c] = playfield[r2-1][c];
        }
        playfield[0][c] = 0;
      }
    }
  }
}

/* x, y are the position in the playfield of the top-left corner of the tetrimino */
void DrawTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, int8_t x, int8_t y) {
  uint8_t r, c;

  uint8_t* shape = GET_TETRIMINO_SHAPE(hand_tetrimino, hand_rotation);

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
      if(playfield[r][c] != None) {
        gfx_SetColor(playfield[r][c]);
        gfx_FillRectangle_NoClip(100 + 12*c, 12*r, 12, 12);
      }
    }
  }
}