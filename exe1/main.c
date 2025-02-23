#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;

int volatile f_btn_rise = 0;
int volatile f_btn_fall = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    f_btn_fall = 1;
  } else if (events == 0x8) { // rise edge
    f_btn_rise = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (f_btn_fall == 1) {
      f_btn_fall = 0;
      printf("fall\n");
    }
    else if (f_btn_rise == 1) {
      f_btn_rise = 0;
      printf("rise\n");
    }
  }
}
