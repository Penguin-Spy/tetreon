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

/* Function prototypes */
void PrintText(int8_t xpos, int8_t ypos, const char* text);
void DrawRect(uint24_t x, uint24_t y, uint24_t width, uint24_t height, uint8_t color);

/* tetrimino colors */
#define COLOR_I 31
#define COLOR_O 231
#define COLOR_T 248
#define COLOR_S 7
#define COLOR_Z 224
#define COLOR_J 228
#define COLOR_L 24

#define COLOR_BG 0x00
#define COLOR_TEXT 0xFF

int main(void) {
  /* Key variable */
  kb_key_t keys_dpad;
  kb_key_t keys_2nd;
  kb_key_t keys_alpha;
  kb_key_t keys_graph;
  const char* erase_string = "";

  gfx_Begin();
  gfx_SetDrawBuffer();

  gfx_FillScreen(COLOR_BG);
  gfx_SetTextFGColor(254);
  gfx_SetTextBGColor(COLOR_BG);

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

    gfx_FillRectangle(48, 0, 39, 8*8);

    /* Print the current arrow key input */
    gfx_PrintStringXY(keys_dpad  & kb_Down  ? "Down"  : erase_string, 48, 0);
    gfx_PrintStringXY(keys_dpad  & kb_Up    ? "Up"    : erase_string, 48, 1*8);
    gfx_PrintStringXY(keys_dpad  & kb_Left  ? "Left"  : erase_string, 48, 2*8);
    gfx_PrintStringXY(keys_dpad  & kb_Right ? "Right" : erase_string, 48, 3*8);
    gfx_PrintStringXY(keys_2nd   & kb_2nd   ? "A"     : erase_string, 48, 4*8);
    gfx_PrintStringXY(keys_alpha & kb_Alpha ? "B"     : erase_string, 48, 5*8);
    gfx_PrintStringXY(keys_2nd   & kb_Mode  ? "Pause" : erase_string, 48, 6*8);
    gfx_PrintStringXY(keys_graph & kb_GraphVar || keys_2nd & kb_Yequ ? "Hold" : erase_string, 48, 7*8);

    gfx_BlitBuffer();

  } while(kb_Data[6] != kb_Clear);

  gfx_End();

  return 0;
}

void DrawRect(uint24_t x, uint24_t y, uint24_t width, uint24_t height, uint8_t color) {
  gfx_SetColor(color);
  gfx_FillRectangle_NoClip(x*8, y*8, width*8, height*8);
}

/* Draw text on the homescreen at the given X/Y location */
void PrintText(int8_t xpos, int8_t ypos, const char* text) {
  os_SetCursorPos(ypos, xpos);
  os_PutStrFull(text);
}
