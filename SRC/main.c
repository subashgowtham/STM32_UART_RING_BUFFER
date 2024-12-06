#include "stm32f10x.h"
#include "delay.h"
#include "uart_ringbuffer.h"



int main(void) 
{
    UART1_Init();
    delayInit();

    char message[] = "This is a UART ring buffer\n";
	
    while (1) {
        UART_Transmit(message);
        delay(500); 
    }
}
