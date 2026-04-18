#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>
#include <ctime>
#include <windows.h>
#include <algorithm>

using namespace std;

const string RESET  = "\033[0m";
const string RED    = "\033[31m";
const string BLUE   = "\033[34m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN   = "\033[36m"; 

vector<vector<char>> board = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'}
};

char currentMarker; 
int currentPlayer;

int playerScore = 0;
int computerScore = 0;
int targetWins = 0;
int bestOf = 0;
int roundNumber = 0;
char playerChoice;
char computerChoice;

int MinMax(int depth, bool isMax, char aiMarker, char playerMarker);
bool IsMovesLeft();
int EvaluateBoard(char aiMarker, char playerMarker);
bool CheckLine(int r1, int c1, int r2, int c2, int r3, int c3, char marker);
bool FindBestMove(char markerToLookFor);
void ComputerMove();
void DrawBoard();
void ResetBoard();
bool CheckWin();
string GetColoredMarker(char marker);

int main() {
    srand(time(0));
    system("cls");

    cout << YELLOW << "\n";
    cout << "      ***********************" << endl;
    cout << "      * TIC-TAC-TOE PRO   *" << endl;
    cout << "      ***********************" << RESET << endl;
    cout << "\n";

    cout << CYAN << "  Choose your Marker (X or O): " << RESET;
    while (true) {
        if (cin >> playerChoice) {
            playerChoice = toupper(playerChoice);
            if (playerChoice == 'X' || playerChoice == 'O') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        } else {
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "  Invalid Marker! Please choose X or O: " << RESET;
    }
    computerChoice = (playerChoice == 'X') ? 'O' : 'X';

    int seriesChoice;
    cout << CYAN << "  Choose series - Best of (3 or 5): " << RESET;
    while (true) {
        if (cin >> seriesChoice) {
            if (seriesChoice == 3 || seriesChoice == 5) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        } else {
            cin.clear();
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "  Invalid choice! Please enter 3 or 5: " << RESET;
    }
    bestOf = seriesChoice;
    targetWins = (bestOf == 3) ? 2 : 3;

    bool humanStartsNext = (rand() % 2 == 0);

    while (playerScore < targetWins && computerScore < targetWins) {
        roundNumber++;
        ResetBoard();

        if (humanStartsNext) {
            currentPlayer = 1;
            currentMarker = playerChoice;
        } else {
            currentPlayer = 2;
            currentMarker = computerChoice;
        }

        DrawBoard();

        if (currentPlayer == 1) {
            cout << CYAN << "  You go first!" << RESET << endl;
        } else {
            cout << CYAN << "  Computer goes first!" << RESET << endl;
        }

        bool roundOver = false;
        for (int i = 0; i < 9 && !roundOver; i++) {
            if (currentPlayer == 1) {
                bool validMove = false;
                do {
                    cout << "  Your turn (" << GetColoredMarker(playerChoice) << "). Enter slot: ";
                    int slot;
                    if (!(cin >> slot)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << RED << "  Invalid Input! Please enter a number 1-9." << RESET << endl;
                        continue;
                    }
                    if (slot < 1 || slot > 9) {
                        cout << RED << "  Invalid Slot! Please choose 1-9." << RESET << endl;
                        continue;
                    }
                    int row = (slot - 1) / 3;
                    int col = (slot - 1) % 3;
                    if (board[row][col] != 'X' && board[row][col] != 'O') {
                        board[row][col] = currentMarker;
                        validMove = true;
                    } else {
                        cout << RED << "  Slot Already Taken! Choose another." << RESET << endl;
                    }
                } while (!validMove);
            } else {
                ComputerMove();
            }

            DrawBoard();

            if (CheckWin()) {
                if (currentPlayer == 1) {
                    playerScore++;
                    cout << GREEN << "      >> YOU WON ROUND " << roundNumber << "! <<" << RESET << endl;
                } else {
                    computerScore++;
                    cout << RED << "      >> COMPUTER WON ROUND " << roundNumber << "! <<" << RESET << endl;
                }
                roundOver = true;
            } else if (i == 8) {
                cout << YELLOW << "      >> ROUND " << roundNumber << " IS A TIE! <<" << RESET << endl;
            }

            if (!roundOver) {
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                currentMarker = (currentMarker == 'X') ? 'O' : 'X';
            }
        }

        cout << endl;
        cout << CYAN << "      ----- Series Score -----" << RESET << endl;
        cout << "      " << GREEN << "You: " << playerScore << RESET
             << "  |  " << RED << "Computer: " << computerScore << RESET << endl;
        cout << CYAN << "      -------------------------" << RESET << endl;

        humanStartsNext = !humanStartsNext;

        if (playerScore < targetWins && computerScore < targetWins) {
            cout << endl << "  Press any key to continue to Round " << (roundNumber + 1) << "...";
            system("pause > nul");
        }
    }

    cout << endl;
    if (playerScore >= targetWins) {
        cout << GREEN;
        cout << "      ============================" << endl;
        cout << "        YOU WON THE SERIES!" << endl;
        cout << "      ============================" << RESET << endl;
    } else {
        cout << RED;
        cout << "      ============================" << endl;
        cout << "        COMPUTER WINS THE SERIES!" << endl;
        cout << "      ============================" << RESET << endl;
    }
    cout << endl;
    cout << "      Final Score: " << GREEN << "You " << playerScore << RESET
         << " - " << RED << computerScore << " Computer" << RESET << endl;
    cout << endl;

    return 0;
}

void ResetBoard() {
    int num = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = '0' + num;
            num++;
        }
    }
}

int MinMax(int depth, bool isMax, char aiMarker, char playerMarker) {
    int score = EvaluateBoard(aiMarker, playerMarker);
    if (score != 0) return score;
    if(!IsMovesLeft())
        return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    char temp = board[i][j];
                    board[i][j] = aiMarker;
                    best = max(best, MinMax(depth + 1, false, aiMarker, playerMarker));
                    board[i][j] = temp;
                }
            }
        }

        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != 'X' && board[i][j] != 'O') {
                    char temp = board[i][j];
                    board[i][j] = playerMarker;
                    best = min(best, MinMax(depth + 1, true, aiMarker, playerMarker));
                    board[i][j] = temp;
                }
            }
        }
        return best;
    }
}

bool IsMovesLeft() {
    for (vector<char> row : board) {
        for (char cell : row) {
            if (cell != 'X' && cell != 'O')
                return true;
        }
    }
    return false;
}

int EvaluateBoard(char aiMarker, char playerMarker) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == aiMarker) return 10;
            if (board[i][0] == playerMarker) return -10;
        } 
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == aiMarker) return 10;
            if (board[0][i] == playerMarker) return -10;
        }
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[1][1] == aiMarker) return 10;
        if (board[1][1] == playerMarker) return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[1][1] == aiMarker) return 10;
        if (board[1][1] == playerMarker) return -10;
    }
    return 0;
}

bool FindBestMove(char markerToLookFor) {
    for (int i = 0; i < 3; i++) 
        if (CheckLine(i, 0, i, 1, i, 2, markerToLookFor)) return true;

    for (int i = 0; i < 3; i++) 
        if (CheckLine(0, i, 1, i, 2, i, markerToLookFor)) return true;

    if (CheckLine(0, 0, 1, 1, 2, 2, markerToLookFor)) return true;
    if (CheckLine(0, 2, 1, 1, 2, 0, markerToLookFor)) return true;

    return false;
}

bool CheckLine(int r1, int c1, int r2, int c2, int r3, int c3, char marker) {
    int count = 0;
    int emptyRow = -1, emptyColumn = -1;

    int rows[] = {r1, r2, r3};
    int cols[] = {c1, c2, c3};

    for (int i = 0; i < 3; i++) {
        char cell = board[rows[i]][cols[i]];
        if (cell == marker) count++;
        else if (cell != 'X' && cell != 'O') {
            emptyRow = rows[i];
            emptyColumn = cols[i];
        }
    }

    if (count == 2 && emptyRow != -1) {
        board[emptyRow][emptyColumn] = currentMarker;
        return true;
    }
    return false;
}

void ComputerMove() {
    Sleep(1000);

    int bestVal = -1000;
    int bestRow = -1;
    int bestCol = -1;
    char opponentMarker = (currentMarker == 'X') ? 'O' : 'X';
    char temp;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                temp = board[i][j];
                board[i][j] = currentMarker;
                int moveVal = MinMax(0, false, currentMarker, opponentMarker);
                board[i][j] = temp;
                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    board[bestRow][bestCol] = currentMarker;
}

string GetColoredMarker(char marker) {
    if (marker == 'X') return RED + "X" + RESET;
    if (marker == 'O') return BLUE + "O" + RESET;
    return string(1, marker); 
}

bool CheckWin() {
    
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true;
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return true;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return true;
    return false;
}

void DrawBoard() {
    system("cls");
    
    cout << YELLOW << "\n";
    cout << "      ***********************" << endl;
    cout << "      * TIC-TAC-TOE PRO   *" << endl;
    cout << "      ***********************" << RESET << endl;

    // Round & score info
    if (roundNumber > 0) {
        cout << CYAN << "      Round " << roundNumber << " | Best of " << bestOf << RESET << endl;
        cout << "      You (" << GetColoredMarker(playerChoice) << "): " << playerScore
             << "  |  Computer (" << GetColoredMarker(computerChoice) << "): " << computerScore << endl;
    }
    cout << "\n";

    cout << "           |     |     " << endl;
    cout << "        " << GetColoredMarker(board[0][0]) << "  |  " << GetColoredMarker(board[0][1]) << "  |  " << GetColoredMarker(board[0][2]) << "  " << endl;
    cout << "      _____|_____|_____" << endl;
    cout << "           |     |     " << endl;
    cout << "        " << GetColoredMarker(board[1][0]) << "  |  " << GetColoredMarker(board[1][1]) << "  |  " << GetColoredMarker(board[1][2]) << "  " << endl;
    cout << "      _____|_____|_____" << endl;
    cout << "           |     |     " << endl;
    cout << "        " << GetColoredMarker(board[2][0]) << "  |  " << GetColoredMarker(board[2][1]) << "  |  " << GetColoredMarker(board[2][2]) << "  " << endl;
    cout << "           |     |     " << endl;
    cout << "\n";
}