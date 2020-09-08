#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "myLCD.h"

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
	
	int x=8205;
	printf("In lan 1");
	fprintf(&lcdstd,"www.hocavr.com");
	move_LCD(2,1);
	stdout=&lcdstd;
	printf("In lan 4: %i", x);
	home_LCD();
	printf(" Vu Duy Khanh");
	
	//UART: in ra uart --------------------
	stdout=&uartstd;
	printf("Hello world!\n");
	fprintf(&uartstd,"Dong nay duoc in bang ham \"fprintf\", %i\n", 5678); //su dung ham printf cua stdio
	printf("Hay nhap 1 phim de kiem tra ma ASCII\n");
	
	while(1){
		
	};
}
ISR(USART_RXC_vect){ //trinh phuc vu ngat USART
	//In ma ASCII cua phim duoc nhan
	char temp=UDR;
	int a=2000;
	
	fprintf(&uartstd,"Ma ASCII: %d\n", a);
}