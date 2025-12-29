# STM32F407 USART2 Interrupts & Ring Buffer

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Platform](https://img.shields.io/badge/platform-STM32F407-green.svg)
![Feature](https://img.shields.io/badge/feature-UART-purple.svg)
![Feature](https://img.shields.io/badge/feature-Ring_Buffer-purple.svg)
![Feature](https://img.shields.io/badge/feature-Interrupts-orange.svg)

## 📖 Overview
This project upgrades the UART driver to be **Non-Blocking** and **Robust**.
* **RXNE Interrupt:** The CPU no longer waits for data. It runs other tasks (blinking LED) and only pauses briefly when a byte physically arrives.
* **Ring Buffer (FIFO):** A 64-byte circular buffer stores incoming data. This ensures that if the CPU is busy doing something else when data arrives, the data is saved rather than lost.

## 📂 Project Structure

```text
07_USART_RingBuffer
├── Core
│   └── Src
│       └── main.c        # Ring Buffer implementation & ISR
├── renode_board
│   └── stm32f407vgt6.resc 
├── makefile
└── README.md
```

## ⚙️ Configuration Details
* Pins: PA2 (TX), PA3 (RX)
* Baud: 115200
* Interrupt: USART2 Global Interrupt (NVIC IRQ #38)


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
### 2. Observe LED: 
The Green LED (PD12) blinks continuously. This proves the Main Loop is active and not blocked.

### 3. Type Data: 
Click the UART window and type. The characters are echoed back.

*Note*: Even if you type faster than the LED blinks, the Ring Buffer captures the keystrokes.


## 📝 Code Explanation
### 1.Enabling Interrupts
We must tell the UART peripheral to generate an interrupt signal when data arrives (```RXNEIE```), and tell the CPU (NVIC) to listen for it.

```c
// Enable RX Not Empty Interrupt
USART2->CR1 |= (1U << 5); S
// Enable NVIC for USART2
NVIC_EnableIRQ(USART2_IRQn);
```

### 2. The Ring Buffer Logic
A circular buffer uses two indices: head (write) and tail (read).

* **Write (ISR)**: buffer[head] = data; head = (head + 1) % SIZE;
* **Read (Main)**: data = buffer[tail]; tail = (tail + 1) % SIZE;
* **Full Condition**: (head + 1) % SIZE == tail
* **Empty Condition**: head == tail


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