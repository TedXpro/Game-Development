# 🤖 Tic-Tac-Toe Pro (AI Edition)

A sophisticated version of Tic-Tac-Toe featuring a computer opponent and a premium terminal interface.

## ✨ Features
- **Smart AI**: The computer evaluates the board to block your moves or secure its own wins.
- **Vibrant UI**: Uses ANSI color codes for a modern terminal look.
- **Thinking Engine**: Subtle delays (`Sleep`) simulate a computer "calculating" its move for better immersion.
- **Robust Error Handling**: Handles non-numeric inputs and invalid slots gracefully.

## 🛠️ Setup & Build
This version uses `windows.h` for specialized functionality, so it is optimized for Windows environments.

1. **Compile**:
   ```powershell
   g++ -static-libgcc -static-libstdc++ main.cpp -o smart_tictactoe.exe
   ```
2. **Run**:
   ```powershell
   ./smart_tictactoe.exe
   ```

## 🎮 How to Play
1. **Choose Marker**: Pick `X` or `O`.
2. **Player 1**: You go first by choosing a slot (1-9).
3. **Computer**: The computer will automatically respond with its best move.
4. **Win**: Get three in a row to defeat the machine!

## 🧠 AI Logic
The "Pro" AI uses a strategic search:
1. **Win**: If it can win in one move, it takes it.
2. **Block**: If you are about to win, it blocks your path.
3. **Random Strategic**: If no immediate wins or blocks exist, it picks a strategic slot.
