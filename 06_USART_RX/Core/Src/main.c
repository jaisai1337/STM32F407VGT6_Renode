#include "stm32f407xx.h"
#include <stdint.h>

void GPIO_Init(void);
void USART2_Init(void);
void USART2_Write(int ch);
char USART2_Read(void);

int main(void)
{
    GPIO_Init();
    USART2_Init();
    char ch;
    USART2_Write('S');
    USART2_Write('t');
    USART2_Write('a');
    USART2_Write('r');
    USART2_Write('t');
    USART2_Write('\n');

    while (1)
    {
        ch = USART2_Read();
        USART2_Write(ch);
    }
}

void USART2_Init(void)
{
    RCC->APB1ENR |= (1U << 17);

    USART2->BRR = 0x008B;
    USART2->CR1 = (1U << 13) | (1U << 3) | (1U << 2);
}

void USART2_Write(int ch)
{
    while (!(USART2->SR & (1U << 7))) {}
    USART2->DR = (ch & 0xFF);
}

char USART2_Read(void)
{
    while (!(USART2->SR & (1U << 5))) {}
    return USART2->DR;
}

void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1U << 0);
    GPIOA->MODER &= ~((3U << 4) | (3U << 6));
    GPIOA->MODER |=  ((2U << 4) | (2U << 6));
    GPIOA->AFR[0] |= (7U << 8);
    GPIOA->AFR[0] |= (7U << 12);
}