#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <conio.h>
#include <windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void setColor(int value) {
    switch (value) {
        case 2:  SetConsoleTextAttribute(hConsole, 2);  break;
        case 4:  SetConsoleTextAttribute(hConsole, 3);  break;
        case 8:  SetConsoleTextAttribute(hConsole, 6);  break;
        case 16: SetConsoleTextAttribute(hConsole, 9);  break;
        case 32: SetConsoleTextAttribute(hConsole, 12); break;
        case 64: SetConsoleTextAttribute(hConsole, 13); break;
        case 128:SetConsoleTextAttribute(hConsole, 14); break;
        case 2048:SetConsoleTextAttribute(hConsole, 15); break;
        default: SetConsoleTextAttribute(hConsole, 7);
    }
}

void InitializeBoard(int board[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            board[i][j] = 0;
}

void addRandomTile(int board[4][4]) {
    int empty[16][2];
    int count = 0;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (board[i][j] == 0) {
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }

    if (count > 0) {
        int idx = rand() % count;
        int val = (rand() % 10 == 0) ? 4 : 2;
        board[empty[idx][0]][empty[idx][1]] = val;
    }
}

void displayBoard(int board[4][4]) {
    system("cls");
    cout << "\n\t\t\t\t\t2048 BOARD GAME\n";
    cout << "\t\t\t\t---------------------------------\n\n";

    for (int i = 0; i < 4; i++) {
        cout << "\t\t";
        for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                SetConsoleTextAttribute(hConsole, 7);
                cout << "|     ";
            } else {
                setColor(board[i][j]);
                cout << "| " << setw(4) << board[i][j];
            }
        }
        cout << "|\n\t\t--------------------------\n";
    }
    SetConsoleTextAttribute(hConsole, 7);
    cout << "\n\t\tUse W/A/S/D to move, Q to quit.\n";
}

bool moveLeft(int board[4][4]) {
    bool moved = false;
    for (int i = 0; i < 4; i++) {
        int temp[4] = {0}, idx = 0;
        for (int j = 0; j < 4; j++)
            if (board[i][j] != 0) temp[idx++] = board[i][j];

        for (int j = 0; j < 3; j++) {
            if (temp[j] != 0 && temp[j] == temp[j + 1]) {
                temp[j] *= 2;
                temp[j + 1] = 0;
                moved = true;
            }
        }

        int finalRow[4] = {0};
        idx = 0;
        for (int j = 0; j < 4; j++)
            if (temp[j] != 0) finalRow[idx++] = temp[j];

        for (int j = 0; j < 4; j++) {
            if (board[i][j] != finalRow[j]) moved = true;
            board[i][j] = finalRow[j];
        }
    }
    return moved;
}

bool moveRight(int board[4][4]) {
    bool moved = false;
    for (int i = 0; i < 4; i++) {
        int temp[4] = {0}, idx = 3;
        for (int j = 3; j >= 0; j--)
            if (board[i][j] != 0) temp[idx--] = board[i][j];

        for (int j = 3; j > 0; j--) {
            if (temp[j] != 0 && temp[j] == temp[j - 1]) {
                temp[j] *= 2;
                temp[j - 1] = 0;
                moved = true;
            }
        }

        int finalRow[4] = {0};
        idx = 3;
        for (int j = 3; j >= 0; j--)
            if (temp[j] != 0) finalRow[idx--] = temp[j];

        for (int j = 0; j < 4; j++) {
            if (board[i][j] != finalRow[j]) moved = true;
            board[i][j] = finalRow[j];
        }
    }
    return moved;
}

bool moveUp(int board[4][4]) {
    bool moved = false;
    for (int j = 0; j < 4; j++) {
        int temp[4] = {0}, idx = 0;
        for (int i = 0; i < 4; i++)
            if (board[i][j] != 0) temp[idx++] = board[i][j];

        for (int i = 0; i < 3; i++) {
            if (temp[i] != 0 && temp[i] == temp[i + 1]) {
                temp[i] *= 2;
                temp[i + 1] = 0;
                moved = true;
            }
        }

        int finalCol[4] = {0};
        idx = 0;
        for (int i = 0; i < 4; i++)
            if (temp[i] != 0) finalCol[idx++] = temp[i];

        for (int i = 0; i < 4; i++) {
            if (board[i][j] != finalCol[i]) moved = true;
            board[i][j] = finalCol[i];
        }
    }
    return moved;
}

bool moveDown(int board[4][4]) {
    bool moved = false;
    for (int j = 0; j < 4; j++) {
        int temp[4] = {0}, idx = 3;
        for (int i = 3; i >= 0; i--)
            if (board[i][j] != 0) temp[idx--] = board[i][j];

        for (int i = 3; i > 0; i--) {
            if (temp[i] != 0 && temp[i] == temp[i - 1]) {
                temp[i] *= 2;
                temp[i - 1] = 0;
                moved = true;
            }
        }

        int finalCol[4] = {0};
        idx = 3;
        for (int i = 3; i >= 0; i--)
            if (temp[i] != 0) finalCol[idx--] = temp[i];

        for (int i = 0; i < 4; i++) {
            if (board[i][j] != finalCol[i]) moved = true;
            board[i][j] = finalCol[i];
        }
    }
    return moved;
}

bool isGameOver(int board[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (board[i][j] == 0) return false;

    int temp[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            temp[i][j] = board[i][j];

    if (moveLeft(temp) || moveRight(temp) || moveUp(temp) || moveDown(temp))
        return false;
    return true;
}

int main() {
    srand(time(0));
    int board[4][4];
    InitializeBoard(board);
    addRandomTile(board);
    addRandomTile(board);

    bool running = true;
    while (running) {
        displayBoard(board);
        char ch = getch();

        bool moved = false;
        switch (ch) {
            case 'a': case 'A': moved = moveLeft(board); break;
            case 'd': case 'D': moved = moveRight(board); break;
            case 'w': case 'W': moved = moveUp(board); break;
            case 's': case 'S': moved = moveDown(board); break;
            case 'q': case 'Q': running = false; break;
        }

        if (moved)
            addRandomTile(board);

        if (isGameOver(board)) {
            displayBoard(board);
            cout << "\n\t\tGAME OVER! No more moves possible.\n";
            running = false;
        }
    }

    return 0;
}

