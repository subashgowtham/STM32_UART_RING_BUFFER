#include "stm32f10x.h"
#include "uart_ringbuffer.h"

#define BUFFER_SIZE 64 													                 // Define the size of the circular buffer

static uint8_t buffer[BUFFER_SIZE];													     // Create buffer

static CircularBuffer txBuffer = {													     // Circular buffer instance
  .buffer = buffer,
  .head = 0,
  .tail = 0,
  .size = sizeof(buffer)
};

//==================UART Intitialization=======================
void UART1_Init(void) {
  RCC -> APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;   // Enable clocks for GPIOA and USART1
	
	// Configure PA9 (TX) as alternate function push-pull
  GPIOA -> CRH &= ~GPIO_CRH_CNF9;															   // Clear CNF9 bits
  GPIOA -> CRH |= GPIO_CRH_CNF9_1;														   // Set CNF9[1] for alternate function
  GPIOA -> CRH |= GPIO_CRH_MODE9;															   // Set MODE9[1:0] for 50 MHz output

  USART1 -> BRR = 0x1D4C;                                        // Configure USART1: 9600 baud
  USART1 -> CR1 = USART_CR1_TE | USART_CR1_UE | USART_CR1_TXEIE; // Enable TX, UART, and TXE interrupt

  NVIC_EnableIRQ(USART1_IRQn);                                   // Enable USART1 interrupt in NVIC
  NVIC_SetPriority(USART1_IRQn, 1);                              // Optional: Set priority
}

//==================Add data to ring buffer====================

uint8_t ring_buffer_put(CircularBuffer * cb, char data) {
  uint16_t nextHead = (cb -> head + 1) % BUFFER_SIZE;            // Calculate the next head position
  if (nextHead == cb -> tail) return 0;                          // Buffer full
  cb -> buffer[cb -> head] = data;                               // Write data to the current head position
  cb -> head = nextHead;                                         // Update the head position
  return 1;
}

//==================Retrieve data ring buffer====================

uint8_t ring_buffer_get(CircularBuffer * cb, uint8_t * data) {
  if (cb -> head == cb -> tail) return 0;                        // Buffer empty
  * data = cb -> buffer[cb -> tail];													   // Read data from the current tail position
  cb -> tail = (cb -> tail + 1) % BUFFER_SIZE;                   // Update the tail position
  return 1;
}

//=======================Transmit data============================
void UART_Transmit(char * string) {
  for (int i = 0; string[i] != '\0'; i++)                        // Loop until the null terminator is reached
	{
    while (!ring_buffer_put( & txBuffer, string[i])) {           // Wait until data is added to the buffer successfully 
      // Optionally handle buffer full condition here
    }
  }
  USART1 -> CR1 |= USART_CR1_TXEIE;						                   // Ensure the TXE interrupt is enabled
}

//=================USART1 Interrupt Handler========================
void USART1_IRQHandler(void) {
  if (USART1 -> SR & USART_SR_TXE) {                             // Check if the transmit data register is empty
    uint8_t data;
    if (ring_buffer_get( & txBuffer, & data)) {                  // Read data from the buffer
      USART1 -> DR = data;                                       // Send the next byte
    } 
		else {
      USART1 -> CR1 &= ~USART_CR1_TXEIE;                         // Disable TXE interrupt when no more data to send
    }
  }
}