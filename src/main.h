#pragma once

#define I2C_SDA_PIN         40
#define I2C_SCL_PIN         39
#define I2C_PORT            I2C_NUM_0
#define I2C_FREQ_HZ         400000
#define MLX90640_I2C_ADDR   0x33

#define SPI_HOST            SPI2_HOST
#define TFT_MOSI            18
#define TFT_CLK             19
#define TFT_CS              10
#define TFT_DC              16
#define TFT_RST             17

#define TFT_WIDTH           480
#define TFT_HEIGHT          320
#define TFT_HOFFSET         0
#define TFT_VOFFSET         0

#define BUTTON_GPIO         38

#define MLX_ROWS            24
#define MLX_COLS            32
#define PIXEL_SIZE_X        (TFT_WIDTH / MLX_COLS)
#define PIXEL_SIZE_Y        (TFT_HEIGHT / MLX_ROWS)

#define TEMP_MIN_VALID      -40.0f
#define TEMP_MAX_VALID      300.0f