# 🕹️ C++ Terminal Games Collection

![C++ Game Studio Banner](file:///C:/Users/Tedy/.gemini/antigravity/brain/632db42e-ab23-42ba-b556-81f563d1c8d7/game_studio_banner_1776522576042.png)

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

### 1. [Tic-Tac-Toe Classic](file:///d:/Designs/GameDevelopment/TicTacToe/README.md)
A standard 2-player version of the classic game. Perfect for playing with a friend on the same terminal.

### 2. [Tic-Tac-Toe Pro (AI Edition)](file:///d:/Designs/GameDevelopment/SmartTicTacToe/README.md)
An advanced version featuring:
- **Computer Opponent**: A strategic AI that plays against you.
- **Colorized UI**: Vibrant ANSI terminal colors for a premium feel.
- **Windows Optimized**: Includes subtle delays for a "thinking" AI experience.

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
