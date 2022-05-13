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

#include "srs.h"

/* tetrimino colors */
#define COLOR_I 31
#define COLOR_O 231
#define COLOR_T 248
#define COLOR_S 7
#define COLOR_Z 224
#define COLOR_J 24
#define COLOR_L 228

uint8_t tetrimino_colors[7] ={
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

enum tetrimino { I, O, T, S, Z, J, L, None }; // none is for the hold slot, which is empty by default
enum rotation { Up, Left, Down, Right };


int main(void) {
  /* Key variable */
  kb_key_t keys_dpad;
  kb_key_t keys_2nd;
  kb_key_t keys_alpha;
  kb_key_t keys_graph;
  const char* erase_string = "";

  /* game state variables */
  uint8_t playfield[20][10] ={
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,0,1,1,1,0},
    {0,1,1,1,0,0,0,1,0,0},
    {0,1,0,1,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
  };
  uint8_t hand_x = 0, hand_y = 0;
  enum tetrimino hand_tetrimino = J;
  enum rotation  hand_rotation = Up;
  enum tetrimino hold_tetrimino = None;

  double* p;
  double balance[10];

  p = balance;

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

    keys_dpad  = kb_Data[7];
    keys_2nd   = kb_Data[1];
    keys_alpha = kb_Data[2];
    keys_graph = kb_Data[3];

    gfx_FillRectangle(228, 0, 40, 8*8);

    /* Print the current arrow key input */
    gfx_PrintStringXY(keys_dpad  & kb_Down  ? "Down"  : erase_string, 228, 0);
    gfx_PrintStringXY(keys_dpad  & kb_Up    ? "Up"    : erase_string, 228, 1*8);
    gfx_PrintStringXY(keys_dpad  & kb_Left  ? "Left"  : erase_string, 228, 2*8);
    gfx_PrintStringXY(keys_dpad  & kb_Right ? "Right" : erase_string, 228, 3*8);
    gfx_PrintStringXY(keys_2nd   & kb_2nd   ? "A"     : erase_string, 228, 4*8);
    gfx_PrintStringXY(keys_alpha & kb_Alpha ? "B"     : erase_string, 228, 5*8);
    gfx_PrintStringXY(keys_2nd   & kb_Mode  ? "Pause" : erase_string, 228, 6*8);
    gfx_PrintStringXY(keys_graph & kb_GraphVar || keys_2nd & kb_Yequ ? "Hold" : erase_string, 228, 7*8);

    DrawPlayfield(playfield);

    DrawTetrimino(hand_tetrimino, hand_rotation, hand_x, hand_y);

    gfx_BlitBuffer();

    if(hand_y < 20) {
      hand_y++;
    } else {
      hand_y = 0;
      if(hand_x < 10) {
        hand_x++;
      } else {
        hand_x = 0;
        if(hand_rotation < 3) {
          hand_rotation++;
        } else {
          hand_rotation = 0;
          if(hand_tetrimino < 6) {
            hand_tetrimino++;
          } else {
            hand_tetrimino = 0;
          }
        }
      }
    }

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

  /*switch(hand_tetrimino) {
    case J:
      gfx_SetColor(COLOR_J);
      // 4*4*4*hand_tetrimino + 4*4*hand_rotation


      //rotations = Jrotations;
      /*default:
        gfx_PrintStringXY("invalid tetrimino: ", 0, 232);
        gfx_PrintInt(tetrimino, 1);
        gfx_PrintString(", ");
        gfx_PrintInt(rotation, 1);*/
        //}

  for(r = 0; r < 4*4; r+=4) {
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
