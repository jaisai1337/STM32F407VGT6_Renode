#include "stm32f407xx.h"

void GPIO_Init(void);
void EXTI_Init(void);

// Simple delay function
int delay(volatile uint32_t count){
    while (count--)
        __NOP();
    return 0;
}
// Interrupt handler for EXTI line 0
void EXTI0_IRQHandler(void){
    if (EXTI->PR & (1U << 0)) 
    {
        GPIOD->ODR ^= (1U << 12);
        EXTI->PR |= (1U << 0);
    }
}
// Initialize GPIO for LED and Button
void GPIO_Init(void){
    RCC->AHB1ENR |= (1U << 0) | (1U << 3);
    GPIOD->MODER |= (1U << 24);
    GPIOA->PUPDR &= ~(3U << 0);
}
// Initialize External Interrupt for Button
void EXTI_Init(void){
    RCC->APB2ENR |= (1U << 14);
    SYSCFG->EXTICR[0] &= ~(0xF << 0); 
    EXTI->IMR |= (1U << 0);
    EXTI->RTSR |= (1U << 0); 
    EXTI->FTSR &= ~(1U << 0);
    NVIC_EnableIRQ(EXTI0_IRQn);
}
int main(void){    
    GPIO_Init();
    EXTI_Init();
    while (1){
        // Main loop does nothing, waiting for interrupts
    }
}