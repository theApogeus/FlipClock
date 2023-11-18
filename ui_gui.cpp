#include "ui_gui.h"
#include "lvgl.h"


LV_FONT_DECLARE(font_Fliplo);
static lv_point_t line_points[] = {{4, 0}, {315, 0}};

static void update_text_subscriber_cb(lv_event_t *e);
static void timer_task(lv_timer_t *t);

void ui_begin() {
  // Create tree styles
  static lv_style_t style_frame;
  lv_style_init(&style_frame);
  lv_style_set_bg_color(&style_frame, UI_FRAME_COLOR);
  lv_style_set_border_color(&style_frame, UI_FRAME_COLOR);
  lv_style_set_border_width(&style_frame, 0);

  static lv_style_t style_text;
  lv_style_init(&style_text);
  lv_style_set_text_color(&style_text, UI_FONT_COLOR);

  static lv_style_t style_line;
  lv_style_init(&style_line);
  lv_style_set_line_color(&style_line, UI_BG_COLOR);
  lv_style_set_line_width(&style_line, 4);

  // Change the active screen's background color*/
  lv_obj_set_style_bg_color(lv_scr_act(), UI_BG_COLOR, LV_PART_MAIN);

  // Create objects frame
  lv_obj_t *hour_frame = lv_obj_create(lv_scr_act());
  lv_obj_add_style(hour_frame, &style_frame, 0);
  lv_obj_set_size(hour_frame, 120, 120);
  lv_obj_align(hour_frame, LV_ALIGN_CENTER, -72, 0);
  lv_obj_clear_flag(hour_frame, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *min_frame = lv_obj_create(lv_scr_act());
  lv_obj_add_style(min_frame, &style_frame, 0);
  lv_obj_set_size(min_frame, 120, 120);
  lv_obj_align(min_frame, LV_ALIGN_CENTER, 72, 0);
  lv_obj_clear_flag(min_frame, LV_OBJ_FLAG_SCROLLABLE);

  // Create objects text
  lv_obj_t *hour_text = lv_label_create(hour_frame);
  lv_obj_add_style(hour_text, &style_text, 0);
  lv_obj_center(hour_text);
  lv_obj_set_style_text_font(hour_text, &font_Fliplo, 0);
  lv_label_set_text(hour_text, "12");
  lv_obj_add_event_cb(hour_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_NEW_HOUR, hour_text, (void *)"%02d");

  lv_obj_t *min_text = lv_label_create(min_frame);
  lv_obj_add_style(min_text, &style_text, 0);
  lv_obj_center(min_text);
  lv_obj_set_style_text_font(min_text, &font_Fliplo, 0);
  lv_label_set_text(min_text, "34");
  lv_obj_add_event_cb(min_text, update_text_subscriber_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_NEW_MIN, min_text, (void *)"%02d");

  lv_obj_t *seg_text = lv_label_create(lv_scr_act());
  lv_obj_add_style(seg_text, &style_text, 0);
  lv_obj_align(seg_text, LV_ALIGN_CENTER, -2, -10);
  lv_obj_set_style_text_font(seg_text, &font_Fliplo, 0);
  lv_label_set_text(seg_text, ":");

  // Create object line
  lv_obj_t *line = lv_line_create(lv_scr_act());
  lv_line_set_points(line, line_points, 2);
  lv_obj_add_style(line, &style_line, 0);
  lv_obj_center(line);

  lv_timer_t *timer = lv_timer_create(timer_task, 500, seg_text);
}

static void timer_task(lv_timer_t *t) {
  lv_obj_t *seg = (lv_obj_t *)t->user_data;
  static bool j;
  if (j)
    lv_obj_add_flag(seg, LV_OBJ_FLAG_HIDDEN);
  else
    lv_obj_clear_flag(seg, LV_OBJ_FLAG_HIDDEN);
  j = !j;
}

static void update_text_subscriber_cb(lv_event_t *e) {
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);

  const char *fmt = (const char *)lv_msg_get_user_data(m);
  const int32_t *v = (const int32_t *)lv_msg_get_payload(m);

  lv_label_set_text_fmt(label, fmt, *v);
}