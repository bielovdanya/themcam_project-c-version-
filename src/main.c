//   Code(ver.1 , C , ESP-IDF Framework) for my ''thermal imaging camera'' project 
//   by Bielov Danya
//   Сode is based on the official ESP-IDF documentation:
// - ESP LCD: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd.html
// - MLX90640: based on the I2C protocol from the official Melexis datasheet
// - The 5x7 font is taken from the public ASCII font table (monodimensional 5x7 font)

//=============================================================================================================
//==================================/    THEMCAM_PROJECT    /==================================================
//=============================================================================================================

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "tft.h"
#include "mlx90640.h"
#include "draw.h"
#include "knopka.h"
#include "main.h"
#include <string.h>

static float frame[MLX_ROWS * MLX_COLS];
static float frame_last[MLX_ROWS * MLX_COLS];
static bool paused = false;

void app_main(void)
{
    i2c_init();
    button_init();

    esp_lcd_panel_handle_t panel;
    tft_init(&panel);
    mlx90640_init();

    while (1) {
        if (!paused) 
        {
            mlx90640_read_frame(frame);
            memcpy(frame_last, frame, sizeof(frame));
        }

        draw_frame(panel, frame_last);
        draw_min_max(panel, frame_last);

        if (button_get_state()) 
        {
            paused = !paused;
            vTaskDelay(pdMS_TO_TICKS(300));
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}