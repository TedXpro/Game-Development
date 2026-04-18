# ⭕ Tic-Tac-Toe Classic (2-Player)

A clean, terminal-based implementation of the classic 3x3 game.

## ✨ Features
- **Local Multiplayer**: Two players can play on the same machine.
- **Input Validation**: Prevents invalid moves and slot hijacking.
- **Color Coding**: `X` is marked in Red, `O` in Blue for clarity.

## 🛠️ Setup & Build
Ensure you have `g++` installed.

1. **Compile**:
   ```powershell
   g++ -static-libgcc -static-libstdc++ main.cpp -o tictactoe.exe
   ```
2. **Run**:
   ```powershell
   ./tictactoe.exe
   ```

## 🎮 How to Play
The board is mapped to numbers **1-9**:
```
 1 | 2 | 3 
---|---|---
 4 | 5 | 6 
---|---|---
 7 | 8 | 9 
```
Simply type the number of the slot you want to mark!
