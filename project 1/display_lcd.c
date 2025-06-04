/******************************************************************************
 * Nome do Arquivo 	: display.c
 *
 * Descri��o       	: Implementa controle do display HD44780 (16 x 2)
 *
 * Ambiente			: MPLAB, XC8 vers�o 1.45, PIC18F4620
 *
 * Respons�vel		: Torres, Weslley
 *
 * Vers�o/Data		: v01.00 - 08/09/2014 - Vers�o inicial
 *					  v01.01 - 22/09/2019 - Corrigido fun��o EscreveFloatLCD. 
 *					  v01.02 - 16/08/2020 - Ajustado de acordo com padr�o de 
 *											codifica��o
 *
 *****************************************************************************/


/******************************************************************************
* HEADER-FILES (Somente os arquivos necess�rios nesse arquivo)
******************************************************************************/
#include "display_lcd.h"
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include <math.h>
/*****************************************************************************/
/******************************************************************************
* Variaveis Globais
******************************************************************************/


/*****************************************************************************/

/******************************************************************************
* Prototipos das fun��es
******************************************************************************/ 

/*****************************************************************************/

/******************************************************************************
 * Funcao:		void delay(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Implementa atraso baseado no cilco de instru��o do uC
 *****************************************************************************/
void delay(void)
{
	unsigned char contador_ext, contador_int;

	for(contador_ext = 0; contador_ext<100; contador_ext++)
	{
        //NOP();		
		for(contador_int = 0; contador_int<50; contador_int++)
		{
            //NOP();
		}
	}
}
/******************************************************************************
 * Funcao:		void init_lcd(void)
 * Entrada:		Nenhuma (void)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Configura portas do PIC 18F4550 que est�o conectadas ao 
 *	display LCD como saida. Essa rotina � baseada no controlador de LCD 
 *  Hitachi HD44780. Configura os parametros de comunica��o com o display,
 *  tais como:
 *	- modo de opera��o (4 ou 8 data bits)
 *	- tipo de cursor
 *	- tamanho da fonte
 *	Ao final da configura��o limpa o display.
 *****************************************************************************/
void init_lcd(void)
{
	// Configura os pinos de controle (RW,RS,E) como sa�da.
	gpio_init(DISPLAY_RW);
	gpio_set_dir(DISPLAY_RW, OUT);
	gpio_put(DISPLAY_RW, 0);
	
	gpio_init(DISPLAY_RS);
	gpio_set_dir(DISPLAY_RS, OUT);
	gpio_put(DISPLAY_RS, 0);	// comando=0, dados=1
	
	gpio_init(DISPLAY_E);
	gpio_set_dir(DISPLAY_E, OUT);
	gpio_put(DISPLAY_E, 0);	// desabilita=0, habilita=1	
		
	// Configura os pinos de dados (D4,D5,D6,D7) como sa�da.
	/* Pino D4 do display 16x2 */
	gpio_init(DISPLAY_D4);
	gpio_set_dir(DISPLAY_D4, OUT);
	gpio_put(DISPLAY_D4, 1);
	
	/* Pino D5 do display 16x2 */
	gpio_init(DISPLAY_D5);
	gpio_set_dir(DISPLAY_D5, OUT);
	gpio_put(DISPLAY_D5, 1);
	
	/* Pino D6 do display 16x2 */
	gpio_init(DISPLAY_D6);
	gpio_set_dir(DISPLAY_D6, OUT);
	gpio_put(DISPLAY_D6, 0);
	
	/* Pino D7 do display 16x2 */
	gpio_init(DISPLAY_D7);
	gpio_set_dir(DISPLAY_D7, OUT);
	gpio_put(DISPLAY_D7, 0);	

//Passo 1: Envio do comando de inicializa��o 0b0011

	// Delay inicial de 15ms ap�s alimentar o LCD: Vdd > 4,5V
	sleep_ms(15);

	//Habilita leitura do comando pelo LCD, atrav�s do pino de enable, para tanto,
	//um pulso de 1ms com borda de descida � efetuado
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);

//Passo 2: Envio do comando de inicializa��o 0b0011

	// Delay de pelo menos 4.1ms
	sleep_ms(5);

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);

//Passo 3: Envio do comando de inicializa��o 0b0011

	sleep_ms(5);

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);

//Passo 4: Envio da Configura��o 0b0010 no Nibble-High
	gpio_put(DISPLAY_D7, 0); 
	gpio_put(DISPLAY_D6, 0); 
	gpio_put(DISPLAY_D5, 1); 
	gpio_put(DISPLAY_D4, 0); 


	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);


//Envio dos comandos de Configura��o do LCD
	escreve_cmd_lcd(0b00101000);	// Function Set: 4bit interface, 2 lines, Font 5x7  
	escreve_cmd_lcd(0b00000110);	// Entry mode set: Increment, Shift OFF
	escreve_cmd_lcd(0b00001110);	// Display Control: Display ON, Cursor ON, Blink OFF
	escreve_cmd_lcd(0b00000001);	// Clear display
}


/******************************************************************************
 * Funcao:		void escreve_cmd_lcd(unsigned char comando)
 * Entrada:		comando: comando que ser� enviado para o LCD
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve um comando para o display de LCD.
 *****************************************************************************/
void escreve_cmd_lcd(unsigned char comando)
{
	//Seleciona pinos de Controle para escrita do comando
	gpio_put(DISPLAY_RW, 0);
	gpio_put(DISPLAY_RS, 0);
	gpio_put(DISPLAY_E, 0);


	//Seleciona pinos de Dados: Nibble-High
	if(comando&0b00010000){	gpio_put(DISPLAY_D4, 1);}
	else {gpio_put(DISPLAY_D4, 0);}
	if(comando&0b00100000){gpio_put(DISPLAY_D5, 1);}
	else {gpio_put(DISPLAY_D5, 0);}
	if(comando&0b01000000){gpio_put(DISPLAY_D6, 1);}
	else {gpio_put(DISPLAY_D6, 0);}
	if(comando&0b10000000){gpio_put(DISPLAY_D7, 1);}
	else {gpio_put(DISPLAY_D7, 0);}

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);

	//Seleciona pinos de Dados: Nibble-Low
	if(comando&0b00000001){gpio_put(DISPLAY_D4, 1);}
	else {gpio_put(DISPLAY_D4, 0);}
	if(comando&0b00000010){gpio_put(DISPLAY_D5, 1);}
	else {gpio_put(DISPLAY_D5, 0);}
	if(comando&0b00000100){gpio_put(DISPLAY_D6, 1);}
	else {gpio_put(DISPLAY_D6, 0);}
	if(comando&0b00001000){gpio_put(DISPLAY_D7, 1);}
	else {gpio_put(DISPLAY_D7, 0);}    

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);
	
	//Como os comandos Clear Display(0b00000001) e Cursor Home(0b0000001x),
	//demoram mais tempo para serem executados,cerca de cerca de 1,5ms.
	//Garante-se um atraso maior caso os mesmos sejam efetuados
	if (comando==0x01 || comando==0x02 || comando==0x03)
	{ delay(); }
}


/******************************************************************************
 * Funcao:		void endereco_cursor_lcd(unsigned char endereco)
 * Entrada:		endereco: endere�o da memoria de dados do display que se 
 *				colocar o cursor
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina posiciona o cursor no endere�o desejado da memoria 
 *				RAM do display LCD. Os comandos de escrita seguintes v�o 
 *				come�ar a partir da posi��o escolhida por esta fun��o
 *****************************************************************************/
void endereco_cursor_lcd(unsigned char endereco)
{
	//Seleciona pinos de Controle para escrita do comando
	gpio_put(DISPLAY_RW, 0);
	gpio_put(DISPLAY_RS, 0);
	gpio_put(DISPLAY_E, 0);

	//Seleciona pinos de Dados: Nibble-High
	gpio_put(DISPLAY_D7, 1);	
	if(endereco&0b00010000){gpio_put(DISPLAY_D4, 1);}
	else {gpio_put(DISPLAY_D4, 0);}
	if(endereco&0b00100000){gpio_put(DISPLAY_D5, 1);}
	else {gpio_put(DISPLAY_D5, 0);}
	if(endereco&0b01000000){gpio_put(DISPLAY_D6, 1);}
	else {gpio_put(DISPLAY_D6, 0);}
	
	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);

	//Seleciona pinos de Dados: Nibble-Low
	if(endereco&0b00000001){gpio_put(DISPLAY_D4, 1);}
	else {gpio_put(DISPLAY_D4, 0);}
	if(endereco&0b00000010){gpio_put(DISPLAY_D5, 1);}
	else {gpio_put(DISPLAY_D5, 0);}
	if(endereco&0b00000100){gpio_put(DISPLAY_D6, 1);}
	else {gpio_put(DISPLAY_D6, 0);}
	if(endereco&0b00001000){gpio_put(DISPLAY_D7, 1);}
	else {gpio_put(DISPLAY_D7, 0);}

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);
}


/******************************************************************************
 * Funcao:		void posicao_cursor_lcd(unsigned char linha, unsigned char 
 *				coluna)
 * Entrada:		linha: n�mero de 1 � 4 da linha em que se deseja deixar o 
 *				cursor
 *				coluna: n�mero de 1 � 16 coluna linha em que se deseja deixar 
 *				o cursor
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina posiciona o cursor na tela do display, na linha e 
 *				coluna desejados. Os comandos de escrita seguintes v�o come�ar
 *  			a partir da posi��o escolhida por esta fun��o
 *****************************************************************************/
void posicao_cursor_lcd(unsigned char linha, unsigned char coluna)
{
	unsigned char endereco = 0x00;
	
	if(coluna <= NUM_COLUNAS)
	{
		//Calcula o endere�o da mem�ria RAM do display que deve ser usado
		//para posicionar o cursor na linha e coluna desejados
		switch (linha)
		{
			case 1:
				endereco = LINHA_UM + coluna;
				endereco_cursor_lcd(endereco);		
			break;
		
			case 2:
				endereco = LINHA_DOIS + coluna;
				endereco_cursor_lcd(endereco);
			break;
			
			case 3:
				endereco = LINHA_TRES + coluna;
				endereco_cursor_lcd(endereco);		
			break;
		
			case 4:
				endereco = LINHA_QUATRO + coluna;
				endereco_cursor_lcd(endereco);
			break;
			
			default:
				endereco = LINHA_UM;
			break;
		}
        
        coluna++;
	}
	else
	{
		endereco = LINHA_UM;
	}
}


/******************************************************************************
 * Funcao:		void escreve_caractere_lcd(unsigned char data)
 * Entrada:		data: Byte de dados para ser escrito na memoria RAM do LCD
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve o byte de caracteres no display a partir da posi��o
 *				de memoria atual do cursor ou da posi��o de mem�ria	escolhida 
 *				com as fun��es posicao_cursor_lcd() ou endereco_cursor_lcd().
 *****************************************************************************/
void escreve_caractere_lcd(char data)
{
	//Seleciona pinos de Controle para escrita do comando
	gpio_put(DISPLAY_RW, 0);
	gpio_put(DISPLAY_RS, 1);
	gpio_put(DISPLAY_E, 0);
	
	//Seleciona pinos de Dados: Nibble-High
	if(data&0b00010000){gpio_put(DISPLAY_D4, 1);}
	else {gpio_put(DISPLAY_D4, 0);}
	if(data&0b00100000){gpio_put(DISPLAY_D5, 1);}
	else {gpio_put(DISPLAY_D5, 0);}
	if(data&0b01000000){gpio_put(DISPLAY_D6, 1);}
	else {gpio_put(DISPLAY_D6, 0);}
	if(data&0b10000000){gpio_put(DISPLAY_D7, 1);}
	else {gpio_put(DISPLAY_D7, 0);}

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);


	//Seleciona pinos de Dados: Nibble-Low
	if(data&0b00000001){gpio_put(DISPLAY_D4, 1);}
	else {gpio_put(DISPLAY_D4, 0);}
	if(data&0b00000010){gpio_put(DISPLAY_D5, 1);}
	else {gpio_put(DISPLAY_D5, 0);}
	if(data&0b00000100){gpio_put(DISPLAY_D6, 1);}
	else {gpio_put(DISPLAY_D6, 0);}
	if(data&0b00001000){gpio_put(DISPLAY_D7, 1);}
	else {gpio_put(DISPLAY_D7, 0);}    

	//Pulso no pino de Enable para habilitar leitura do LCD na borda de descida
	gpio_put(DISPLAY_E, 1);      			
    sleep_ms(2);
	gpio_put(DISPLAY_E, 0); 
	sleep_ms(2);
}


/******************************************************************************
 * Funcao:		void escreve_inteiro_lcd(int valor)
 * Entrada:		valor: numero inteiro com sinal (de -32768 a 32767) a ser 
 * 				escrito no display.
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina escreve o numero inteiro no display a partir da 
 *				posi��o de memoria atual do cursor ou da posi��o de mem�ria
 *				escolhida com as fun��es posicao_cursor_lcd() ou 
 *				endereco_cursor_lcd().
 *****************************************************************************/
void escreve_inteiro_lcd(int valor)
{
	char texto[7] = "000000";
    sprintf(texto, "%d", valor);
	escreve_frase_ram_lcd(texto);
}

// Reverses a string 'str' of length 'len' 
void reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    } 
} 
 
// Converts a given integer x to string str[]. 
// d is the number of digits required in the output. 
// If d is more than the number of digits in x, 
// then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
 
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
 
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 
 
// Converts a floating-point/double number to a string. 
void ftoa(float n, char* res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
 
    // Extract floating part 
    float fpart = n - (float)ipart; 
 
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
 
    // check for display option after point 
    if (afterpoint != 0) { 
        res[i] = '.'; // add dot 
 
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter 
        // is needed to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
 
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

/******************************************************************************
 * Funcao:		void escreve_float_lcd(float valor, char formatacao)
 * Entrada:		valor: numero float com sinal (de -9999.9999 a 9999.9999) que
 *				ser� escrito no display
 *				formatacao: numero de casas decimais desejadas ap�s a 
 *				virgula (de 0 � 4 casas decimais).
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve o numero float no display a partir da posi��o atual 
 *				do cursor com quantas casas decimais forem definidas no 
 *				par�metro formatacao.
 *****************************************************************************/
void escreve_float_lcd(float valor, char formatacao)
{

	char i = 0;         
	char res[20]; 								// Vetor para armazenar o float convertido
  ftoa(valor, res, formatacao);	// Converte float para texto
	
	while(i<=formatacao)
	{
				escreve_caractere_lcd(res[i]);
				i++;
	}
}

/******************************************************************************
 * Funcao:		void escreve_frase_ram_lcd(char *buffer)
 * Entrada:		*buffer: apontador para string na memoria de dados(RAM)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina copia uma string terminada com caracter nulo
 *				da memoria de dados do microcontrolador para o display de LCD.
 *				A string apontada por *buffer � copiada no display a partir da
 *				posi��o de memoria atual do cursor ou da posi��o de mem�ria
 *  			escolhida com as fun��es posicao_cursor_lcd() ou 
 *   			endereco_cursor_lcd().
 *****************************************************************************/
void escreve_frase_ram_lcd(char *buffer)
{
	while(*buffer)		// escreve dados no LCD ate achar caracter nulo
	{
		escreve_caractere_lcd(*buffer); // escreve caracter no LCD
		buffer++;		// incrementa apontador
	}
}


/******************************************************************************
 * Funcao:		void escreve_frase_lcd(const char *buffer)
 * Entrada:		*buffer: apontador para string na memoria de programa(ROM)
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Copia uma string terminada com caracter nulo da memoria de 
 *				programa do microcontrolador para o display de LCD.
 *				A string apontada por *buffer � copiada no display a partir da
 *				posi��o de memoria atual do cursor ou da posi��o de mem�ria 
 *				escolhida com as fun��es posicao_cursor_lcd() ou 
 *				endereco_cursor_lcd().
 *****************************************************************************/
void escreve_frase_lcd(const char *buffer)
{
	while(*buffer)		// escreve dados no LCD ate achar caracter nulo
	{
		escreve_caractere_lcd(*buffer); // escreve caracter no LCD
		buffer++;		// incrementa apontador
	}
}


/******************************************************************************
 * Funcao:		void cria_caractere_lcd(unsigned char endereco, const char *buffer)
 * Entrada:		endereco: Numero de 0 � 7 que indica a posi��o da CGRAM que o 
 *				caractere ser� criado.
 *
 *				*buffer: apontador para o vetor de char com tamanho 8 que 
 *				guarda cada linha de desenho do caractere que ser� criado.
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:   Essa rotina cria um caractere de usu�rio em uma das 7 posi��es
 *				disponiveis na CGRAM do display. O caractere criado tem tamanho 
 *				8x5 pixel (8 linhas e 5 colunas), portanto cada valor do vetor 
 *				buffer representa uma linha, onde os 3 bits mais significativos
 *				de cada valor � desprezado.
 *****************************************************************************/
void cria_caractere_lcd(unsigned char endereco, const char *buffer)
{
	unsigned char comando;		//guarda comando a ser enviado para LCD
	unsigned char contador = 0;	//contador do comando for

	//Dependendo do valor de endereco, seleciona o comando correto
	//Garante que enderco s� tenha valores de 0 � 7
	endereco = endereco & 0b00000111;
	
	switch (endereco)
	{	case 0: comando = 0b01000000;	break;
		case 1: comando = 0b01001000;	break;
		case 2: comando = 0b01010000;	break;
		case 3: comando = 0b01011000;	break;
		case 4: comando = 0b01100000;	break;
		case 5: comando = 0b01101000;	break;
		case 6: comando = 0b01110000;	break;
		case 7: comando = 0b01111000;	break;
		default:comando = 0b01000000;	break;
	}	
	//Comando de escrita na CGRAM (cria��o de caratere personalizado)
	escreve_cmd_lcd(comando);
	
	//Escreve cada linha do caractere com a sequencia de valores do buffer
	for(contador=0; contador<8; contador++)
	{
		escreve_caractere_lcd(*buffer);	// escreve caracter no LCD
	    buffer++;	// incrementa apontador
	}
	
	//Comando para sair do modo cria��o de caratere e posicionar cursor
	//na linha 1, coluna 1 da tela.
	endereco_cursor_lcd(0x00);	
}

/******************************************************************************
 * Funcao:		void itoa(unsigned int val, char* str )
 * Entrada:		val: 
 *
 *				*str: 
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:   Essa rotina .
 *****************************************************************************/
void converte_inteiro_matriz(unsigned int val, char* str )
{
  str[0]=(val/10000)+0x30;  
  str[1]=((val%10000)/1000)+0x30;  
  str[2]=((val%1000)/100)+0x30;  
  str[3]=((val%100)/10)+0x30;
  str[4]=(val%10)+0x30;
  str[5]=0;
}