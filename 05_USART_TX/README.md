# STM32F407 USART2 TX (Hello World)

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Feature](https://img.shields.io/badge/feature-UART-purple.svg)

## 📖 Overview
This project enables **Serial Communication** (UART). 
It configures **USART2** on pin **PA2** to transmit text messages at **115200 baud**. 

This is the most essential feature for embedded development, as it allows you to print debug information (variables, errors, status) to your computer screen.

## 📂 Project Structure

```text
05_USART_TX
├── Core
│   └── Src
│       └── main.c        # USART2 configuration and print logic
├── renode_board
│   └── stm32f407vgt6.resc # Script enabling UART analyzer
├── makefile
└── README.md
```

## ⚙️ Configuration Details
* Pin: PA2 (Alternate Function 7).
* Baud Rate: 115200 bps.
* Clock: 16 MHz (Default HSI).
* Settings: 8 Data bits, No Parity, 1 Stop bit (8N1).


## 🛠 Build & Run
### Prerequisites
* ```arm-none-eabi-gcc```
* ```make```
* ```renode```

## 3. Run Simulation (Renode)
1. Start Simulation:
    ```bash
    renode renode_board/stm32f407vgt6.resc
    ```
2. Observe Output:

    * A window named ```sysbus.usart2``` will automatically open.

    * Once you start the simulation, the text ```Hello from STM32F407 Bare Metal!``` will appear every second.

    Note: If the window does not appear, you can manually open it by typing ```showAnalyzer sysbus.usart2``` in the Renode console.


## 📝 Code Explanation
### 1. Pin Configuration (Alternate Function)
Unlike simple LED blinking (Output Mode), the UART peripheral needs direct control of the pin. We set **PA2** to **Alternate Function Mode** (AF) and specifically select **AF7**, which maps to USART2.

```c
// 1. Set PA2 to Alternate Function Mode (Binary 10)
GPIOA->MODER |= (2U << 4);

// 2. Select AF7 (USART2) in Alternate Function Low Register
// PA2 is pin 2. Position is 2 * 4 bits = 8.
GPIOA->AFR[0] |= (7U << 8);
```

### 2. Baud Rate Calculation
The STM32 needs to know how fast to send bits. We targeted 115200 bps with a 16 MHz clock.

The formula is: $$ \text{USARTDIV} = \frac{\text{f}_{CLK}}{16 \times \text{Baud}} = \frac{16,000,000}{1,843,200} = 8.68 $$

We split this into the integer part (Mantissa) and fractional part (Fraction).

* Mantissa: 8
* Fraction: $0.6805 \times 16 \approx 11$ (```0xB```)
* Register (BRR): ```0x008B```
```c
USART2->BRR = 0x008B; // Set Calculated Baud Rate
```
### 3. Transmit Function
To send a character, we must ensure the hardware is ready. We check the **TXE** (Transmit Data Register Empty) flag in the Status Register (```SR```).
```c
void USART2_Write(int ch)
{
    // 1. Wait until the register is empty (Bit 7)
    while (!(USART2->SR & (1U << 7))) {}

    // 2. Write the character to the Data Register
    USART2->DR = (ch & 0xFF);
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