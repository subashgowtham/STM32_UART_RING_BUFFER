#include "delay.h"

volatile int msTicks = 0; // Use volatile to ensure correct operation in ISR

// Initialize SysTick for 1ms time base
void delayInit(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        // Handle SysTick configuration error
        while (1);
    }
}

// Millisecond delay function
void delay(int ms)
{
    msTicks = 0;                // Reset msTicks
    while (msTicks < ms);       // Wait until msTicks reaches ms
}

// SysTick interrupt handler
void SysTick_Handler(void)
{
    msTicks++;                  // Increment msTicks every 1ms
}
