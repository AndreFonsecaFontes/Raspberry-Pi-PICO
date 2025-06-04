/******************************************************************************
 * Nome do Arquivo 	: displayLCD.h
 *
 * Descrição       	: Interface implementacao do display 16x2
 *
 * Ambiente			    : WOKWI e RP2040 C/C++ SDK
 *
 * Responsável		  : Weslley M. Torres
 *
 * Versão/Data		  : 1.0.0 - 10/03/2024 - Initial version 
 *
 *****************************************************************************/
#ifndef __DISPLAYLCD_H
#define __DISPLAYLCD_H


/** I N C L U D E S **********************************************************/

/** D E F I N E S ************************************************************/

// Define dos pinos de DADOS do LCD ligados no RP2040
#define DISPLAY_D4	2
#define DISPLAY_D5	3
#define DISPLAY_D6	4
#define DISPLAY_D7	5

// Define dos pinos de CONTROLE do LCD ligados no RP2040
#define DISPLAY_RS	7
#define DISPLAY_E	8
#define DISPLAY_RW	6

#define OUT 1
#define IN 0


//endere�o de inicio de cada linha
#define LINHA_UM		0x00U	
#define	LINHA_DOIS		0x40U
#define LINHA_TRES		0x14U
#define	LINHA_QUATRO	0x54U

#define NUM_COLUNAS		0x10U

/*** D E F I N E S   D E   C O N F I G U R A C A O ***************************/

/******************************************************************************
 * Define:		LimpaDisplay()
 * Descri��o: 	Remove todos os caracteres do display e retorna a posi��o do
 *				cursor para a linha 1 e coluna 1
 *****************************************************************************/
#define LIMPA_DISPLAY()  	escreve_cmd_lcd(0b00000001)


/******************************************************************************
 * Define:		CursorHome()
 * Descri��o: 	Retorna a posi��o do cursor para a linha 1 e coluna 1, mas
 *				sem remover todos os caracteres do display.
 *****************************************************************************/
#define CursorHome()  	escreve_cmd_lcd(0b00000010)


/******************************************************************************
 * Define:		LigaDisplay()
 * Descri��o: 	Liga a tela do display e desliga o cursor do visor.
 *****************************************************************************/
#define LigaDisplay()  		escreve_cmd_lcd(0b00001100)


/******************************************************************************
 * Define:		DesligaDisplay()
 * Descri��o: 	Desliga a tela do display, mas n�o remove os caracteres da  
 *				mem�ria, nem apaga o backlight.
 *****************************************************************************/
#define DesligaDisplay()  	escreve_cmd_lcd(0b00001000)


/******************************************************************************
 * Define:		LigaCursor()
 * Descri��o: 	Faz com que o cursor apare�a na tela, e se a mesma estiver
 *				desligada, ser� ligada.
 *****************************************************************************/
#define LigaCursor()		escreve_cmd_lcd(0b00001110)


/******************************************************************************
 * Define:		DesligaCursor()
 * Descri��o: 	Faz com o cursor suma da tela e se a mesma estiver
 *				desligada, ser� ligada.
 *****************************************************************************/
#define DesligaCursor()		escreve_cmd_lcd(0b00001100)


/******************************************************************************
 * Define:		PiscaCursor()
 * Descri��o: 	Faz com que o cursor apare�a no LCD e fique piscando. Se a 
 *				tela estiver desligada, ser� ligada.
 *****************************************************************************/
#define PiscaCursor()		escreve_cmd_lcd(0b00001111)


/******************************************************************************
 * Define:		DeslocaCursorDir()
 * Descri��o: 	Faz com que o cursor desloque na tela uma posi��o � direita
 *****************************************************************************/
#define DeslocaCursorDir()		escreve_cmd_lcd(0b00010100)


/******************************************************************************
 * Define:		DeslocaCursorEsq()
 * Descri��o: 	Faz com que o cursor desloque na tela uma posi��o � esquerda
 *****************************************************************************/
#define DeslocaCursorEsq()		escreve_cmd_lcd(0b00010000)


/******************************************************************************
 * Define:		DeslocaDisplayDir()	
 * Descri��o: 	Faz com que a tela desloque uma posi��o � direita
 *****************************************************************************/
#define DeslocaDisplayDir()		escreve_cmd_lcd(0b00011100)


/******************************************************************************
 * Define:		DeslocaDisplayEsq()	
 * Descri��o: 	Faz com que a tela desloque uma posi��o � esquerda
 *
 *****************************************************************************/
#define DeslocaDisplayEsq()		escreve_cmd_lcd(0b00011000)


/**  P R O T O T I P O  D E  F U N C O E S  **********************************/

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
extern void init_lcd(void);


/******************************************************************************
 * Funcao:		void escreve_cmd_lcd(unsigned char cmd)
 * Entrada:		comando: comando que ser� enviado para o LCD
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve um comando para o display de LCD.
 *****************************************************************************/
extern void escreve_cmd_lcd(unsigned char comando);


/******************************************************************************
 * Funcao:		void endereco_cursor_lcd(unsigned char endereco)
 * Entrada:		endereco: endere�o da memoria de dados do display que se 
 *				colocar o cursor
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Essa rotina posiciona o cursor no endere�o desejado da memoria 
 *				RAM do display LCD. Os comandos de escrita seguintes v�o 
 *				come�ar a partir da posi��o escolhida por esta fun��o
 *****************************************************************************/
extern void endereco_cursor_lcd(unsigned char endereco);


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
extern void posicao_cursor_lcd(unsigned char linha, unsigned char coluna);


/******************************************************************************
 * Funcao:		void escreve_caractere_lcd(unsigned char data)
 * Entrada:		data: Byte de dados para ser escrito na memoria RAM do LCD
 * Sa�da:		Nenhuma (void)
 * Descri��o:	Escreve o byte de caracteres no display a partir da posi��o
 *				de memoria atual do cursor ou da posi��o de mem�ria	escolhida 
 *				com as fun��es posicao_cursor_lcd() ou endereco_cursor_lcd().
 *****************************************************************************/
extern void escreve_caractere_lcd(char data);


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
extern void escreve_inteiro_lcd(int valor);


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
extern void escreve_float_lcd(float valor, char formatacao);


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
extern void escreve_frase_ram_lcd(char *buffer);


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
extern void escreve_frase_lcd(const char *buffer);


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
extern void cria_caractere_lcd(unsigned char endereco, const char *buffer);
/******************************************************************************
 * Funcao:		void converte_inteiro_matriz(unsigned int val, char* str )
 * Entrada:		val: 
 *
 *				*str: 
 *
 * Sa�da:		Nenhuma (void)
 * Descri��o:   Essa rotina .
 *****************************************************************************/
 void converte_inteiro_matriz(unsigned int val, char* str );
#endif
