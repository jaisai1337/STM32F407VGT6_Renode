#include "stm32f407xx.h"
#include <stdint.h>

void GPIO_Init(void);
void USART2_Init(void);
void USART2_Write(int ch);
void delay(uint32_t ms);

void print(char *str)
{
    while (*str)
    {
        USART2_Write(*str++);
    }
}

int main(void)
{
    GPIO_Init();
    USART2_Init();

    while (1)
    {
        print("Hello from STM32F407 Bare Metal!\r\n");
        delay(10000);
    }
}

void USART2_Init(void)
{
    RCC->APB1ENR |= (1U << 17);
    USART2->BRR = 0x008B;
    USART2->CR1 |= (1U << 3);
    USART2->CR1 |= (1U << 13);
}

void USART2_Write(int ch)
{
    while (!(USART2->SR & (1U << 7))) {}
    USART2->DR = (ch & 0xFF);
}

void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1U << 0);
    GPIOA->MODER &= ~(3U << 4);
    GPIOA->MODER |= (2U << 4);
    GPIOA->AFR[0] |= (7U << 8);
}

// Simple delay
void delay(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 4000; i++) { __asm("nop"); }
}