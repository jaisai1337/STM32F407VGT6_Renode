#include "stm32f407xx.h"

void GPIO_Init(void);
void TIM2_Init(void);

// Timer2 interrupt handler
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & (1U << 0)) 
    {
        GPIOD->ODR ^= (1U << 12);
        TIM2->SR &= ~(1U << 0);
    }
}

int main(void)
{
    GPIO_Init();
    TIM2_Init();
    while (1)
    {
    }
}
// Timer2 initialization function
void TIM2_Init(void)
{
    RCC->APB1ENR |= (1U << 0);
    TIM2->PSC = 16000 - 1;
    TIM2->ARR = 1000 - 1; 
    TIM2->DIER |= (1U << 0);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM2->CR1 |= (1U << 0);
}
// GPIO initialization function
void GPIO_Init(void)
{
    RCC->AHB1ENR |= (1U << 3);
    GPIOD->MODER |= (1U << 24); 
}