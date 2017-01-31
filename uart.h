#include <stdio.h>

void uart_init();
int uart_putchar(char c, FILE * stream);
int uart_getchar(FILE * stream);

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
