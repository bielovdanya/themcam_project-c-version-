#include "mlx90640.h"
#include "driver/i2c.h"
#include <string.h>
#include "main.h"

void i2c_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA_PIN,
        .scl_io_num = I2C_SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ
    };
    i2c_param_config(I2C_PORT, &conf);
    i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
}

void mlx90640_init(void) {}

void mlx90640_read_frame(float *framebuf)
{
    uint8_t raw[834];
    memset(raw, 0, sizeof(raw));

    for (int i = 0; i < 2; i++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        uint16_t addr = 0x2400 + i * 416;
        i2c_master_write_byte(cmd, (MLX90640_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, addr >> 8, true);
        i2c_master_write_byte(cmd, addr & 0xFF, true);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (MLX90640_I2C_ADDR << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmd, raw + i * 416, 416, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(100));
        i2c_cmd_link_delete(cmd);
    }

    for (int i = 0; i < 768; i++) {
        framebuf[i] = (float)((raw[i * 2] << 8 | raw[i * 2 + 1]) & 0x7FFF) * 0.01f;
    }
}