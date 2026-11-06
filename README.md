# 🎰 CHIP-8 Emulator in C

An emulator for the **CHIP-8** interpreted programming language, written entirely in C. This project loads and executes CHIP-8 ROMs by interpreting the original 8-bit instruction set, managing memory, registers, timers, and pixel rendering.

---

## 🛠️ Architecture & Features

* **CPU / Core**: Complete emulation of the CHIP-8 instruction set (35 opcodes).
* **Display**: Original $64 \times 32$ pixel resolution (rendered using [e.g., SDL2 / Raylib / Ncurses]).
* **Input**: 16-key hexadecimal keypad ($0\text{-}F$) mapped to standard QWERTY keys.
* **Timers**: 60 Hz Delay Timer and Sound Timer implementations.
* **Memory**: 4KB emulated RAM with the default fontset loaded in the reserved space ($0x000 - 0x1FF$).

---

## 📜 Credits & Resources

* **Guide** : [text](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) 

--- 

## 📁 Project Structure

```text
.
├── include/       # Header files (.h)
├── src/           # Source files (.c)
├── roms/          # Test ROMs and games
├── Makefile       # Build automation script
└── README.md
