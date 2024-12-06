#ifndef DELAY_H
#define DELAY_H

#include "stm32f10x.h" // Adjust as needed for your STM32 series

void delayInit(void);   // Initializes SysTick for delay
void delay(int ms);   // Millisecond delay function

#endif // DELAY_H