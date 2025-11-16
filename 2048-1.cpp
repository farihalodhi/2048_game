#include<iostream>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include<iomanip>
#include<stack>
using namespace std;

int board[4][4];
int previousBoard[4][4];

class BoardState{  //made class for board because stack directly arrays store nhi karta in cpp
	public:
		int arr[4][4];
		BoardState(){
		}
		BoardState(int board[4][4]){
			for(int i = 0 ; i < 4 ; i++){
				for (int  j = 0 ; j < 4 ;j++){
					arr[i][j] = board[i][j];
				}
			}
		}
};
stack<BoardState> history;

void saveState(int board[4][4]){
	BoardState state(board);
	history.push(state);
}
bool UndoMove(int board[4][4]){
	if(history.empty()){
		cout<<"\nNo moves to undo\n";
		return false;
	}
	BoardState last = history.top(); //jo bhi last move hoga will be stored in this
	history.pop();
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ;j++){
			 board[i][j] = last.arr[i][j];
		}
	}
	cout<<"\nMove undone.\n";
	return true;
}
void InitializeBoard(int board[4][4]){
	for(int i =0 ; i < 4 ; i++){
		for(int j=0 ; j < 4 ; j++){
			board[i][j] = 0 ;
		}
	}
}
void PrintBoard(int board[4][4]){
	for(int i = 0 ; i < 4 ; i++){
		cout<<"\t\t";
		for(int  j = 0; j < 4 ; j++){
			if(board[i][j] == 0){
				cout<<"| 0";
			} else {
				cout<<"| "<<board[i][j] ;
			}
		}
	cout << "|\n\t\t-----------\n";
	}
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
bool moveLeft(int board[4][4]){
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

bool moveRight(int board[4][4]){
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
bool moveUp(int board[4][4]){
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
bool moveDown(int board[4][4]){
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
	for(int i = 0 ; i < 4 ; i++){
		for(int j = 0 ; j < 4 ; j++){
			temp[i][j] = board[i][j];
		}
	}
	if(moveLeft(temp) || moveRight(temp) || moveUp(temp) || moveDown(temp)){
		return false;
	}
	return true;
}
int main(){
	cout<<"\t\t\t\t2048 GAME\n";
	srand(time(0));
	InitializeBoard(board);
//	PrintBoard(board);
	addRandomTile(board);
	addRandomTile(board);
	addRandomTile(board);
	bool running = true;
	cout<<"\nControls: A = LEFT , D = RIGHT , W = UP , S = DOWN, U = UNDO , Q = QUIT\n";
	while(running){
		PrintBoard(board);
		
		cout<<"Enter move: \n";
		
		char opt = getch();	
		bool moved = false;
		
		if(opt == 'u' || opt == 'U'){ //u is for undo your previous move
			UndoMove(board);
			continue;
		}
		saveState(board);
		switch(opt){
			case 'a': case 'A':
				moved = moveLeft(board);
				break;
			case 'd': case 'D':
				moved = moveRight(board);
				break;
			case 'w': case 'W':
				moved = moveUp(board);
				break;
			case 's':case'S':
				moved = moveDown(board);
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
		PrintBoard(board);
		cout<<"GAME OVER\n";
		running = false;		
	}

	}

	
	return 0;
}
