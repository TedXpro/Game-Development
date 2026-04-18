# 🤖 Tic-Tac-Toe Pro (AI Edition)

A sophisticated version of Tic-Tac-Toe featuring an unbeatable MinMax AI opponent, a Best-of-X series mode, and a premium ANSI-colored terminal interface.

## ✨ Features
- **MinMax AI**: Uses the MinMax algorithm to evaluate every possible game state — the computer plays optimally and never loses.
- **Best of 3 / Best of 5**: Choose a series format with live score tracking across rounds.
- **Random & Alternating Starters**: Round 1 is randomly assigned; after that, starters alternate fairly.
- **Round Cap**: The series ends after the chosen number of rounds. If no one clinches, the player with the most wins takes it — or it's declared a draw.
- **Vibrant UI**: ANSI color-coded markers (Red for X, Blue for O) with a live scoreboard.
- **Thinking Engine**: A 1-second delay simulates the computer "calculating" its move.
- **Robust Input Validation**: Handles non-numeric inputs, out-of-range slots, and occupied cells gracefully.

## 🛠️ Setup & Build
This version uses `windows.h` for `Sleep()` and `system("cls")`, so it is optimized for Windows environments with MinGW/GCC.

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
2. **Choose Series**: Select Best of 3 (first to 2 wins) or Best of 5 (first to 3 wins).
3. **Play Rounds**: Take turns placing your marker on slots 1-9. The computer responds with its optimal move.
4. **Win the Series**: Accumulate enough round wins before the series ends!

## 🧠 AI Logic (MinMax Algorithm)
The AI uses a recursive MinMax search tree:
1. **Evaluate**: Scores every possible board state (+10 for AI win, -10 for player win, 0 for draw).
2. **Maximize**: On the AI's turn, it picks the move with the highest score.
3. **Minimize**: On the player's turn, it assumes the player picks the move with the lowest score.
4. **Randomize**: Among equally-optimal moves, one is chosen at random for variety.
