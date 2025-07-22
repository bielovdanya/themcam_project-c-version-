#include "esp_lcd_panel_ops.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "main.h"

extern void draw_char(esp_lcd_panel_handle_t panel, int x, int y, char c, uint16_t color);
extern void draw_string(esp_lcd_panel_handle_t panel, int x, int y, const char *str, uint16_t color);

static uint32_t color_map(float temp)
{
    float norm = (temp - TEMP_MIN_VALID) / (TEMP_MAX_VALID - TEMP_MIN_VALID);
    norm = fmaxf(0.0f, fminf(1.0f, norm));
    uint8_t r = (uint8_t)(255 * norm);
    uint8_t g = (uint8_t)(255 * (1.0f - fabsf(norm - 0.5f) * 2));
    uint8_t b = (uint8_t)(255 * (1.0f - norm));
    return (r << 16) | (g << 8) | b;
}

static uint16_t rgb888_to_565(uint32_t rgb)
{
    return ((rgb >> 19) & 0x1F) << 11 |
           ((rgb >> 10) & 0x3F) << 5  |
           ((rgb >> 3) & 0x1F);
}

void draw_number(esp_lcd_panel_handle_t panel, int x, int y, float temp)
{
    char buf[8];
    snprintf(buf, sizeof(buf), "%.1f", temp);
    draw_string(panel, x, y, buf, 0xFFFF);
}

void draw_frame(esp_lcd_panel_handle_t panel, const float *frame)
{
    uint16_t buffer[PIXEL_SIZE_X * PIXEL_SIZE_Y];

    for (int y = 0; y < MLX_ROWS; y++) {
        for (int x = 0; x < MLX_COLS; x++) {
            float t = frame[y * MLX_COLS + x];
            uint32_t color = color_map(t);
            uint16_t rgb565 = rgb888_to_565(color);
            for (int i = 0; i < PIXEL_SIZE_X * PIXEL_SIZE_Y; i++) {
                buffer[i] = rgb565;
            }

            int px = x * PIXEL_SIZE_X;
            int py = y * PIXEL_SIZE_Y;
            esp_lcd_panel_draw_bitmap(panel, px, py, px + PIXEL_SIZE_X, py + PIXEL_SIZE_Y, buffer);
            draw_number(panel, px + 2, py + 2, t);
        }
    }
}

void draw_min_max(esp_lcd_panel_handle_t panel, const float *frame)
{
    float tmin = 999, tmax = -999;
    for (int i = 0; i < MLX_ROWS * MLX_COLS; i++) {
        if (frame[i] < tmin) tmin = frame[i];
        if (frame[i] > tmax) tmax = frame[i];
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "MIN: %.1fC", tmin);
    draw_string(panel, 5, 5, buf, 0xFFFF);
    snprintf(buf, sizeof(buf), "MAX: %.1fC", tmax);
    draw_string(panel, 5, 20, buf, 0xFFFF);
}

// 5x7 ASCII font
static const uint8_t font5x7[][5] = {
#include "font5x7.inc"
};

void draw_char(esp_lcd_panel_handle_t panel, int x, int y, char c, uint16_t color)
{
    if (c < 32 || c > 126) return;
    const uint8_t *glyph = font5x7[c - 32];
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 7; row++) {
            if (glyph[col] & (1 << row)) {
                esp_lcd_panel_draw_bitmap(panel, x + col, y + row, x + col + 1, y + row + 1, &color);
            }
        }
    }
}

void draw_string(esp_lcd_panel_handle_t panel, int x, int y, const char *str, uint16_t color)
{
    while (*str) {
        draw_char(panel, x, y, *str++, color);
        x += 6;
    }
}