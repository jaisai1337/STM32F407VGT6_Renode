#include "stm32f407xx.h"
#include <stdint.h>

#define BUFFER_SIZE 64

// --- Ring Buffer Variables ---
volatile uint8_t rx_buffer[BUFFER_SIZE];
volatile uint16_t rx_head = 0;
volatile uint16_t rx_tail = 0;

// --- Function Prototypes ---
void GPIO_Init(void);
void USART2_Init(void);
void USART2_Write(int ch);
int USART2_Read(void);
int IsDataAvailable(void);

void USART2_IRQHandler(void)
{
    if (USART2->SR & (1U << 5))
    {
        uint8_t data = USART2->DR;
        uint16_t next_head = (rx_head + 1) % BUFFER_SIZE;
        if (next_head != rx_tail)
        {
            rx_buffer[rx_head] = data;
            rx_head = next_head;
        }
    }
}

int main(void)
{
    GPIO_Init();
    USART2_Init();
    
    USART2_Write('I');
    USART2_Write('R');
    USART2_Write('Q');
    USART2_Write('\n');

    while (1)
    {
        
        if (IsDataAvailable())
        {
            int c = USART2_Read();
            USART2_Write(c);
        }
        GPIOD->ODR ^= (1U << 12) ; // Toggle LED on PD12
        for (volatile int i = 0; i < 100000; i++);

        
    }
}

int IsDataAvailable(void)
{
    return (uint16_t)(BUFFER_SIZE + rx_head - rx_tail) % BUFFER_SIZE;
}

int USART2_Read(void)
{
    if (rx_head == rx_tail) return -1;
    uint8_t value = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % BUFFER_SIZE;
    return value;
}

void USART2_Init(void)
{
    RCC->APB1ENR |= (1U << 17);
    USART2->BRR = 0x008B;
    USART2->CR1 |= (1U << 5);
    NVIC_EnableIRQ(USART2_IRQn);
    USART2->CR1 |= (1U << 3) | (1U << 2) | (1U << 13);
}

void USART2_Write(int ch)
{
    while (!(USART2->SR & (1U << 7))) {}
    USART2->DR = (ch & 0xFF);
}

void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1U << 0) | (1U << 3);
    GPIOA->MODER &= ~((3U << 4) | (3U << 6));
    GPIOA->MODER |=  ((2U << 4) | (2U << 6));
    GPIOA->AFR[0] |= (7U << 8) | (7U << 12);

    // GPIOD for LEDs
    GPIOD->MODER &= ~(3U << 24);
    GPIOD->MODER |= (1U << 24);
}