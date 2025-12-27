# STM32F407 GPIO Interrupts (EXTI)

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Simulation](https://img.shields.io/badge/simulation-Renode-orange.svg)

## 📖 Overview

This project demonstrates how to use **External Interrupts (EXTI)** on the STM32F407 without using HAL.
Instead of constantly checking the button state in a `while` loop (Polling), the CPU does nothing until the button is pressed. When pressed, the hardware triggers an interrupt, jumps to the **ISR (Interrupt Service Routine)**, and toggles the LED.

**Key Concepts:**
* **NVIC:** Nested Vector Interrupt Controller configuration.
* **SYSCFG:** Routing GPIO pins to EXTI lines.
* **ISR:** Writing the `EXTI0_IRQHandler` function.

## 📂 Project Structure

```text
01_GPIO_Interrupt
├── Core
│   └── Src
│       └── main.c        # Contains EXTI config and ISR logic
├── renode_board
│   └── stm32f407vgt6.resc # Simulation script with Button support
├── makefile
└── README.md
```



## 🚀 How to Test in Renode

Since there is no physical finger to press the button in a simulation, you must send a command to the "virtual" button using the Renode Monitor.

### 1. **Start the Simulation**

```bash
renode renode_board/stm32f407vgt6.resc
```
The simulation starts, and the LED should be OFF.
### 2. **Trigger the Button**
### Method 1: The Quick Click (Recommended)
To simulate a full button click (Press down + Release up) in one go:

```bash
sysbus.userButton PressAndRelease
```
### Method 2: Manual Control
If you want to hold the button down (e.g., to test level-triggered logic):

**1. Press and Hold:**
```bash
sysbus.userButton Press
```
Result: The Green LED (PD12) will toggle ON.

**2. Release:**

You can "release" the button (though the code triggers on the rising edge, so the action happened on the Press):

```bash
sysbus.userButton Release
```
### 3. Verify Output
If you have ```logLevel -1 sysbus.led12``` enabled for the LED, you will see:

```[NOISY] led12: LED state changed to True```

## 🛠 Build & Run

### Prerequisites

* arm-none-eabi-gcc
* make
* renode

## 📝 Code Explanation

### 1. The Interrupt Service Routine (ISR)
This function is not called by main(). It is called by the hardware when EXTI Line 0 (PA0) detects a rising edge.

```C
void EXTI0_IRQHandler(void)
{
    // 1. Check if the interrupt came from Line 0
    if (EXTI->PR & (1U << 0)) 
    {
        // 2. Toggle LED
        GPIOD->ODR ^= (1U << 12);
        
        // 3. CRITICAL: Clear the interrupt pending bit
        // If skipped, the CPU triggers this function infinitely!
        EXTI->PR |= (1U << 0);
    }
}
```
### 2. Initialization Logic
To make the button work as an interrupt, we perform three specific steps in main.c:

1. SYSCFG Clock: Required to modify EXTI routing.
```C
RCC->APB2ENR |= (1U << 14); // Enable SYSCFG
```
2. Route PA0 to EXTI0:
```C
SYSCFG->EXTICR[0] &= ~(0xF << 0); // 0000 maps to Port A
```
3. Enable NVIC:
```C
NVIC_EnableIRQ(EXTI0_IRQn); // Enable IRQ #6 (EXTI0) in the CPU
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