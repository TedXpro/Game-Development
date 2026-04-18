#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>
#include <limits>

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

void DrawBoard();
bool CheckWin();
string GetColoredMarker(char marker);

int main() {
    
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