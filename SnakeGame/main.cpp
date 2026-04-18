#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <string>
#include <cctype>
#include <limits>

using namespace std;

const int MAX_TAIL = 600;
const int SCREEN_X = 1;
const int SCREEN_Y = 3;

const string RESET     = "\033[0m";
const string CYAN_B    = "\033[96m";
const string GREEN_B   = "\033[92m";
const string GREEN_D   = "\033[32m";
const string RED_B     = "\033[91m";
const string YELLOW_B  = "\033[93m";
const string WHITE_B   = "\033[97m";
const string DIM       = "\033[2m";
const string MAGENTA_B = "\033[95m";

/**
 * @brief Defines a playable map with dimensions and optional portal positions.
 *
 * Portal pairs connect opposite walls: Top <-> Bottom, Left <-> Right.
 * When hasPortals is false, all wall collisions result in death.
 */
struct GameMap {
    string name;
    int width;
    int height;
    bool hasPortals;
    int topPortalX;
    int bottomPortalX;
    int leftPortalY;
    int rightPortalY;
};

GameMap maps[3] = {
    {"Classic",    30, 20, false, -1, -1, -1, -1},
    {"Portal Run", 30, 20, true,  15, 15, 10, 10},
    {"Wide Arena", 40, 15, true,  20, 20,  7,  7}
};

int WIDTH, HEIGHT;
GameMap* currentMap;

bool gameOver;
int headX, headY;
int fruitX, fruitY;
int score;
int speedLevel;
int highScore = 0;
int tailX[MAX_TAIL], tailY[MAX_TAIL];
int tailLength;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void GotoXY(int x, int y);
void HideCursor();
void Setup();
void DrawFrame();
void DrawCell(int gx, int gy, const string& ch, const string& color);
void ClearCell(int gx, int gy);
void SpawnFruit();
void UpdateScore();
void Input();
void Logic();
bool GameOverScreen();
int GetSleepTime();

/**
 * @brief Entry point for Snake Pro.
 *
 * Runs a session loop: map selection → game loop → game over screen
 * with replay prompt. High score persists across replays.
 *
 * @return int Exit code (0 on success).
 */
int main() {
    SetConsoleOutputCP(65001);
    srand(time(0));

    bool playing = true;
    while (playing) {
        Setup();

        while (!gameOver) {
            Input();
            Logic();
            Sleep(GetSleepTime());
        }

        if (score > highScore) highScore = score;

        playing = GameOverScreen();
    }

    return 0;
}

/**
 * @brief Calculates the frame delay based on direction and speed level.
 *
 * Vertical movement uses a longer delay to compensate for terminal
 * characters being taller than wide. Speed level scales from 1 (slow)
 * to 10 (fast), increasing every 50 points scored.
 *
 * @return int Sleep duration in milliseconds.
 */
int GetSleepTime() {
    if (dir == STOP) return 50;
    if (dir == UP || dir == DOWN)
        return 180 - (speedLevel - 1) * 10;
    return 150 - (speedLevel - 1) * 8;
}

/**
 * @brief Moves the console cursor to an absolute screen position.
 *
 * @param x Column position (0-indexed from the left).
 * @param y Row position (0-indexed from the top).
 */
void GotoXY(int x, int y) {
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/**
 * @brief Hides the blinking console cursor for a cleaner game display.
 */
void HideCursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 25;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

/**
 * @brief Draws a colored character at a game-coordinate position.
 *
 * @param gx   Game X coordinate.
 * @param gy   Game Y coordinate.
 * @param ch   The character string to render.
 * @param color ANSI color escape code.
 */
void DrawCell(int gx, int gy, const string& ch, const string& color) {
    GotoXY(gx + SCREEN_X, gy + SCREEN_Y);
    cout << color << ch << RESET;
}

/**
 * @brief Erases a cell at a game-coordinate position by writing a space.
 *
 * @param gx Game X coordinate.
 * @param gy Game Y coordinate.
 */
void ClearCell(int gx, int gy) {
    GotoXY(gx + SCREEN_X, gy + SCREEN_Y);
    cout << " ";
}

/**
 * @brief Spawns the fruit at a random empty cell not occupied by the snake.
 */
void SpawnFruit() {
    if (tailLength >= WIDTH * HEIGHT - 1) return;

    bool onSnake;
    do {
        onSnake = false;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;

        if (fruitX == headX && fruitY == headY) { onSnake = true; continue; }
        for (int i = 0; i < tailLength; i++) {
            if (fruitX == tailX[i] && fruitY == tailY[i]) {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);
}

/**
 * @brief Recalculates speed level from score and updates the HUD display.
 *
 * Speed increases by 1 every 50 points, capping at level 10.
 */
void UpdateScore() {
    int newLevel = 1 + score / 50;
    if (newLevel > 10) newLevel = 10;
    speedLevel = newLevel;

    GotoXY(10, 1);
    cout << YELLOW_B << score << "     " << RESET;

    GotoXY(20, 1);
    cout << WHITE_B << "|  Speed: " << YELLOW_B << speedLevel << "  " << RESET;
}

/**
 * @brief Displays the map selection menu and initializes all game state.
 *
 * Presents 3 map choices, validates input, then draws the static frame
 * and places the initial snake head and fruit.
 */
void Setup() {
    system("cls");

    cout << YELLOW_B << "\n          SNAKE PRO\n" << RESET << endl;

    if (highScore > 0) {
        cout << "    " << WHITE_B << "High Score: " << YELLOW_B << highScore << RESET << "\n" << endl;
    }

    cout << WHITE_B << "    Choose Your Map:\n" << RESET << endl;

    for (int i = 0; i < 3; i++) {
        cout << "    " << CYAN_B << "[" << (i + 1) << "]" << RESET << " ";
        cout << WHITE_B << maps[i].name << RESET;
        cout << DIM << " (" << maps[i].width << "x" << maps[i].height << ")";
        if (maps[i].hasPortals)
            cout << " - 4 wall portals";
        else
            cout << " - No portals";
        cout << RESET << endl;
    }

    cout << endl;
    cout << CYAN_B << "    Enter choice (1-3): " << RESET;

    int choice;
    while (true) {
        if (cin >> choice && choice >= 1 && choice <= 3) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED_B << "    Invalid! Enter 1, 2, or 3: " << RESET;
    }

    currentMap = &maps[choice - 1];
    WIDTH = currentMap->width;
    HEIGHT = currentMap->height;

    system("cls");
    HideCursor();

    gameOver = false;
    dir = STOP;
    headX = WIDTH / 2;
    headY = HEIGHT / 2;
    tailLength = 0;
    score = 0;
    speedLevel = 1;

    SpawnFruit();
    DrawFrame();

    DrawCell(headX, headY, "@", GREEN_B);
    DrawCell(fruitX, fruitY, "*", RED_B);
}

/**
 * @brief Renders the static game frame once (borders, title, score, controls).
 *
 * Portal positions are rendered as magenta gaps in the cyan walls.
 * Only dynamic elements (snake, fruit, score, speed) are updated per-frame.
 */
void DrawFrame() {
    int totalWidth = WIDTH + 2;

    string title = "SNAKE PRO";
    string fullTitle = title + " - " + currentMap->name;
    int titlePad = (totalWidth - (int)fullTitle.length()) / 2;
    if (titlePad < 0) titlePad = 0;

    GotoXY(titlePad, 0);
    cout << YELLOW_B << title << RESET << DIM << " - " << currentMap->name << RESET;

    GotoXY(2, 1);
    cout << WHITE_B << "Score: " << YELLOW_B << "0" << RESET;
    GotoXY(20, 1);
    cout << WHITE_B << "|  Speed: " << YELLOW_B << "1" << RESET;

    GotoXY(0, 2);
    cout << CYAN_B << "+";
    for (int i = 0; i < WIDTH; i++) {
        if (currentMap->hasPortals && i == currentMap->topPortalX)
            cout << MAGENTA_B << " " << RESET;
        else
            cout << CYAN_B << "-";
    }
    cout << CYAN_B << "+" << RESET;

    for (int i = 0; i < HEIGHT; i++) {
        GotoXY(0, SCREEN_Y + i);
        if (currentMap->hasPortals && i == currentMap->leftPortalY)
            cout << MAGENTA_B << " " << RESET;
        else
            cout << CYAN_B << "|" << RESET;

        GotoXY(WIDTH + 1, SCREEN_Y + i);
        if (currentMap->hasPortals && i == currentMap->rightPortalY)
            cout << MAGENTA_B << " " << RESET;
        else
            cout << CYAN_B << "|" << RESET;
    }

    GotoXY(0, SCREEN_Y + HEIGHT);
    cout << CYAN_B << "+";
    for (int i = 0; i < WIDTH; i++) {
        if (currentMap->hasPortals && i == currentMap->bottomPortalX)
            cout << MAGENTA_B << " " << RESET;
        else
            cout << CYAN_B << "-";
    }
    cout << CYAN_B << "+" << RESET;

    GotoXY(2, SCREEN_Y + HEIGHT + 1);
    cout << DIM << "W/A/S/D or Arrows = Move | X = Quit" << RESET;
}

/**
 * @brief Reads keyboard input and updates the snake's direction.
 *
 * Supports both WASD keys and arrow keys. Prevents 180-degree
 * reversals. Press X to quit immediately.
 */
void Input() {
    if (_kbhit()) {
        int ch = _getch();

        if (ch == 0 || ch == 224) {
            ch = _getch();
            switch (ch) {
                case 72: if (dir != DOWN)  dir = UP;    break;
                case 80: if (dir != UP)    dir = DOWN;  break;
                case 75: if (dir != RIGHT) dir = LEFT;  break;
                case 77: if (dir != LEFT)  dir = RIGHT; break;
            }
        } else {
            switch (tolower(ch)) {
                case 'w': if (dir != DOWN)  dir = UP;    break;
                case 's': if (dir != UP)    dir = DOWN;  break;
                case 'a': if (dir != RIGHT) dir = LEFT;  break;
                case 'd': if (dir != LEFT)  dir = RIGHT; break;
                case 'x': gameOver = true; break;
            }
        }
    }
}

/**
 * @brief Core game logic executed once per tick.
 *
 * Handles head movement, portal teleportation (if the current map
 * supports portals and the head exits through a portal gap), wall
 * and self-collision detection, fruit pickup with tail growth, and
 * surgical per-cell screen updates for flicker-free rendering.
 */
void Logic() {
    if (dir == STOP) return;

    int prevX = headX;
    int prevY = headY;

    switch (dir) {
        case LEFT:  headX--; break;
        case RIGHT: headX++; break;
        case UP:    headY--; break;
        case DOWN:  headY++; break;
        default: break;
    }

    bool outOfBounds = (headX < 0 || headX >= WIDTH || headY < 0 || headY >= HEIGHT);

    if (outOfBounds) {
        bool teleported = false;

        if (currentMap->hasPortals) {
            if (headY < 0 && headX == currentMap->topPortalX) {
                headX = currentMap->bottomPortalX;
                headY = HEIGHT - 1;
                teleported = true;
            } else if (headY >= HEIGHT && headX == currentMap->bottomPortalX) {
                headX = currentMap->topPortalX;
                headY = 0;
                teleported = true;
            } else if (headX < 0 && headY == currentMap->leftPortalY) {
                headX = WIDTH - 1;
                headY = currentMap->rightPortalY;
                teleported = true;
            } else if (headX >= WIDTH && headY == currentMap->rightPortalY) {
                headX = 0;
                headY = currentMap->leftPortalY;
                teleported = true;
            }
        }

        if (!teleported) {
            gameOver = true;
            return;
        }
    }

    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == headX && tailY[i] == headY) {
            gameOver = true;
            return;
        }
    }

    bool ate = (headX == fruitX && headY == fruitY);

    if (ate) {
        for (int i = tailLength; i > 0; i--) {
            tailX[i] = tailX[i - 1];
            tailY[i] = tailY[i - 1];
        }
        tailX[0] = prevX;
        tailY[0] = prevY;
        tailLength++;

        score += 10;
        SpawnFruit();
    } else if (tailLength > 0) {
        ClearCell(tailX[tailLength - 1], tailY[tailLength - 1]);

        for (int i = tailLength - 1; i > 0; i--) {
            tailX[i] = tailX[i - 1];
            tailY[i] = tailY[i - 1];
        }
        tailX[0] = prevX;
        tailY[0] = prevY;
    } else {
        ClearCell(prevX, prevY);
    }

    if (tailLength > 0) {
        DrawCell(tailX[0], tailY[0], "o", GREEN_D);
    }
    DrawCell(headX, headY, "@", GREEN_B);

    if (ate) {
        DrawCell(fruitX, fruitY, "*", RED_B);
        UpdateScore();
    }
}

/**
 * @brief Displays the Game Over overlay with score, high score, and replay prompt.
 *
 * Shows a centered red box over the final board state. Displays the
 * session high score and asks the player to play again or quit.
 *
 * @return true  If the player chooses to play again.
 * @return false If the player chooses to quit.
 */
bool GameOverScreen() {
    int boxWidth = 20;
    int boxX = (WIDTH + 2 - boxWidth) / 2;
    int boxY = HEIGHT / 2 + SCREEN_Y - 3;

    GotoXY(boxX, boxY);
    cout << RED_B << "+------------------+" << RESET;
    GotoXY(boxX, boxY + 1);
    cout << RED_B << "|                  |" << RESET;
    GotoXY(boxX, boxY + 2);
    cout << RED_B << "|    GAME  OVER    |" << RESET;
    GotoXY(boxX, boxY + 3);
    cout << RED_B << "|                  |" << RESET;
    GotoXY(boxX, boxY + 4);
    cout << RED_B << "+------------------+" << RESET;

    GotoXY(boxX + 1, boxY + 6);
    cout << WHITE_B << "Score: " << YELLOW_B << score << RESET;

    GotoXY(boxX + 1, boxY + 7);
    cout << WHITE_B << "Best:  " << YELLOW_B << highScore << RESET;

    GotoXY(boxX - 2, boxY + 9);
    cout << CYAN_B << "Play again? (Y/N): " << RESET;

    while (true) {
        int ch = _getch();
        if (tolower(ch) == 'y') {
            return true;
        } else if (tolower(ch) == 'n') {
            GotoXY(0, SCREEN_Y + HEIGHT + 3);
            return false;
        }
    }
}