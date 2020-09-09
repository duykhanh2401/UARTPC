#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "myLCD.h"

unsigned char usart_getchar()
{
	while(!(UCSRA&(1<<RXC)));
	return UDR;
}
//chuong trinh con phat ky tu uart (ham co ban)
void uart_char_tx(unsigned char chr){
	if (chr == '\n')
	uart_char_tx('\r');
	while (bit_is_clear(UCSRA,UDRE)) {}; //cho den khi bit UDRE=1
	UDR=chr;
}
//tao 2 "FILE" (stream) tuong ung voi LCD va UART
static FILE lcdstd = FDEV_SETUP_STREAM(putChar_LCD,  NULL,_FDEV_SETUP_WRITE);
static FILE uartstd= FDEV_SETUP_STREAM(uart_char_tx, NULL,_FDEV_SETUP_WRITE);

int main(void){
	
	UBRRH=0;
	UBRRL=51;
	UCSRA=0x00;
	UCSRC=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	sei();
	
	
	//LCD: khoi dong va in LCD-------------
	init_LCD();
	clr_LCD();
	

	
	//UART: in ra uart --------------------

	fprintf(&uartstd,"Ket noi thanh cong !\n");

	
	while(1){
		fprintf(&uartstd," Test !");
		_delay_ms(2000);
		
	};
}
ISR(USART_RXC_vect){ //trinh phuc vu ngat USART
	//In ma ASCII cua phim duoc nhan
	char a=usart_getchar();
	fprintf(&uartstd,"%c\n", a);
}