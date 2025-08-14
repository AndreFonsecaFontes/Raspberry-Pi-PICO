#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "display_lcd.h"

/////////////////////////////////////////////////////////////////////////////////////////

#define OUT_H 1
#define OUT_L 0
#define OUT 1
#define IN 0

#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0

#define DELAY 100000 // 1/Fs (in microseconds)

uint16_t contador = 0;

unsigned char nome_1 [17] = "xxxxxxx xxxxxxx ";
uint8_t nome_2 [17] = "xxxxxxx xxxxxxx ";
uint8_t nome_3 [17] = "Andre Fonseca ";
unsigned char RA_1 [17] = "xxxxxxx";
uint8_t RA_2 [17] = "xxxxxxx";
uint8_t RA_3 [17] = "xxxxxxx";



/////////////////////////////////////////////////////////////////////////////////////////

void system_init(void);
void alarm_irq() ;

/////////////////////////////////////////////////////////////////////////////////////////


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

int main() {
  stdio_init_all();
  system_init();
  sleep_ms (5);


  while (true) {

    if(contador == 0)
    {
      posicao_cursor_lcd(1,1);
      escreve_frase_ram_lcd(nome_1);
      posicao_cursor_lcd(2,0);
      escreve_frase_ram_lcd(RA_1);
      DesligaCursor();
    }
    else if (contador == 30)
    {
      posicao_cursor_lcd(1,1);
      escreve_frase_ram_lcd(nome_2);
      posicao_cursor_lcd(2,0);
      escreve_frase_ram_lcd(RA_2);
      DesligaCursor();
    }
      else if (contador == 60)
    {
      posicao_cursor_lcd(1,1);
      escreve_frase_ram_lcd(nome_3); 
      posicao_cursor_lcd(2,0);
      escreve_frase_ram_lcd(RA_3);
      DesligaCursor();
    }
    else if (contador == 90)
    {
      contador = 0;
    }
    sleep_ms(5);

  }
}

void system_init(void)
{ 
  uint64_t target = 0;

  // Enable the interrupt for the alarm (we're using Alarm 0)
  hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM) ;
  // Associate an interrupt handler with the ALARM_IRQ
  irq_set_exclusive_handler(ALARM_IRQ, alarm_irq) ;
  // Enable the alarm interrupt
  irq_set_enabled(ALARM_IRQ, true) ;
  
  target = timer_hw->timerawl + DELAY;

  // Write the lower 32 bits of the target time to the alarm register, arming it.
  timer_hw->alarm[ALARM_NUM] = (uint32_t) target;

  init_lcd();

  
}
