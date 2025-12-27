#include "stm32f407xx.h"

void GPIO_Init(void);
void TIM4_PWM_Init(void);
void delay_ms(uint32_t ms);

int main(void)
{
    GPIO_Init();
    TIM4_PWM_Init();

    int duty = 0;
    int step = 10;

    while (1)
    {
        TIM4->CCR1 = duty;
        duty += step;
        if (duty >= 1000 || duty <= 0) step = -step;
        delay_ms(10); 
    }
}

void TIM4_PWM_Init(void)
{
    RCC->APB1ENR |= (1U << 2);
    TIM4->PSC = 160 - 1;
    TIM4->ARR = 1000 - 1;
    TIM4->CCMR1 |= (1U << 6) | (1U << 5);
    TIM4->CCMR1 |= (1U << 3);
    TIM4->CCER |= (1U << 0);
    TIM4->CR1 |= (1U << 0);
}

void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1U << 3);
    GPIOD->MODER &= ~(3U << 24);
    GPIOD->MODER |= (2U << 24);
    GPIOD->AFR[1] |= (2U << 16); 
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 4000; i++) { __asm("nop"); }
}