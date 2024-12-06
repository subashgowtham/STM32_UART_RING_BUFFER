#ifndef UART_RINGBUFFER_H
#define UART_RINGBUFFER_H

#include "stm32f10x.h"

typedef struct {
    uint8_t *buffer;
	  uint8_t  size;
    volatile uint16_t head;
    volatile uint16_t tail;
} CircularBuffer;


void UART1_Init(void);
uint8_t CircularBuffer_Write(CircularBuffer* cb, char data);
uint8_t CircularBuffer_Read(CircularBuffer* cb, uint8_t* data);
void UART_Transmit(char *string);

#endif