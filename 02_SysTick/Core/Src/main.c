#include "stm32f407xx.h"

volatile uint32_t ticks = 0;

// SysTick Interrupt Handler
void SysTick_Handler(void)
{
    ticks++;
}

// Check stm32f407_discovery.repl 16000000 change if different clock speed
// Initialize SysTick for 1ms interrupts
void SysTick_Init(void)
{
    SysTick->LOAD = (16000000U / 1000);
    SysTick->VAL = 0;
    SysTick->CTRL = (1U << 2) | (1U << 1) | (1U << 0);
}

// Get current milliseconds
uint32_t millis(void)
{
    return ticks;
}
// Delay in milliseconds
void delay_ms(uint32_t ms)
{
    uint32_t start = millis();
    while ((millis() - start) < ms);
}

// Initialize GPIO for LED
void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1U << 3);
    GPIOD->MODER |= (1U << 24);
}

int main(void)
{

    GPIO_Init();
    SysTick_Init();
    while (1)
    {
        GPIOD->ODR ^= (1U << 12);
        delay_ms(1000);
    }
}