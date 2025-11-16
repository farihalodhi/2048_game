#include<iostream>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include<iomanip>
#include<stack>
#include<windows.h>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int board[4][4];
int previousBoard[4][4];
int score = 0;

class BoardState{  //made class for board because stack directly arrays store nhi karta in cpp
	public:
		int arr[4][4];
		int score;
		BoardState(){
		}
		BoardState(int board[4][4],int sc){
			for(int i = 0 ; i < 4 ; i++){
				for (int  j = 0 ; j < 4 ;j++){
					arr[i][j] = board[i][j];
				}
			}
			score = sc;
		}
};
stack<BoardState> history;

void setColor(int value){
	switch(value) {
		case 2: 
			SetConsoleTextAttribute(hConsole, 2);
			break;
		case 4:
			SetConsoleTextAttribute(hConsole, 3);
			break;
		case 8:
			SetConsoleTextAttribute(hConsole, 6);
			break;
		case 16:
			SetConsoleTextAttribute(hConsole, 9);
			break;
		case 32:
			SetConsoleTextAttribute(hConsole, 12);
			break;	
		case 64:
			SetConsoleTextAttribute(hConsole, 13);
			break;	
		case 128:
			SetConsoleTextAttribute(hConsole, 14);
			break;
		case 256:
			SetConsoleTextAttribute(hConsole, 11);
			break;	
		case 512:
			SetConsoleTextAttribute(hConsole, 10);
			break;
		case 1024:
			SetConsoleTextAttribute(hConsole, 14);
			break;								
		case 2048:
			SetConsoleTextAttribute(hConsole, 15);
			break;	
		default:
			SetConsoleTextAttribute(hConsole, 7);								
	}
}

void saveState(int board[4][4] , int score){
	BoardState state(board,score);
	history.push(state);
}
bool UndoMove(int board[4][4] , int &score){
	if(history.empty()){
        SetConsoleTextAttribute(hConsole, 12);		
		cout<<"\nNo moves to undo\n";
        SetConsoleTextAttribute(hConsole, 7);		
		return false;
	}
	BoardState last = history.top(); //jo bhi last move hoga will be stored in this
	history.pop();
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ;j++){
			 board[i][j] = last.arr[i][j];
		}
	}
	score = last.score;
	SetConsoleTextAttribute(hConsole, 10);
	cout<<"\nMove undone.\n";
    SetConsoleTextAttribute(hConsole, 7);	
	return true;
}
void InitializeBoard(int board[4][4]){
	for(int i =0 ; i < 4 ; i++){
		for(int j=0 ; j < 4 ; j++){
			board[i][j] = 0 ;
		}
	}
}
void PrintBoard(int board[4][4],int score){
	system("cls");
	
    SetConsoleTextAttribute(hConsole, 11);
    
	cout<<"\t\t\t\t  SCORE: "<<score<<"\n\n";
    SetConsoleTextAttribute(hConsole, 7);
			
	for(int i = 0 ; i < 4 ; i++){
		cout<<"\t\t";
		
		for(int  j = 0; j < 4 ; j++){
			cout<<"| ";
			int value = board[i][j];
			setColor(value);
			if(value == 0){
				cout<< setw(4) << " 0";
			} else {
				cout << setw(4) << value;
			}

		SetConsoleTextAttribute(hConsole, 7);
		}
	cout << "|\n\t\t-------------------------\n";
	}
	cout<<"\nControls: A = LEFT , D = RIGHT , W = UP , S = DOWN, U = UNDO , Q = QUIT\n";
    SetConsoleTextAttribute(hConsole, 7);	
//	cout<<"\nUse W for move Up\nUse A to move down\nUse S to move left\nUse D to move right\nUse Q to quit\n";
}
void addRandomTile(int board[4][4]){
	int emptyCells[16][2];
	int count = 0; // keep total count of empty cells
	
	//finding empty cells
	for(int i = 0 ; i < 4 ;i++){
		for(int  j = 0 ; j < 4 ; j++){
			if(board[i][j] == 0){
				emptyCells[count][0] = i; //store row 
				emptyCells[count][1] = j; // store column
				count++;
			}
		}
	}
	if(count == 0){ //no empty cell
		return;
	}
	int index = rand() % count ; //choose random empty cell
	int r = rand() % 10; //generates random value btw 10
	int value;
	if(r == 0){
		value = 4;
	} else {
		value = 2;
	}
	int x = emptyCells [index][0];
	int y = emptyCells [index][1];
	board[x][y]= value;	
}
bool moveLeft(int board[4][4], int &score){
	bool moved = false; 
	
	for(int i = 0 ;i<4 ; i++){
		//COMPRESS IF 0
		int temp[4] = {0};
		int index = 0; 
		for(int j = 0 ; j < 4 ; j++){
			if(board[i][j] != 0){
				temp[index] = board[i][j];
				index++;
			}
		}
		//MERGE combine same number
		for(int j = 0 ; j < 3 ; j++){  //j till 3 bcs comparing with j+1 so out of bound chala jaye ga if j > 4
			if(temp[j] != 0 && temp[j] == temp[j+1]){
				temp[j] += temp[j+1];
				score += temp[j];
				temp[j+1] = 0;
				moved = true;
			}
		}
		//	COMPRESSS AGAIN taake spaces hat jayen
		int newRow[4] = {0};
		index = 0;
		for(int j = 0 ; j < 4; j++){
			if(temp[j] != 0){
				newRow[index] = temp[j];
				index++;
			}
		}
		//SAVE TO ORIGINAL BOARD
		for(int j = 0 ; j < 4 ; j++){
			if(board[i][j] != newRow[j]){
				board[i][j] = newRow[j];
				moved = true;
			}
		}
	}
	return moved;
}

bool moveRight(int board[4][4] ,int &score){
	bool moved = false;
	for(int i = 0; i < 4; i++){
		int temp[4] = {0}; 
		int index = 3; // starts with rightmostt
		for(int j = 3 ; j>=0 ; j--){
			if(board[i][j] != 0){
				temp[index] = board[i][j];
				index--;
			}
		}
		
		for(int j = 3 ; j > 0 ; j--){
			if(temp[j] != 0 && temp[j] == temp[j-1]){
				temp[j] += temp[j-1];
				score += temp[j];
				temp[j-1] = 0;
				moved = true;
			}
		}
		int newRow[4] = {0};
		index = 3;
		for (int j = 3 ; j>=0 ; j--){
			if(temp[j] != 0){
				newRow[index] = temp[j];
				index--;
			}
		}
		for(int j = 0; j < 4;j++){
			if(board[i][j] != newRow[j]){
				board[i][j] = newRow[j];
				moved = true;
			}
		}
	}
		return moved;	
}
bool moveUp(int board[4][4], int &score){
	bool moved = false;
	for(int j = 0 ; j < 4 ; j++){
	 // for each column
		int temp[4] = {0};
		int index = 0; // top of column
		
		for(int i = 0; i < 4 ; i++){
			if(board[i][j] != 0){
				temp[index] = board[i][j];
				index++;
			}
		}
		for(int i = 0 ; i < 3 ; i++){
			if(temp[i] != 0 && temp[i] == temp[i+1]){
				temp[i] += temp[i+1];
				score += temp[i];
				temp[i+1] = 0;
				moved = true;
			}
		}
		int newCol[4] = {0};
		index = 0;
		for(int i = 0 ; i < 4 ; i++){
			if(temp[i]!=0){
				newCol[index] = temp[i];
				index++;
			}
		}
		for(int i = 0 ; i < 4 ; i++){
			if(board[i][j] != newCol[i]){
				board[i][j] = newCol[i];
				moved = true;
			}
		}
	}
	return moved;
}
bool moveDown(int board[4][4] , int &score){
	bool moved = false;
	for(int j = 0 ; j  < 4 ; j++){
		int temp[4] = {0};
		int index = 3;
		for(int i = 3 ; i>=0 ; i--){
			if(board[i][j]!=0){
				temp[index] = board[i][j];
				index--;
			}
		}
		for(int i = 3 ; i >= 0 ; i--){
			if(temp[i] != 0 && temp[i] == temp[i-1]){
				temp[i] += temp[i-1];
				score += temp[i];
				temp[i-1] = 0;
				moved = true;
			}
		}
		int newCol[4] = {0};
		index = 3;
		for(int i = 3 ; i >= 0 ;i--){
			if(temp[i]!=0){
				newCol[index] = temp[i];
				index--;
			}
		}
		for(int i = 0 ; i<4 ; i++){
			if(board[i][j] != newCol[i]){
				board[i][j] = newCol[i];
				moved = true;
			}
		}
	}
	return moved;	
}
bool isGameOver(int board[4][4]){
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			if(board[i][j] == 0){ // board is not full yet
				return false;
			}			
		}
	}
	int temp[4][4];
	int dummySc = 0;
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			temp[i][j] = board[i][j];
		}
	}
	if(moveLeft(temp,dummySc)){
		return false;
	}
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			temp[i][j] = board[i][j];
		}
	}
	if(moveRight(temp,dummySc)){
		return false;
	}	
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			temp[i][j] = board[i][j];
		}
	}
	if(moveUp(temp,dummySc)){
		return false;
	}	
	for(int i = 0 ; i < 4 ; i++){
	for(int j = 0 ; j < 4 ; j++){
		temp[i][j] = board[i][j];
	}
	if(moveDown(temp,dummySc)){
		return false;
	}
	return true;
}
	
}
int main(){
	cout<<"\t\t\t\t2048 GAME\n";
//	cout<<"Control: W = UP , A = LEFT , S = DOWN , D = RIGHT, U = UNDO , Q = QUIT";
	srand(time(0));
	
	
	InitializeBoard(board);
//	PrintBoard(board);
	addRandomTile(board);
	addRandomTile(board);
	addRandomTile(board);
	
	
	bool running = true;

	while(running){
		PrintBoard(board,score);
		
		cout<<"Enter move: \n";
		
		char opt = getch();	
		bool moved = false;
		
		if(opt == 'u' || opt == 'U'){ //u is for undo your previous move
			UndoMove(board,score);
			continue;
		}
		saveState(board,score);
		switch(opt){
			case 'a': case 'A':
				saveState(board,score);
				moved = moveLeft(board,score);
				break;
			case 'd': case 'D':
				saveState(board,score);				
				moved = moveRight(board,score);
				break;
			case 'w': case 'W':
				saveState(board,score);				
				moved = moveUp(board,score);
				break;
			case 's':case'S':
				saveState(board,score);				
				moved = moveDown(board,score);
				break;
			case 'q': case'Q':
				running = false;
				break;
			default:
				cout<<"\ninvalid move!";
				history.pop();
				continue;					
		}
			if(moved){
		addRandomTile(board);
	}		
	if(isGameOver(board)){
		PrintBoard(board,score);
        SetConsoleTextAttribute(hConsole, 12);		
		cout<<"\n\t\tGAME OVER! No more moves possible\n";
		cout<<"\t\tFinal Score: "<< score <<"\n";
        SetConsoleTextAttribute(hConsole, 7);		
		running = false;		
	}

	}

	return 0;
}
