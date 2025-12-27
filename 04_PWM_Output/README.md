# STM32F407 Timer Interrupt (TIM2)

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Simulation](https://img.shields.io/badge/simulation-Renode-orange.svg)

## 📖 Overview

This project demonstrates how to use **Hardware Timer 2 (TIM2)** to generate periodic interrupts. 
Unlike the previous `delay` method which paused the CPU, this method allows the CPU to run other code in the `while(1)` loop while the Timer hardware handles the time-keeping in the background.

**Key Feature:** The LED blinks at **1 Hz** (1 second) driven entirely by hardware interrupts.

## 📂 Project Structure

```text
03_Timer_Interrupt
├── Core
│   └── Src
│       └── main.c        # TIM2 configuration and ISR
├── renode_board
│   └── stm32f407vgt6.resc # Simulation script
├── makefile
└── README.md
```

## ⚙️ The Math (1Hz Frequency)

We need to slow down the 16 MHz system clock to 1 Hz.

$$ \text{Timer Frequency} = \frac{\text{System Clock}}{(\text{Prescaler} + 1) \times (\text{AutoReload} + 1)} $$

### 1. Prescaler (PSC): Divide 16 MHz down to 1 kHz (1ms).

* $\frac{16,000,000}{16,000} = 1,000 \text{ Hz}$

* Register Value: ```16000 - 1```

### 2. Auto-Reload (ARR): Count 1000 of those 1ms ticks to get 1 second.

* $1,000 \text{ ms} = 1 \text{ second}$
Register Value: ```1000 - 1```


## 🚀 How to Test in Renode
### 1. **Start the Simulation**

```bash
renode renode_board/stm32f407vgt6.resc
```
### 2. **Verify Behavior**
* The Green LED (PD12) will toggle state every 1 second (Virtual Time).

* If the simulation runs slow, check the elapsed command to verify the Virtual Time is advancing correctly relative to the LED toggles.

### ⚠️ Important Note on Renode
Ensure your .repl file defines TIM2 with the correct frequency to match your C code:
```text
timer2: Timers.STM32_Timer @ sysbus 0x40000000
    -> nvic@28
    frequency: 16000000 // 16 MHz
    initialLimit: 0xFFFFFFFF
```



## 🛠 Build & Run

### Prerequisites

* ```arm-none-eabi-gcc```
* ```make```
* ```renode```

## 📝 Code Explanation

### 1. Initialization (```TIM2_Init```)
```C
// Enable Clock for TIM2 (APB1 Bus)
RCC->APB1ENR |= (1U << 0);
// Set Prescaler (16MHz -> 1kHz)
TIM2->PSC = 16000 - 1; 
// Set Auto-Reload (1kHz -> 1Hz)
TIM2->ARR = 1000 - 1; 
// Enable Interrupt & Start Timer
TIM2->DIER |= (1U << 0);
TIM2->CR1 |= (1U << 0);
NVIC_EnableIRQ(TIM2_IRQn);
```
### 2. Interrupt Handler (TIM2_IRQHandler)
This function interrupts the CPU exactly once per second.
```C
void TIM2_IRQHandler(void)
{
    // Check if Update Interrupt Flag is set
    if (TIM2->SR & (1U << 0)) 
    {
        GPIOD->ODR ^= (1U << 12); // Toggle LED
        TIM2->SR &= ~(1U << 0);   // Clear Flag (Crucial!)
    }
}
```


## 🐞 Debugging with VS Code

You can attach VS Code to the Renode simulation to step through code, inspect variables, and view peripheral registers.

### Prerequisites
* **Extension:** Install the **C/C++** extension (Microsoft) in VS Code.
* **SVD File:** Ensure `STM32F407.svd` is in your project root to view registers.

### How to Attach
1.  **Start Renode:** Run the simulation script (`renode renode_board/stm32f407vgt6.resc`).
    * *Note: The script is configured to start a GDB server on port `1234`.*
2.  **Configure Launch:** Create a debug configuration in `.vscode/launch.json` with the following key settings:
    * **Type:** `cppdbg`
    * **Request:** `launch`
    * **Debugger Path:** Point to your `arm-none-eabi-gdb`.
    * **Server Address:** `localhost:1234` (Connects to Renode).
    * **SVD Path:** Point to `${workspaceRoot}/STM32F407.svd`.
3.  **Start Debugging:** Press `F5` (or Run > Start Debugging). VS Code will attach to the running simulation, allowing you to hit breakpoints and inspect the MCU state.