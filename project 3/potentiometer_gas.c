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

#define DELAY 500000 // 1/Fs (in microseconds)

#define GPIO_TENSAO 26
#define ADC_TENSAO 0

//////////////////////////////////////////////////////////////////////////////

uint16_t contador = 0;

uint8_t var = 0;
uint8_t var_2 = 0;
uint8_t var_3 = 0;

unsigned char nome_1 [17] = "Romano Bibiano ";
uint8_t nome_2 [17] = "Gabriel Baldo ";
uint8_t nome_3 [17] = "Andre Fonseca ";
unsigned char RA_1 [17] = "1600732221002";
uint8_t RA_2 [17] = "1600732413028";
uint8_t RA_3 [17] = "1600732413001";

uint16_t pino_ch [8] = {16,15,14,13,12,11,10,9};
uint8_t estado [8] = {0,0,0,0,0,0,0,0};
uint16_t pos_cursor [8] = {0,2,4,6,8,10,12,14};
uint16_t num_pinos [8] = {1,2,3,4,5,6,7,8};

uint16_t leds_pin [6] = {17,18,19,20,21,22};

uint8_t tensao = 0;

//////////////////////////////////////////////////////////////////////////////

void system_init(void);
void alarm_irq() ;
void button_callback(uint gpio);
void nameOnLcd();
void escolherLeds();
void acenderLeds();
void potPorcent();

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
  
  //Leds Pisca Pisca

  if(estado[0] == 1)
  {
    gpio_put(leds_pin[0], toggle);
    toggle = !toggle;
  }
  else
  {
    gpio_put(leds_pin[0], OUT_L);
  }


  if(estado[1] == 1)
  {
    gpio_put(leds_pin[1], toggle_2);
    toggle_2 = !toggle_2;
  }
  else
  {
    gpio_put(leds_pin[1], OUT_L);
  }

//////////////////////////////////////////////////////////////////////////////

}

int main() {
  stdio_init_all();
  system_init();
  sleep_ms (5);


  while (true) 
  {
//////////////////////////////////////////////////////////////////////////////

    nameOnLcd();
    escolherLeds();
    acenderLeds();
    potPorcent();

//////////////////////////////////////////////////////////////////////////////

    sleep_ms(1);
  
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
  adc_select_input(ADC_TENSAO);

  sleep_ms(5);

 
}

//////////////////////////////////////////////////////////////////////////////

void nameOnLcd (void)
{
    if(contador == 0 && var  == 0)
  {
    posicao_cursor_lcd(1,1);
    escreve_frase_ram_lcd(nome_1);
    posicao_cursor_lcd(2,0);
    escreve_frase_ram_lcd(RA_1);
    DesligaCursor();

  }
    else if (contador == 6 && var  == 0 )
  {
    posicao_cursor_lcd(1,1);
    escreve_frase_ram_lcd(nome_2);
    posicao_cursor_lcd(2,0);
    escreve_frase_ram_lcd(RA_2);
    DesligaCursor();
  }
    else if (contador == 12 && var  == 0 )
  {
    posicao_cursor_lcd(1,1);
    escreve_frase_ram_lcd(nome_3); 
    posicao_cursor_lcd(2,0);
    escreve_frase_ram_lcd(RA_3);
    DesligaCursor();
  }
    else if (contador == 18 && var  == 0)
  {
    LIMPA_DISPLAY();
    var = 1;
  }
}

//////////////////////////////////////////////////////////////////////////////

void escolherLeds (void)
{
  if( var == 1)
  {
    for(uint8_t i=0; i<8; i++)
    {
      estado[i] = gpio_get(pino_ch[i]);
      
    }
    for(uint8_t i = 0; i<8; i++)
    {
      posicao_cursor_lcd(1,pos_cursor[i]);
      escreve_inteiro_lcd(estado[i]);
    }
  
    DesligaCursor();
  }
}

//////////////////////////////////////////////////////////////////////////////

void acenderLeds (void)
{
  if((estado[2] && estado[3]) == 1)
  {
    gpio_put(leds_pin[2], OUT_H);
  }
  else
  {
    gpio_put(leds_pin[2], OUT_L);
  }

  if(estado[4] == 1)
  {
    gpio_put(leds_pin[3], OUT_L);
  }
  else
  {
    gpio_put(leds_pin[3], OUT_H);
  }

  if(estado[5] ==0 && estado[6] == 0 && estado[7] == 0)
  {
    gpio_put(leds_pin[4], OUT_H);
  }
  else 
  {
    gpio_put(leds_pin[4], OUT_L);
  }
}

//////////////////////////////////////////////////////////////////////////////

void potPorcent (void)
{
  if(var == 1)
  {

    adc_read();
    uint16_t adc_reading = adc_read();
    uint16_t porcent = (100 * adc_reading) / 4095;

    //For pocent < 10
    if(porcent >= 10 && var_3 == 0)
    {
      var_3 = 1;
    }    
    else if(porcent <10 && var_3 == 1)
    {
      if(contador)
      {
        LIMPA_DISPLAY();
        var_3 = 0;
      }
    }

    //For pocent == 100
    if (porcent == 100 && var_2 == 0)
    {
      var_2 = 1;
    }
    else if(porcent < 100 && var_2 == 1)
    { 
      if(contador)
      {
        LIMPA_DISPLAY();
        var_2 = 0;
      }

    }


    posicao_cursor_lcd(2,0);
    escreve_inteiro_lcd(porcent);
  }
}