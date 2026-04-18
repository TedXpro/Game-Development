# 🐍 Snake Pro

A high-performance terminal Snake game with flicker-free rendering, portal mechanics, and an adaptive speed system.

## ✨ Features

- **Flicker-Free Engine**: Uses `SetConsoleCursorPosition` (Windows API) to surgically update only the cells that change — zero screen tearing.
- **3 Playable Maps**:
  - **Classic** (30×20) — No portals. Wall = death.
  - **Portal Run** (30×20) — 4 wall portals (one per side) that teleport the snake to the opposite wall.
  - **Wide Arena** (40×15) — Wider playfield with portals for fast-paced gameplay.
- **Adaptive Speed**: Starts at Speed 1 and increases every 50 points up to Speed 10.
- **ANSI Color UI**: Green snake, red fruit, cyan walls, magenta portal gaps, and a live HUD.
- **Replay System**: After Game Over, see your score vs. session high score and choose to play again or quit.
- **Dual Controls**: WASD keys **and** arrow keys are both supported.

## 🎮 Controls

| Key | Action |
|-----|--------|
| `W` / `↑` | Move Up |
| `A` / `←` | Move Left |
| `S` / `↓` | Move Down |
| `D` / `→` | Move Right |
| `X` | Quit Game |

## 🛠️ Build & Run

```powershell
cd SnakeGame
g++ -static-libgcc -static-libstdc++ main.cpp -o snake_game.exe
./snake_game.exe
```

## 🧠 Technical Details

- **Rendering**: `GotoXY()` wrapper around `SetConsoleCursorPosition`. Static frame drawn once in `Setup()`, only dynamic elements (snake, fruit, score, speed) updated per tick.
- **Tail Logic**: Shifting-array pattern — each segment inherits the position of the one ahead of it. On fruit pickup, the tail grows by one segment without erasing the old end.
- **Portal System**: Portals connect opposite walls. When the snake head crosses a portal gap, it teleports to the paired exit. The tail follows naturally through subsequent frames.
- **Speed Formula**: `horizontalSleep = 150 - (level-1) * 8`ms, `verticalSleep = 180 - (level-1) * 10`ms. Vertical is slower to compensate for terminal characters being taller than wide.
