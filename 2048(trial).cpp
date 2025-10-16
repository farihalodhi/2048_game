#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include<windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // this handle acts as a refernece to your console window
void setColor(int value){
	switch(value){
			case 2:
		SetConsoleTextAttribute(hConsole,2);
		break;
	case 4:
		SetConsoleTextAttribute(hConsole,3);
		break;
	case 8:
		SetConsoleTextAttribute(hConsole,6);
		break;
	case 16:
		SetConsoleTextAttribute(hConsole,9);
		break;
	case 32:
		SetConsoleTextAttribute(hConsole,12);
		break;
	case 64:
		SetConsoleTextAttribute(hConsole,13);
		break;
	case 128:
		SetConsoleTextAttribute(hConsole,14);
		break;
	case 2048:
	SetConsoleTextAttribute(hConsole,15);		
		break;
	default:
	SetConsoleTextAttribute(hConsole,7);	
	}
		
}
void InitializeBoard(int board[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            board[i][j] = 0;
        }
    }
}

void addRandomTile(int board[4][4]) {
    int row, column;
    do {
        row = rand() % 4;
        column = rand() % 4;
    } while(board[row][column] != 0);

    int newTile = (rand() % 10 == 0) ? 4 : 2;
    board[row][column] = newTile;
}

void displayBoard(int board[4][4]) {
    cout << "\n\t\t\t\t\t2048 BOARD GAME\n";
    cout << "\t\t\t\t---------------------------------\n\n";

    for(int i = 0; i < 4; i++) {
        cout << "\t\t";
        for(int j = 0; j < 4; j++) {
            if(board[i][j] == 0){
            	          SetConsoleTextAttribute(hConsole,7);
                cout << "|     ";
			}
  
            else{
            	setColor(board[i][j]);
                cout << "| " << setw(4) << board[i][j];
			}
        }
        cout << "|\n\t\t--------------------------\n";
    }
}
void moveLeft(int board[4][4]){
	for(int i=0;i<4;i++){
		int temp[4] = {0};
		int idx=0; // index tracking
		for(int j=0;j<4;j++){
			if(board[i][j]!=0){
			temp[idx++] = board[i][j];
		}
		}
		for(int j=0;j<3;j++){
			if(temp[j]!=0 && temp[j]==temp[j+1]){
				temp[j]+=temp[j+1];
				temp[j+1] =0;
			}
		}
		   int finalRow[4] = {0};
        idx = 0;
        for (int j = 0; j < 4; j++) {
            if (temp[j] != 0) {
                finalRow[idx++] = temp[j];
            }
        }
           for (int j = 0; j < 4; j++) {
            board[i][j] = finalRow[j];
        }
	}
}
int main() {
    srand(time(0));
    int board[4][4];
    InitializeBoard(board);
    addRandomTile(board);
    addRandomTile(board);
    displayBoard(board);
    moveLeft(board);
    cout<<"\nafter moving left"<<endl;
    displayBoard(board);
    return 0;
}

