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
    { 0,1,0,1,0,0,1,1,1,0 },
    { 0,1,1,1,0,0,0,1,0,0 },
    { 0,1,0,1,0,0,1,1,1,0 },
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
    { 0,0,0,0,0,0,0,0,0,0 }
  };
  int8_t hand_x = 0, hand_y = 0;
  enum tetrimino hand_tetrimino = J;
  enum rotation  hand_rotation = Up;
  enum tetrimino hold_tetrimino = None;

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


    /* Tetrimino shifting */
    if(controls & ctrl_Left) {
      //TODO: check collision

      hand_x--;
    }
    if(controls & ctrl_Right) {
      //TODO: check collision

      hand_x++;
    }
    if(controls & ctrl_Up) {
      //TODO: hard drop

      hand_y--;
    }
    if(controls & ctrl_Down) {
      //TODO: soft drop

      hand_y++;
    }

    /* Tetrimino rotation */
    if(controls & ctrl_RotL) {
      //TODO: check collision

      hand_rotation -= 1;

    }
    if(controls & ctrl_RotR) {
      hand_rotation += 1;
    }

    if(hand_rotation > Right) hand_rotation = Up;
    if(hand_rotation < Up) hand_rotation = Right;

    if(controls & ctrl_Hold) {
      hand_tetrimino += 1;
      if(hand_tetrimino >= None) hand_tetrimino = I;
    }
    gfx_SetTextXY(228, 8*8);
    gfx_PrintInt(hand_tetrimino, 1);
    gfx_SetTextXY(244, 8*8);
    gfx_PrintInt(hand_rotation, 1);

    DrawPlayfield(playfield);

    DrawTetrimino(hand_tetrimino, hand_rotation, hand_x, hand_y);

    if(CheckCollision(hand_tetrimino, hand_rotation, hand_x, hand_y, playfield)) {
      gfx_PrintStringXY("collision", 100, 232);
    } else {
      gfx_PrintStringXY("nah               ", 100, 232);
    }


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
      gfx_SetTextXY(c*8, r*8 + 5*8);
      gfx_PrintInt(*(shape+(r*4)+c), 1);
      if(*(shape+(r*4)+c) == 1) {
        gfx_SetColor(COLOR_SIDE);
        gfx_FillRectangle(100 + 12*c + 12*test_x, 12*r + 12*test_y, 12, 12);

        check_x = test_x + c;
        if(check_x < 0 || check_x > PLAYFIELD_WIDTH-1) collide = 1;

        check_y = test_y + r;
        if(check_y < 0 || check_y > PLAYFIELD_HEIGHT-1) collide = 1;
        /*if(test_x + c < 0) {
          collide = 1;
        }
        if(test_x + c > PLAYFIELD_WIDTH) {
          collide = 1;
        }*/
        if(playfield[test_y+r][test_x+c]) {
          collide = 1;
        }
      }
    }
  }
  return collide;
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
      gfx_SetTextXY(c*8, r*8);
      gfx_PrintInt(*(shape+(r*4)+c), 1);
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
      if(playfield[r][c]) {
        gfx_SetColor(playfield[r][c]);
        gfx_FillRectangle_NoClip(100 + 12*c, 12*r, 12, 12);
      }
    }
  }
}