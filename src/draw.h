#pragma once
#include "esp_lcd_panel_ops.h"

void draw_frame(esp_lcd_panel_handle_t panel, const float *frame);
void draw_min_max(esp_lcd_panel_handle_t panel, const float *frame);