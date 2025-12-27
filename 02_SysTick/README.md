# STM32F407 SysTick & Millis

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Simulation](https://img.shields.io/badge/simulation-Renode-orange.svg)

## 📖 Overview

This project replaces crude `for` loop delays with precise timing using the ARM Cortex-M4 **SysTick Timer**.

It implements a system very similar to Arduino's `millis()`:
* **`SysTick_Init()`**: Configures the hardware timer to trigger an interrupt exactly every **1ms**.
* **`millis()`**: Returns the number of milliseconds elapsed since the board started.
* **`delay_ms()`**: A precise delay function based on the system timer, independent of CPU speed.

## 📂 Project Structure

```text
02_SysTick
├── Core
│   └── Src
│       └── main.c        # Contains SysTick config and interrupt handler
├── renode_board
│   └── stm32f407vgt6.resc # Simulation script
├── makefile
└── README.md
```



## 🚀 How to Test in Renode

Since there is no physical finger to press the button in a simulation, you must send a command to the "virtual" button using the Renode Monitor.

### 1. **Start the Simulation**

```bash
renode renode_board/stm32f407vgt6.resc
```
### 2. **Verify Timing Accuracy**
Since simulations can run slower than real time (due to CPU load), you cannot trust your wall clock. You must trust the Virtual Clock.

1. Let the simulation run for a moment.

2. Type ```elapsed``` in the Renode Monitor.

3. Compare the Virtual Time with the LED toggles.

    * Code is set to ```delay_ms(100)``` (100ms).

    * The LED should toggle 10 times for every 1.0 second of Virtual Time.


## 🛠 Build & Run

### Prerequisites

* ```arm-none-eabi-gcc```
* ```make```
* ```renode```

## 📝 Code Explanation

### 1. Clock Configuration (Crucial!)
The STM32F407 defaults to **16 MHz** (HSI) on startup. We must match our math to this speed to ensure our timing is accurate.

**The Math:**
$$\text{Ticks per ms} = \frac{\text{System Clock}}{1000} = \frac{16,000,000}{1000} = 16,000$$

**The Register:**
We subtract 1 because the counter is 0-indexed (counting from 0 to N takes N+1 cycles).

```c
SysTick->LOAD = (16000000U / 1000) - 1; // Load 15,999
```

### 2. The Interrupt Handler
This function runs automatically every 1 millisecond.

```C
volatile uint32_t ticks = 0;

void SysTick_Handler(void)
{
    ticks++;
}
```
### 3. Non-Blocking Logic
We check the difference between the current time and the start time. This logic allows ```ticks``` to overflow (wrap around from 4 billion to 0) without breaking the delay calculation.
```C
void delay_ms(uint32_t ms)
{
    uint32_t start = millis();
    while ((millis() - start) < ms); // Wait
}
```

## ⚠️ Simulation Configuration
The Renode hardware definition must match your C code's clock speed, or the simulation timing will be wrong (e.g., 1 second in code = 10 seconds in simulation).

**File:** `renode_board/stm32f407_discovery.repl`

Ensure the frequency is set to **16000000** (16 MHz)


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