#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

int volatile f_btn_r_fall = 0;
int volatile f_btn_g_fall = 0;


void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4 && gpio == BTN_PIN_R) { // fall edge
    f_btn_r_fall = 1;
  }
  else if (events == 0x4 && gpio == BTN_PIN_G) { // fall edge
    f_btn_g_fall = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                     &btn_callback);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

  while (true) {
    if (f_btn_r_fall) {
      printf("fall red\n");
      f_btn_r_fall = 0;
    }
    if (f_btn_g_fall) {
      printf("fall green\n");
      f_btn_g_fall = 0;
    }
  }
}
