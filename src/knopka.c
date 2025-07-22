#include "knopka.h"
#include "driver/gpio.h"
#include "main.h"

void button_init(void)
{
    gpio_config_t io = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io);
}

bool button_get_state(void)
{
    return gpio_get_level(BUTTON_GPIO) == 0;
}