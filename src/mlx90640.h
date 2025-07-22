#pragma once

void i2c_init(void);
void mlx90640_init(void);
void mlx90640_read_frame(float *framebuf);