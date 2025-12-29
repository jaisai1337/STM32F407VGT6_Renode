# STM32F407 USART2 RX (Echo)

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Feature](https://img.shields.io/badge/feature-UART-purple.svg)

## 📖 Overview
This project enables **UART Reception**. 
The STM32 waits for a character to be received on **PA3** and immediately transmits it back on **PA2** (Echo).

This creates a "Loopback" effect, proving that both Transmission and Reception are working correctly.

## 📂 Project Structure

```text
06_USART_RX
├── Core
│   └── Src
│       └── main.c        # RX and Echo logic
├── renode_board
│   └── stm32f407vgt6.resc 
├── makefile
└── README.md
```

## ⚙️ Configuration Details
* **Pins:** 
    * **PA2** (TX) - Alternate Function 7
    * **PA3** (RX) - Alternate Function 7
* **Baud Rate:** 115200 bps
* **Clock:** 16 MHz (Default HSI)
* **Settings:** 8 Data bits, No Parity, 1 Stop bit (8N1)

## 🛠 Build & Run
### Prerequisites
* ```arm-none-eabi-gcc```
* ```make```
* ```renode```

## 3. Run Simulation (Renode)
### 1. Start Simulation:
```bash
renode renode_board/stm32f407vgt6.resc
```
### 2. Interact:

* A terminal window (```sysbus.usart2```) will open.

* Click inside the window and type keys on your keyboard.

* You will see the characters appear.

* *Technically*: You press 'A' -> Renode sends 'A' to STM32 (RX) -> STM32 reads 'A' -> STM32 sends 'A' back (TX) -> Renode displays 'A'.


## 📝 Code Explanation
### 1. Enabling the Receiver
We configure Control Register 1 (CR1) to enable both bits.

* TE (Bit 3): Transmitter Enable
* RE (Bit 2): Receiver Enable
```c
USART2->CR1 = (1U << 13) | (1U << 3) | (1U << 2);
```

### 2. Reading Data (Blocking)
To read a byte, we must wait until the hardware says "I have data!". We check the **RXNE** (Read Data Register Not Empty) flag.
```c
char USART2_Read(void)
{
    // Wait until RXNE (Bit 5) is set
    while (!(USART2->SR & (1U << 5))) {}
    
    // Reading DR automatically clears the RXNE flag
    return USART2->DR;
}
```
### 3. Pin Configuration (PA3)
PA3 is the RX pin for USART2. It requires Alternate Function 7.
```c
// Set PA3 to Alternate Function Mode
GPIOA->MODER |= (2U << 6); 

// Map PA3 to AF7 (USART2)
GPIOA->AFR[0] |= (7U << 12);
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
