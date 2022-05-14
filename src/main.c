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

#include "tetriminos.h"

/* tetrimino colors */
#define COLOR_I 31
#define COLOR_O 231
#define COLOR_T 248
#define COLOR_S 7
#define COLOR_Z 224
#define COLOR_J 24
#define COLOR_L 228

uint8_t tetrimino_colors[7] = {
  COLOR_I,
  COLOR_O,
  COLOR_T,
  COLOR_S,
  COLOR_Z,
  COLOR_J,
  COLOR_L
};

#define COLOR_BG 0x00
#define COLOR_SIDE 74
#define COLOR_TEXT 0xFF

/* Function prototypes */
void PrintText(int8_t xpos, int8_t ypos, const char* text);
void DrawRect(uint24_t x, uint24_t y, uint24_t width, uint24_t height, uint8_t color);
void DrawTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, uint8_t x, uint8_t y);
void DrawPlayfield(uint8_t playfield[20][10]);

#define ctrl_Up    (1<<0)
#define ctrl_Down  (1<<1)
#define ctrl_Left  (1<<2)
#define ctrl_Right (1<<3)
#define ctrl_RotL  (1<<4)
#define ctrl_RotR  (1<<5)
#define ctrl_Hold  (1<<6)
#define ctrl_Pause (1<<7)


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
  uint8_t hand_x = 0, hand_y = 0;
  enum tetrimino hand_tetrimino = J;
  enum rotation  hand_rotation = Up;
  enum tetrimino hold_tetrimino = None;

  gfx_Begin();
  gfx_SetDrawBuffer();

  gfx_FillScreen(COLOR_SIDE);
  gfx_SetTextFGColor(254);
  gfx_SetTextBGColor(COLOR_BG);

  gfx_SetColor(COLOR_BG);
  gfx_FillRectangle(100, 0, 120, 240);

  DrawRect(1, 1, 4, 1, COLOR_I);
  DrawRect(1, 3, 2, 2, COLOR_O);
  DrawRect(2, 6, 1, 1, COLOR_T); DrawRect(1, 7, 3, 1, COLOR_T);
  DrawRect(2, 9, 2, 1, COLOR_S); DrawRect(1, 10, 2, 1, COLOR_S);
  DrawRect(1, 12, 2, 1, COLOR_Z); DrawRect(2, 13, 2, 1, COLOR_Z);
  DrawRect(1, 15, 1, 1, COLOR_J); DrawRect(1, 16, 3, 1, COLOR_J);
  DrawRect(3, 18, 1, 1, COLOR_L); DrawRect(1, 19, 3, 1, COLOR_L);
  gfx_SetColor(COLOR_BG);

  do {
    /* Update kb_Data */
    kb_Scan();

    /*if(keys_dpad & kb_Up     && !(controls_prev && ctrl_Up))    controls |= ctrl_Up;
    if(keys_dpad & kb_Down   && !(controls_prev && ctrl_Down))  controls |= ctrl_Down;
    if(keys_dpad & kb_Left   && !(controls_prev && ctrl_Left))  controls |= ctrl_Left;
    if(keys_dpad & kb_Right  && !(controls_prev && ctrl_Right)) controls |= ctrl_Right;
    if(keys_2nd   & kb_2nd   && !(controls_prev && ctrl_RotL))  controls |= ctrl_RotL;
    if(keys_alpha & kb_Alpha && !(controls_prev && ctrl_RotR))  controls |= ctrl_RotR;
    if(keys_2nd   & kb_Mode  && !(controls_prev && ctrl_Pause)) controls |= ctrl_Pause;
    if((keys_graph & kb_GraphVar || keys_2nd & kb_Yequ) && !(controls_prev && ctrl_Hold)) controls |= ctrl_Hold;*/

    controls_cur = 0;
    controls_cur |= kb_Data[7] & kb_Up ? ctrl_Up : 0;
    controls_cur |= kb_Data[7] & kb_Down ? ctrl_Down : 0;
    controls_cur |= kb_Data[7] & kb_Left ? ctrl_Left : 0;
    controls_cur |= kb_Data[7] & kb_Right ? ctrl_Right : 0;
    controls_cur |= kb_Data[1] & kb_2nd ? ctrl_RotL : 0;
    controls_cur |= kb_Data[2] & kb_Alpha ? ctrl_RotR : 0;
    controls_cur |= kb_Data[1] & kb_Mode ? ctrl_Pause : 0;
    controls_cur |= kb_Data[3] & kb_GraphVar || kb_Data[1] & kb_Yequ ? ctrl_Hold : 0;

    //if(controls_cur & ctrl_Up && !(controls_prev & ctrl_Up)) controls |= ctrl_Up;
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

      hand_y++;
    }
    if(controls & ctrl_Down) {
      //TODO: soft drop

      hand_y--;
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


    //hand_rotation = (hand_rotation > 3 || hand_rotation < 0) ? 0 : hand_rotation;

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

    gfx_BlitBuffer();

  } while(kb_Data[6] != kb_Clear);

  gfx_End();

  return 0;
}

void DrawRect(uint24_t x, uint24_t y, uint24_t width, uint24_t height, uint8_t color) {
  gfx_SetColor(color);
  gfx_FillRectangle_NoClip(x*12, y*12, width*12, height*12);
}

/* x, y are the position in the playfield of the top-left block of the tetrimino */
void DrawTetrimino(enum tetrimino hand_tetrimino, enum rotation hand_rotation, uint8_t x, uint8_t y) {
  uint8_t r, c;
  uint8_t* shape = rotations;
  shape += 4*4*4*hand_tetrimino + 4*4*hand_rotation;

  gfx_SetColor(tetrimino_colors[hand_tetrimino]);

  for(r = 0; r < 4*4; r += 4) {
    for(c = 0; c < 4; c++) {
      gfx_SetTextXY(c*8, r*2);
      gfx_PrintInt(*(shape+r+c), 1);
      if(*(shape+r+c) == 1) {
        gfx_FillRectangle(100 + 12*c + 12*x, 3*r + 12*y, 12, 12);
      }
    }
  }
}

void DrawPlayfield(uint8_t playfield[20][10]) {
  uint8_t r, c;

  gfx_SetColor(COLOR_BG);
  gfx_FillRectangle(100, 0, 120, 240);
  gfx_SetColor(COLOR_Z);

  for(r = 0; r < 20; r++) {
    for(c = 0; c < 10; c++) {
      if(playfield[r][c] == 1) {
        gfx_FillRectangle_NoClip(100 + 12*c, 12*r, 12, 12);
      }
    }
  }
}

/* Draw text on the homescreen at the given X/Y location */
void PrintText(int8_t xpos, int8_t ypos, const char* text) {
  os_SetCursorPos(ypos, xpos);
  os_PutStrFull(text);
}
