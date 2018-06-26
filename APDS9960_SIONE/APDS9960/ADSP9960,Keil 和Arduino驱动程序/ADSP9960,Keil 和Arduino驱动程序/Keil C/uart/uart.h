#ifndef __UART_H
#define __UART_H

//#define	 printfs  uart_puts

void USART_INIT(void);
void Uart_Putc(unsigned char c);
void Uart_Puts(char *s );
void uart_puts(char *s );
void Uart_String_CRLF(char *s );

#endif
