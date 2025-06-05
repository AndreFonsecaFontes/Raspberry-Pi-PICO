#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "display_lcd.h"
#include "hardware/adc.h"

//////////////////////////////////////////////////////////////////////////////

#define OUT_H 1
#define OUT_L 0
#define OUT 1
#define IN 0

#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0

#define DELAY 100000 // 1/Fs (in microseconds)

#define GPIO_TENSAO 26
#define ADC_TENSAO 0

//////////////////////////////////////////////////////////////////////////////

uint16_t contador = 0;

uint8_t var = 0;

const uint8_t sensor[3] = {7,8,9};
const uint16_t valor_pot_up [16] = {255,510,765,1020,1275,1530,1785,2040,2295,2550,2805,3060,3315,3570,3825,4080};
uint8_t qnt = 0;
uint8_t litros = 0;

uint8_t barra [8] = {
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111
};

//////////////////////////////////////////////////////////////////////////////

void system_init(void);
void alarm_irq() ;
void button_callback(uint gpio);
void mostraLcdGas ();

//////////////////////////////////////////////////////////////////////////////


void alarm_irq(void) 
{
  static bool toggle = false;
  static bool toggle_2 = false;


  printf("Timer interrupt occurred!\n");
  // Clear the alarm irq
  hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);

  uint64_t target = timer_hw->timerawl + DELAY;

  // Write the lower 32 bits of the target time to the alarm register, arming it.
  timer_hw->alarm[ALARM_NUM] = (uint32_t) target;
  
  contador++;

}

//////////////////////////////////////////////////////////////////////////////

int main() {
  stdio_init_all();
  system_init();
  cria_caractere_lcd(0,barra);
  sleep_ms (5);


  while (true) 
  {
//////////////////////////////////////////////////////////////////////////////

  mostraLcdGas();

//////////////////////////////////////////////////////////////////////////////

    sleep_ms(5);
  
  }

  return 0;

  }




void system_init(void)
{ 
  uint64_t target = 0;

  for(uint8_t i = 0; i<8; i++)
  {
    gpio_init(pino_ch[i]);
    gpio_set_dir(pino_ch[i], IN);
    gpio_pull_up(pino_ch[i]);
  }

    for(uint8_t i = 0; i<6; i++)
  {
    gpio_init(leds_pin[i]);
    gpio_set_dir(leds_pin[i], OUT);
    gpio_put(leds_pin[i], OUT_L);
  }


  // Enable the interrupt for the alarm (we're using Alarm 0)
  hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM) ;
  // Associate an interrupt handler with the ALARM_IRQ
  irq_set_exclusive_handler(ALARM_IRQ, alarm_irq) ;
  // Enable the alarm interrupt
  irq_set_enabled(ALARM_IRQ, true) ;
  
  target = timer_hw->timerawl + DELAY;

  // Write the lower 32 bits of the target time to the alarm register, arming it.
  timer_hw->alarm[ALARM_NUM] = (uint32_t) target;

  //Set the interruption
  
  init_lcd();
  adc_init();
  adc_gpio_init(GPIO_TENSAO);
  

  sleep_ms(5);

 
}

//////////////////////////////////////////////////////////////////////////////

void mostraLcdGas (void)
{
  uint16_t sensor_comb = 0;

  adc_select_input(ADC_TENSAO);
  adc_read();
  sensor_comb = adc_read();

  litros = (72*sensor_comb)/4095;


  for(uint8_t i =0; i<16; i++)
  {
    if(sensor_comb >= valor_pot_up[i])
    {
      posicao_cursor_lcd(1, i);
      escreve_caractere_lcd(0); 
    }
  }

  if(litros != qnt)
  {
    LIMPA_DISPLAY();
    qnt = litros;
  }

  posicao_cursor_lcd(2, 0);
  escreve_inteiro_lcd(litros);
  escreve_frase_lcd("L");

  DesligaCursor();
}