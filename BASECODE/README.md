# STM32F407 Bare-Metal Blink with Renode Simulation

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Simulation](https://img.shields.io/badge/simulation-Renode-orange.svg)


## 📖 Overview

This project implements a **bare-metal** GPIO toggle application for the **STM32F407 Discovery** board. It demonstrates direct register manipulation (CMSIS-Core) without using HAL or the Standard Peripheral Library. 

Crucially, this project includes a complete **Renode** simulation environment, allowing you to run, debug, and visualize the firmware behavior (LED blinking) without physical hardware.

## 📂 Project Structure

```text
.
├── Core
│   ├── Inc                  # Headers (CMSIS, Device specific)
│   └── Src                  # Source code (main.c, startup, syscalls)
├── renode_board             # Renode simulation scripts
│   ├── stm32f407_discovery.repl  # Platform description
│   └── stm32f407vgt6.resc        # Script to start simulation
├── STM32F407.svd            # System View Description for debug
├── STM32F407VGTx_FLASH.ld   # Linker script
├── makefile                 # Build configuration
└── README.md                # This document 
```

## 🛠 Prerequisites
To build and run this project, ensure you have the following installed:

* **Toolchain:** arm-none-eabi-gcc
* **Build System:** make
* **Simulator:** Renode
* **Hardware Flashing (Optional):** OpenOCD (if using physical board)

## 🚀 Getting Started
### 1. Clone the Repository
```bash
git clone https://github.com/Jaisai1337/STM32F407VGT6_Renode
cd STM32F407VGT6_Renode/BASECODE
```

### 2. Build the Firmware
Run the makefile to compile the source code and generate the ELF binary.

```bash
make
```
Output will be generated in the build/ directory.

### 3. Run Simulation (Renode)
This project uses Renode to simulate the STM32F407 hardware.

```bash
renode renode_board/stm32f407vgt6.resc
```
**Expected Behavior:**

1. Renode monitor and UART analyzer windows will open.
2. The simulation loads the ./build/BASECODE.elf (ensure the path in .resc matches your build output).
3. Virtual LEDs connected to PD12-PD15 will blink.

Note: The delay function uses a blocking loop. Simulation speed depends on your host machine.

## 📝 Code Explanation

The core logic resides in main.c. It bypasses HAL to manipulate registers directly:

* RCC Setup: Enables the AHB1 clock for GPIOD.
* GPIO Mode: Configures pins 12, 13, 14, and 15 of Port D as Output.
* Logic Loop: Toggles the ODR (Output Data Register) and waits using a simple CPU cycle delay.

```C
// Enable GPIOD Clock
RCC->AHB1ENR |= (1U << 3);
// Set PD12-PD15 to Output Mode
GPIOD->MODER |= (1U << 24) | (1U << 26) | (1U << 28) | (1U << 30);
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