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
const string DIM    = "\033[2m";

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
void ComputerMove();
void DrawBoard();
void ResetBoard();
bool CheckWin();
bool IsDrawInevitable();
string GetColoredMarker(char marker);

/**
 * @brief Entry point for Tic-Tac-Toe Pro (AI Edition).
 * 
 * Handles the full game lifecycle: marker selection, series format
 * selection (Best of 3 or 5), and the series loop with alternating
 * starters and round-capped scoring.
 * 
 * @return int Exit code (0 on success).
 */
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

    while (playerScore < targetWins && computerScore < targetWins && roundNumber < bestOf) {
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
            } else if (IsDrawInevitable()) {
                cout << YELLOW << "      >> ROUND " << roundNumber << " IS A TIE! <<" << RESET << endl;
                cout << DIM << "      No winning moves remaining." << RESET << endl;
                roundOver = true;
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

        bool seriesClinched = (playerScore >= targetWins || computerScore >= targetWins);
        bool allRoundsPlayed = (roundNumber >= bestOf);

        if (!seriesClinched && !allRoundsPlayed) {
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
    } else if (computerScore >= targetWins) {
        cout << RED;
        cout << "      ============================" << endl;
        cout << "        COMPUTER WINS THE SERIES!" << endl;
        cout << "      ============================" << RESET << endl;
    } else if (playerScore > computerScore) {
        cout << GREEN;
        cout << "      ============================" << endl;
        cout << "        YOU WIN ON POINTS!" << endl;
        cout << "      ============================" << RESET << endl;
    } else if (computerScore > playerScore) {
        cout << RED;
        cout << "      ============================" << endl;
        cout << "        COMPUTER WINS ON POINTS!" << endl;
        cout << "      ============================" << RESET << endl;
    } else {
        cout << YELLOW;
        cout << "      ============================" << endl;
        cout << "        SERIES ENDS IN A DRAW!" << endl;
        cout << "      ============================" << RESET << endl;
    }
    cout << endl;
    cout << "      Final Score: " << GREEN << "You " << playerScore << RESET
         << " - " << RED << computerScore << " Computer" << RESET << endl;
    cout << endl;

    return 0;
}

/**
 * @brief Resets the board to its initial numbered state ('1' through '9').
 * 
 * Called at the start of each new round in a series to clear all
 * previously placed markers.
 */
void ResetBoard() {
    int num = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = '0' + num;
            num++;
        }
    }
}

/**
 * @brief Implements the MinMax algorithm for optimal AI decision-making.
 * 
 * Recursively evaluates all possible game states to determine the
 * best possible outcome for the AI. Uses alternating maximizing and
 * minimizing layers to simulate both players playing optimally.
 * 
 * @param depth    Current recursion depth (0 at the root call).
 * @param isMax    True if the current layer is maximizing (AI's turn).
 * @param aiMarker The marker character used by the AI ('X' or 'O').
 * @param playerMarker The marker character used by the human player.
 * @return int Score: +10 for AI win, -10 for player win, 0 for draw.
 */
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

/**
 * @brief Checks whether any empty slots remain on the board.
 * 
 * A cell is considered empty if it does not contain 'X' or 'O'
 * (i.e., it still holds its original number character).
 * 
 * @return true  If at least one playable slot remains.
 * @return false If the board is completely filled.
 */
bool IsMovesLeft() {
    for (vector<char> row : board) {
        for (char cell : row) {
            if (cell != 'X' && cell != 'O')
                return true;
        }
    }
    return false;
}

/**
 * @brief Evaluates the current board state and returns a heuristic score.
 * 
 * Scans all rows, columns, and diagonals for three-in-a-row patterns.
 * Used exclusively by the MinMax algorithm to score terminal game states.
 * 
 * @param aiMarker     The AI's marker character.
 * @param playerMarker The human player's marker character.
 * @return int +10 if AI wins, -10 if player wins, 0 if no winner yet.
 */
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

/**
 * @brief Executes the computer's turn using the MinMax algorithm.
 * 
 * Evaluates every available move via MinMax, collects all moves that
 * share the highest score, and randomly selects one. This ensures
 * optimal play while adding variety to the computer's openings.
 * 
 * Includes a 1-second delay (Sleep) to simulate "thinking" for
 * a better user experience.
 */
void ComputerMove() {
    Sleep(1000);

    int bestVal = -1000;
    char opponentMarker = (currentMarker == 'X') ? 'O' : 'X';
    vector<pair<int,int>> bestMoves;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 'X' && board[i][j] != 'O') {
                char temp = board[i][j];
                board[i][j] = currentMarker;
                
                int moveVal = MinMax(0, false, currentMarker, opponentMarker);
                
                board[i][j] = temp;

                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    bestMoves.clear(); // Found a NEW best, throw away the old mediocre ones
                    bestMoves.push_back({i, j});
                } else if (moveVal == bestVal) {
                    bestMoves.push_back({i, j}); // Another move that is equally good
                }
            }
        }
    }

    if (!bestMoves.empty()) {
        int pick = rand() % bestMoves.size();
        board[bestMoves[pick].first][bestMoves[pick].second] = currentMarker;
    }
}

/**
 * @brief Returns a color-coded string representation of a board marker.
 * 
 * 'X' is rendered in Red, 'O' in Blue. Number characters (empty slots)
 * are returned without color formatting.
 * 
 * @param marker The character to colorize ('X', 'O', or '1'-'9').
 * @return string The ANSI-colored string for terminal output.
 */
string GetColoredMarker(char marker) {
    if (marker == 'X') return RED + "X" + RESET;
    if (marker == 'O') return BLUE + "O" + RESET;
    return string(1, marker); 
}

/**
 * @brief Checks if the current player has achieved three-in-a-row.
 * 
 * Scans all rows, columns, and both diagonals of the board.
 * Used by the main game loop to determine if a round has been won.
 * 
 * @return true  If any row, column, or diagonal has three matching markers.
 * @return false If no winning condition is met.
 */
bool CheckWin() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) return true;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) return true;
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) return true;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) return true;
    return false;
}

/**
 * @brief Detects if a draw is inevitable regardless of remaining moves.
 * 
 * Tests every empty cell by temporarily placing both 'X' and 'O'.
 * If no placement can produce three-in-a-row for either marker,
 * the game is a guaranteed draw and can end early.
 * 
 * @return true  If no remaining move can win for either player.
 * @return false If at least one winning move still exists.
 */
bool IsDrawInevitable() {
    int lines[8][3][2] = {
        {{0,0}, {0,1}, {0,2}},
        {{1,0}, {1,1}, {1,2}},
        {{2,0}, {2,1}, {2,2}},
        {{0,0}, {1,0}, {2,0}},
        {{0,1}, {1,1}, {2,1}},
        {{0,2}, {1,2}, {2,2}},
        {{0,0}, {1,1}, {2,2}},
        {{0,2}, {1,1}, {2,0}}
    };

    for (int l = 0; l < 8; l++) {
        bool hasX = false, hasO = false;
        for (int i = 0; i < 3; i++) {
            char cell = board[lines[l][i][0]][lines[l][i][1]];
            if (cell == 'X') hasX = true;
            if (cell == 'O') hasO = true;
        }
        if (!(hasX && hasO)) return false;
    }
    return true;
}

/**
 * @brief Clears the screen and renders the game board with the UI header.
 * 
 * Displays the game title, current round number, series format,
 * live score for both players (with colored markers), and the
 * 3x3 board grid with ANSI-colored markers.
 */
void DrawBoard() {
    system("cls");
    
    cout << YELLOW << "\n";
    cout << "      ***********************" << endl;
    cout << "      * TIC-TAC-TOE PRO   *" << endl;
    cout << "      ***********************" << RESET << endl;

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