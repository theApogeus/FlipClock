#include <lvgl.h>
#include <M5Core2.h>
#include <WiFi.h>
#include "time.h"

#include "config.h"
#include "ui_gui.h"

static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t buf1[(320 * 240) / 10];

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(2);

  M5.Lcd.printf("\nConnecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.print(".");
    delay(100);
  }
  M5.Lcd.println("\nCONNECTED!");
  configTime(GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC, NTP_SERVER);
  printLocalTime();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(500);

  lv_disp_draw_buf_init(&draw_buf, buf1, NULL, (320 * 240) / 10);
  lv_init();

  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = m5_lcd_flush;
  disp_drv.draw_buf = &draw_buf;
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  lv_disp_drv_register(&disp_drv);

  ui_begin();
}

void loop() {
  M5.update();
  lv_timer_handler();
  delay(3);
  static uint32_t last_tick;
  if (millis() - last_tick > 100) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      lv_msg_send(MSG_NEW_HOUR, &timeinfo.tm_hour);
      lv_msg_send(MSG_NEW_MIN, &timeinfo.tm_min);
    }
    last_tick = millis();
  }
  if (M5.BtnB.isPressed()) {
    standby_mode();
    delay(20);
  }
}

static void standby_mode() {
  static bool j;
  if (j)
    M5.Lcd.sleep();
  else
    M5.Lcd.wakeup();
  j = !j;
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    M5.Lcd.println("No time available (yet)");
    return;
  }
  M5.Lcd.println("");
  M5.Lcd.println(&timeinfo, "%A, %B %d \n%Y %H:%M:%S");
}

void m5_lcd_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  M5.Lcd.startWrite();
  M5.Lcd.setAddrWindow(area->x1, area->y1, w, h);
  M5.Lcd.pushColors((uint16_t *)&color_p->full, w * h, true);
  M5.Lcd.endWrite();

  lv_disp_flush_ready(disp_drv);
}
