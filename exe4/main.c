#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

const int BTN_PIN_G = 26;
const int LED_PIN_G = 6;

int volatile f_btn_r_fall = 0;
int volatile f_btn_g_rise = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4 && gpio == BTN_PIN_R) { // fall edge
    f_btn_r_fall = 1;
  }
  else if (events == 0x8 && gpio == BTN_PIN_G) { // fall edge
    f_btn_g_rise = 1;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);
  
  gpio_init(LED_PIN_R);
  gpio_set_dir(LED_PIN_R, GPIO_OUT);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);
  
  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);


  // callback led r (first)
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
    &btn_callback);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

  int led_status_r = 0;
  int led_status_g = 0;

  while (true) {
    if (f_btn_r_fall) {
      printf("fall red\n");
      led_status_r = !led_status_r;
      gpio_put(LED_PIN_R, led_status_r);
      f_btn_r_fall = 0;
    }
    else if (f_btn_g_rise) {
      printf("rise green\n");
      led_status_g = !led_status_g;
      gpio_put(LED_PIN_G, led_status_g);
      f_btn_g_rise = 0;
    }
  }
}
