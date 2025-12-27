# STM32F407 PWM Output (Breathing LED)

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Feature](https://img.shields.io/badge/feature-PWM-purple.svg)

## 📖 Overview
This project generates a **Pulse Width Modulation (PWM)** signal on **PD12** (Green LED) using **TIM4**. 
By varying the duty cycle (the % of time the signal is ON), we create a "Breathing LED" effect.

## 📂 Project Structure

```text
03_Timer_Interrupt
├── Core
│   └── Src
│       └── main.c        # TIM4 PWM Signal Generation LED Fade
├── renode_board
│   └── stm32f407vgt6.resc # Simulation script
├── makefile
└── README.md
```

## ⚠️ Renode Limitation
Renode simulates digital logic, not analog physics.
* **Real Board:** The LED fades smoothly in and out.
* **Renode:** The LED toggles ON/OFF rapidly. You cannot verify "dimming" with your eyes.
* **Verification:** You must use the **Logic Analyzer** to see the changing pulse width.

## 🚀 How to Test in Renode

### 1. Start Simulation
```bash
renode renode_board/stm32f407vgt6.resc
```
### 2. Open In Renode
In the Renode console, type:
```text
sysbus.timer4 ReadDoubleWord 0x34
```


## 📝 Code Explanation
**Alternate Functions (AF)**

Pins on STM32 are multiplexed. To connect the internal TIM4 to the external PD12 pin, we switch the pin mode from Output to Alternate Function (AF).
* Register: GPIOD->AFR[1] (High Register for pins 8-15).

* Value: AF2 (0010) maps TIM4 to PD12.

**PWM Logic**

* ARR (Period): 1000 ticks.

* CCR1 (Duty): Varies from 0 to 1000.

* Result: * CCR1 = 100 -> 10% ON (Dim)

    * CCR1 = 900 -> 90% ON (Bright)

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