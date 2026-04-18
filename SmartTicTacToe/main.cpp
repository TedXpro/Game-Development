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

int MinMax(int depth, bool isMax, char aiMarker, char playerMarker);
bool IsMovesLeft();
int EvaluateBoard(char aiMarker, char playerMarker);
bool CheckLine(int r1, int c1, int r2, int c2, int r3, int c3, char marker);
bool FindBestMove(char markerToLookFor);
void ComputerMove();
void DrawBoard();
bool CheckWin();
string GetColoredMarker(char marker);

int main() {
    srand(time(0));
    
    system("cls"); 

    cout << CYAN << "Player 1 choose your Marker (X, O): " << RESET;
    while (true) {
        if (cin >> currentMarker) {
            currentMarker = toupper(currentMarker);
            if (currentMarker == 'X' || currentMarker == 'O') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                break;
            }
        } else {
            cin.clear(); 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << RED << "Invalid Marker! Please choose X or O: " << RESET;
    }

    currentPlayer = 1;
    DrawBoard();

    for (int i = 0; i < 9; i++) {
        int slot;
        if (currentPlayer == 1) {
            bool validMove = false;
            
            do {
                cout << "It's player " << currentPlayer << "'s turn (" << GetColoredMarker(currentMarker) << "). Enter slot: ";
                
                if (!(cin >> slot)) {
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    cout << RED << "Invalid Input! Please enter a number 1-9." << RESET << endl;
                    continue;
                }

                if (slot < 1 || slot > 9) {
                    cout << RED << "Invalid Slot! Please choose 1-9." << RESET << endl;
                    continue;
                }
                
                int row = (slot - 1) / 3;
                int col = (slot - 1) % 3;
                
                if (board[row][col] != 'X' && board[row][col] != 'O'){
                    board[row][col] = currentMarker;
                    validMove = true;
                } else {
                    cout << RED << "Slot Already Taken! Please choose another." << RESET << endl;
                }
            } while (!validMove);
        } else {
            ComputerMove();
        }
        DrawBoard();

        if (CheckWin()) {
            cout << GREEN << "      ***********************" << endl;
            cout << "      * PLAYER " << currentPlayer << " WINS!    *" << endl;
            cout << "      ***********************" << RESET << endl;
            return 0;
        }

        
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        currentMarker = (currentMarker == 'X') ? 'O' : 'X';
    }

    cout << YELLOW << "It's a tie game!" << RESET << endl;
    return 0;
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