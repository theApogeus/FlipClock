#pragma once

#define UI_BG_COLOR lv_color_black()
#define UI_FRAME_COLOR lv_palette_darken(LV_PALETTE_BLUE_GREY, 4)
#define UI_FONT_COLOR lv_color_white()
#define UI_COLOR_RED lv_palette_main(LV_PALETTE_RED)

#define MSG_NEW_HOUR 1
#define MSG_NEW_MIN 2

#define LV_DELAY(x)       \
  do                      \
  {                       \
    uint32_t t = x;       \
    while (t--)           \
    {                     \
      lv_timer_handler(); \
      delay(1);           \
    }                     \
  } while (0);

void ui_begin();
