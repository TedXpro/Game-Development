# 🕹️ C++ Terminal Games Collection

Welcome to my collection of classic terminal games built with C++. This project is a "Game Studio" style monorepo containing various board games and AI experiments.

## 🚀 Getting Started

### Prerequisites
To build and play these games, you need:
- A **C++ Compiler** (GCC/MinGW is recommended for Windows).
- **Git** (for version control).

### Quick Setup (Windows)
1. **Install MinGW**: [Download here](https://osdn.net/projects/mingw/releases/) or use `choco install mingw`.
2. **Add to PATH**: Ensure your C++ compiler is in your System Environment Variables.
3. **Verify**: Open a terminal and type `g++ --version`.

---

## 🎮 The Games

### 1. [Tic-Tac-Toe Classic](./TicTacToe/README.md)
A standard 2-player version of the classic game. Perfect for playing with a friend on the same terminal.

### 2. [Tic-Tac-Toe Pro (AI Edition)](./SmartTicTacToe/README.md)
An advanced version featuring:
- **Unbeatable MinMax AI**: The computer evaluates every possible game state and plays optimally.
- **Best of 3 / Best of 5 Series**: Live score tracking with round caps and alternating starters.
- **Colorized UI**: Vibrant ANSI terminal colors with a live scoreboard.


---

## 🛠️ How to Build All Games
Each game has a `main.cpp`. To compile any game manually:
```powershell
# Example for Tic-Tac-Toe
cd TicTacToe
g++ -static-libgcc -static-libstdc++ main.cpp -o tictactoe.exe
./tictactoe.exe
```

## 📜 License
This project is for educational purposes. Feel free to use and modify!
