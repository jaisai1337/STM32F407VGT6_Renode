#include "stm32f407xx.h"

int delay(volatile uint32_t count)
{
    while (count--)
        __NOP();
    return 0;
}

int main(void)
{    
    // Enable GPIOD
    RCC->AHB1ENR |= (1U << 3);
    // PD12-15 output
    GPIOD->MODER &= ~(3U << 24) | (3U << 26) | (3U << 28) | (3U << 30);
    GPIOD->MODER |=  (1U << 24) | (1U << 26) | (1U << 28) | (1U << 30);
    while (1)
    {
        GPIOD->ODR ^= (1U << 12) | (1U << 13) | (1U << 14) | (1U << 15);
        delay(10000000);
    }
}