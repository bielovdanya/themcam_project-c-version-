#include "tft.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_ili9488.h"
#include "main.h"
#include "esp_log.h"

void tft_init(esp_lcd_panel_handle_t *panel)
{
    spi_bus_config_t buscfg = {
        .mosi_io_num = TFT_MOSI,
        .sclk_io_num = TFT_CLK,
        .miso_io_num = -1,
        .max_transfer_sz = 480 * 320 * 2
    };
    esp_err_t ret = spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE("TFT", "initialize failed");
        return;
    }
    esp_lcd_panel_io_handle_t io_handle;
    esp_lcd_panel_io_spi_config_t io_cfg = {
        .dc_gpio_num = TFT_DC,
        .cs_gpio_num = TFT_CS,
        .pclk_hz = 20 * 1000 * 1000,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8
    };
    ret = esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI_HOST, &io_cfg, &io_handle);
    if (ret != ESP_OK) {
        ESP_LOGE("TFT", "spi failed");
        return;
    }

    esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = TFT_RST,
        .rgb_endian = LCD_RGB_ENDIAN_RGB,
        .bits_per_pixel = 16,
    };
    size_t buffer_size = 0;
    ret = esp_lcd_new_panel_ili9488(io_handle, &panel_cfg, buffer_size, panel);
    if (ret != ESP_OK) {
        ESP_LOGE("TFT", "ili9488 failed");
        return;
    }

    esp_lcd_panel_reset(*panel);
    esp_lcd_panel_init(*panel);
    esp_lcd_panel_disp_on_off(*panel, true);
}
