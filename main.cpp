#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

/*
    Tetris Game
    Author: Allah Ditta
    Institution: Namal University Mianwali
    Copyright (c) 2026 Allah Ditta
    All rights reserved.
*/

const int boardRows = 25;
const int boardCols = 30;
const int shapeSize = 4;
char gameBoard[boardRows][boardCols];
int score = 0;

char a = 219;
void gotoRowCol(int rpos, int cpos);
void sleep(int m);
void initializeBoard();
void drawBoard();
bool isColumnFilled();
void copyShape(char source[shapeSize][shapeSize], char target[shapeSize][shapeSize]);
void printZShape(char shape[shapeSize][shapeSize]);
void printTShape(char shape[shapeSize][shapeSize]);
void printIShape(char shape[shapeSize][shapeSize]);
void printOShape(char shape[shapeSize][shapeSize]);
void printSShape(char shape[shapeSize][shapeSize]);
void printLShape(char shape[shapeSize][shapeSize]);
void placeShape(char shape[shapeSize][shapeSize], int rowPos, int colPos);
bool canMove(char shape[shapeSize][shapeSize], int rowPos, int colPos);
void rotateShape(char shape[shapeSize][shapeSize]);
void generateAndMoveShape();
void saveGameState(const string& filename);
bool loadGameState(const string& filename);
bool fileExists(const string& filename);
void resetGameState();

void gotoRowCol(int rpos, int cpos) {
    COORD scrn;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOutput, scrn);
}

void sleep(int m) {
    for (int j = 0; j < m * 21000; j++) {}
}

void initializeBoard() {
    for (int i = 0; i < boardRows; i++) {
        for (int j = 0; j < boardCols; j++) {
            gameBoard[i][j] = ' ';
        }
    }
}

void drawBoard() {
    for (int i = 0; i < boardRows; i++) {
        for (int j = 0; j < boardCols; j++) {
            gotoRowCol(i, j);
            if (gameBoard[i][j] != ' ') {
                cout << gameBoard[i][j];
            } else {
                cout << ":";
            }
        }
    }
    gotoRowCol(0, boardCols + 2);
    cout << "Score: " << score++ << endl;
    gotoRowCol(1, 40);
    cout << " A for left movement ,D for right , W for rotating shapes" << endl;
}

bool isColumnFilled() {
    for (int col = 0; col < boardCols; col++) {
        if (gameBoard[0][col] != ' ') {
            return true;
        }
    }
    return false;
}

void copyShape(char source[shapeSize][shapeSize], char target[shapeSize][shapeSize]) {
    for (int i = 0; i < shapeSize; i++) {
        for (int j = 0; j < shapeSize; j++) {
            target[i][j] = source[i][j];
        }
    }
}

void printZShape(char shape[shapeSize][shapeSize]) {
    char temp[shapeSize][shapeSize] = {
        {' ', a, a, ' '},
        {a, a, ' ', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    };
    copyShape(temp, shape);
}

void printTShape(char shape[shapeSize][shapeSize]) {
    char temp[shapeSize][shapeSize] = {
        {' ', a, ' ', ' '},
        {a, a, a, ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    };
    copyShape(temp, shape);
}

void printIShape(char shape[shapeSize][shapeSize]) {
    char temp[shapeSize][shapeSize] = {
        {' ', a, ' ', ' '},
        {' ', a, ' ', ' '},
        {' ', a, ' ', ' '},
        {' ', a, ' ', ' '}
    };
    copyShape(temp, shape);
}

void printOShape(char shape[shapeSize][shapeSize]) {
    char temp[shapeSize][shapeSize] = {
        {a, a, ' ', ' '},
        {a, a, ' ', ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    };
    copyShape(temp, shape);
}

void printSShape(char shape[shapeSize][shapeSize]) {
    char temp[shapeSize][shapeSize] = {
        {a, a, ' ', ' '},
        {' ', a, a, ' '},
        {' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' '}
    };
    copyShape(temp, shape);
}

void printLShape(char shape[shapeSize][shapeSize]) {
    char temp[shapeSize][shapeSize] = {
        {' ', a, ' ', ' '},
        {' ', a, ' ', ' '},
        {' ', a, a, ' '},
        {' ', ' ', ' ', ' '}
    };
    copyShape(temp, shape);
}

void placeShape(char shape[shapeSize][shapeSize], int rowPos, int colPos) {
    for (int i = 0; i < shapeSize; i++) {
        for (int j = 0; j < shapeSize; j++) {
            if (shape[i][j] != ' ') {
                gameBoard[rowPos + i][colPos + j] = shape[i][j];
            }
        }
    }
}

bool canMove(char shape[shapeSize][shapeSize], int rowPos, int colPos) {
    for (int i = 0; i < shapeSize; i++) {
        for (int j = 0; j < shapeSize; j++) {
            if (shape[i][j] != ' ') {
                int newRow = rowPos + i;
                int newCol = colPos + j;

                if (newRow >= boardRows || newCol < 0 || newCol >= boardCols || gameBoard[newRow][newCol] != ' ') {
                    return false;
                }
            }
        }
    }
    return true;
}

void rotateShape(char shape[shapeSize][shapeSize]) {
    char temp[shapeSize][shapeSize];
    for (int i = 0; i < shapeSize; i++) {
        for (int j = 0; j < shapeSize; j++) {
            temp[j][shapeSize - 1 - i] = shape[i][j];
        }
    }
    copyShape(temp, shape);
}

void generateAndMoveShape() {
    char currentShape[shapeSize][shapeSize] = {};
    int ran = rand() % 6;

    switch (ran) {
    case 0:
        printZShape(currentShape);
        break;
    case 1:
        printTShape(currentShape);
        break;
    case 2:
        printIShape(currentShape);
        break;
    case 3:
        printOShape(currentShape);
        break;
    case 4:
        printSShape(currentShape);
        break;
    case 5:
        printLShape(currentShape);
        break;
    }

    int rowPos = 0, colPos = (boardCols - shapeSize) / 2;

    while (true) {
        if (isColumnFilled()) {
            system("CLS");
            drawBoard();
            gotoRowCol(boardRows / 2, boardCols + 5);
            cout << "GAME OVER!";
            saveGameState("ad.txt");
            exit(0);
        }

        system("cls");
        drawBoard();

        for (int i = 0; i < shapeSize; i++) {
            for (int j = 0; j < shapeSize; j++) {
                if (currentShape[i][j] != ' ') {
                    gotoRowCol(rowPos + i, colPos + j);
                    cout << currentShape[i][j];
                }
            }
        }

        if (_kbhit()) {
            char ch = _getch();
            if ((ch == 'a' || ch == 'A') && canMove(currentShape, rowPos, colPos - 1)) {
                colPos--;
            } else if ((ch == 'd' || ch == 'D') && canMove(currentShape, rowPos, colPos + 1)) {
                colPos++;
            } else if (ch == 'w' || ch == 'W') {
                rotateShape(currentShape);
            } else if ((ch == 's' || ch == 'S') && canMove(currentShape, rowPos + 1, colPos)) {
                rowPos++;
            }
        }

        sleep(10000);

        if (canMove(currentShape, rowPos + 1, colPos)) {
            rowPos++;
        } else {
            placeShape(currentShape, rowPos, colPos);
            break;
        }
    }
}

void saveGameState(const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << score << endl;
        for (int i = 0; i < boardRows; i++) {
            for (int j = 0; j < boardCols; j++) {
                outFile << gameBoard[i][j];
            }
            outFile << endl;
        }
        outFile.close();
    } else {
        cout << "Unable to open file for saving." << endl;
    }
}

bool loadGameState(const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        inFile >> score;
        inFile.ignore();
        for (int i = 0; i < boardRows; i++) {
            for (int j = 0; j < boardCols; j++) {
                inFile.get(gameBoard[i][j]);
            }
            inFile.ignore();
        }
        inFile.close();
        return true;
    } else {
        cout << "Unable to open file for loading." << endl;
        return false;
    }
}

bool fileExists(const string& filename) {
    ifstream file(filename);
    return file.good();
}

void resetGameState() {
    score = 0;
    initializeBoard();
}

int main() {
    if (fileExists("ad.txt")) {
        char choice;
        cout << "Would you like to continue from your previous game? (y/n): ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            if (!loadGameState("ad.txt")) {
                cout << "Failed to load the game. Starting a new game." << endl;
                resetGameState();
            }
        } else {
            resetGameState();
            saveGameState("ad.txt");
        }
    } else {
        resetGameState();
        saveGameState("ad.txt");
    }

    while (true) {
        generateAndMoveShape();
    }

    return 0;
}
