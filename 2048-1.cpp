#include<iostream>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include<iomanip>
#include<stack>
#include<windows.h>
#include<fstream>
#include<queue>
using namespace std;
//add arrow keys
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int board[4][4];
int previousBoard[4][4];
int score = 0;
string name;


class Player{
	public:
	string name;
	int highScore;
	int gamesPlayed;
	bool isEmpty;
	
    Player() {  // default constructor
    name = "";
    highScore = 0;
    gamesPlayed = 0;
    isEmpty = true;
}

	Player(string nm,int Sc,int n){
		name = nm;
		highScore = Sc;
		gamesPlayed = n;
		isEmpty = false;
	}
};

class HashTable{
	public:
	static const int TABLE_SIZE = 100;
	Player table[TABLE_SIZE];
	
	int hashFunction(string name){
		int hash = 0;
		for(int i = 0 ; i < name.length() ; i++){
			hash += (int)name[i];
		}
		return hash % TABLE_SIZE;
	}
	
	HashTable(){
		for(int i = 0 ; i < TABLE_SIZE ; i++){
			table[i].isEmpty = true;
		}
	}
	
	void insertToHash(string name,int score){
		int index = hashFunction(name);
		int startIndex = index;
		
		while(!table[index].isEmpty && table[index].name != name){
			index = (index+1)%TABLE_SIZE;
			if(index == startIndex){
				cout<< "Hash TableFULL\n";
				return;
			}
		}
		
		if(table[index].isEmpty){
			table[index] = Player(name,score,1);
		} else {
			table[index].gamesPlayed++;
			if(score > table[index].highScore){
				table[index].highScore = score;
			}
		}
	}	
	void saveToFile(){
		ofstream MyFile("Player.txt");
		if(!MyFile.is_open()){
			SetConsoleTextAttribute(hConsole,12);
			cout << "Error: could not save data"<<endl;
			SetConsoleTextAttribute(hConsole,7);
			return;
		}
		for(int i=0;i<TABLE_SIZE;i++){
			if(!table[i].isEmpty){
				MyFile << table[i].name <<" "<<table[i].highScore <<" "<< table[i].gamesPlayed <<	 endl;
			}
		}
		MyFile.close();
	}
	void LoadFromFile(){
		ifstream in("Player.txt");
		if(!in.is_open()){
			return;
		}
		string name;
		int score,games;
		while(in >> name >> score >> games){
			int index = hashFunction(name);
			int startIndex = index;
			
			while(!table[index].isEmpty && table[index].name != name){
				index = (index+1) % TABLE_SIZE;
				if(index == startIndex){
					break;
				}
			}
			if(table[index].isEmpty || table[index].name == name){
				table[index] = Player(name , score , games);
			}
		}
		in.close();
	}
	Player *search(string name){
		int index = hashFunction(name);
		int startIndex = index;
		int attempts = 0;
		
		while(!table[index].isEmpty && attempts < TABLE_SIZE){
			if(table[index].name == name){
				return &table[index];
			}
			index = (index + 1) % TABLE_SIZE;
			attempts++;
			if(index == startIndex){
				break;
			}
		}
		return NULL;
	}
	void getAllPlayers(Player players[] , int &count){
		count = 0;
		for(int i=0; i<TABLE_SIZE;i++){
			if(!table[i].isEmpty){
				players[count] = table[i];
				count++;
			}
		}
	}
	
};
HashTable leaderboard;
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
    Player *p =leaderboard.search(name);
    int PlayerBest;
    if(p != NULL){
    	PlayerBest = p->highScore;
	} else {
		PlayerBest = 0;
	}
    
    cout << "\t\t\t PLAYER: "<< name <<" | BEST: "<< PlayerBest << "\n";
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
void showLeaderBoard(){
	leaderboard.LoadFromFile();
	
	priority_queue< pair<int,string> > pq;
	
	for(int i = 0 ; i < leaderboard.TABLE_SIZE ;i++){
		if(!leaderboard.table[i].isEmpty){
			pq.push({leaderboard.table[i].highScore,leaderboard.table[i].name});
		}
	}
	if(pq.empty()){
		SetConsoleTextAttribute(hConsole, 14);
		cout<<"\n================================" << endl;
		cout<<"           LEADERBOARD   "<< endl;
		cout<< "===================================="<<endl;
		SetConsoleTextAttribute(hConsole,12);
		cout<<"no players yet! be the first"<<endl;
		SetConsoleTextAttribute(hConsole, 7);
		cout << "press any key to continue..\n";
		getch();
		return;
	}
	SetConsoleTextAttribute(hConsole, 14);
	cout<<"=========================================="<<endl;
	cout<<"                    LEADERBOARD  "<< endl;
	SetConsoleTextAttribute(hConsole, 11);
	cout<< left << setw(5) << "Rank" << setw(15) << "Name" << "High score\n";
	SetConsoleTextAttribute(hConsole, 7);
	cout<<"----------------------------------------\n";
	int rank = 1;
	while(!pq.empty() && rank <= 5){
		if(rank <= 3){
			SetConsoleTextAttribute(hConsole, 14);
		} else {
			SetConsoleTextAttribute(hConsole, 7);
		}
		
		pair<int, string> top = pq.top();
		pq.pop();
		cout<<left <<setw(5)<<rank<< setw(15) 
		<<top.second << top.first <<"\n"<<endl;
//		pq.pop();
		rank++;
	}
	if(rank == 1){
		cout << "no scores yey. play a game first\n";
	}
	cout<<"\n";
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
		for(int i = 3 ; i > 0 ; i--){
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
void PlayGame(){
	srand(time(0));
	
	leaderboard.LoadFromFile();
	InitializeBoard(board);
	score = 0 ;
	history = stack<BoardState>();
	
//	PrintBoard(board);
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
				moved = moveLeft(board,score);
				break;
			case 'd': case 'D':			
				moved = moveRight(board,score);
				break;
			case 'w': case 'W':			
				moved = moveUp(board,score);
				break;
			case 's':case'S':			
				moved = moveDown(board,score);
				break;
			case 'q': case'Q':
				leaderboard.insertToHash(name,score);
				leaderboard.saveToFile();
				running = false;
				break;
			default:
				SetConsoleTextAttribute(hConsole, 12);
				cout<<"\ninvalid move! Press any key to continue\n";
				SetConsoleTextAttribute(hConsole, 7);
				getch();
				history.pop();
				continue;					
		}
			if(moved){
		addRandomTile(board);
		} 		
	if(isGameOver(board)){
		PrintBoard(board,score);
		leaderboard.insertToHash(name,score);
		leaderboard.saveToFile();
        SetConsoleTextAttribute(hConsole, 12);		
		cout<<"\n\t\tGAME OVER! No more moves possible\n";
		cout<<"\t\tFinal Score: "<< score <<"\n";
        SetConsoleTextAttribute(hConsole, 7);		
		running = false;		
		}
	}
//	leaderboard.LoadFromFile();
//	leaderboard.insertToHash(name,score);
//	leaderboard.saveToFile();
//	
	cout << "\nScore Saved! Press any key to continue.\n";
	getch();
}

int main(){
	leaderboard.LoadFromFile();
	SetConsoleTextAttribute(hConsole, 14	);
	cout << "\n\t\t\t   ==========================\n";
	cout<<"\t\t\t\t      2048 GAME\n";
	cout<<"\t\t\t  ==============================\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	cout<<"\t\t\t\tEnter your name: ";
	cin>>name;
	int choice;
	do{
		SetConsoleTextAttribute(hConsole, 11);
		cout<< "\n\t\t\t === MAIN MENU ===\n";
		SetConsoleTextAttribute(hConsole, 7);
		cout<<"1)Play Game\n2)Show Leaderboard\n3)Quit\n";
		cout<<"\nEnter Your Choice: ";
		cin >> choice;
	
		switch(choice){
			case 1:
				PlayGame();
				break;
			case 2:
				showLeaderBoard(); // through heap pehle hashtable mein sara data ayega then usse max heap also hash table checks for duplicates
				break;
			case 3:
				leaderboard.saveToFile();
				SetConsoleTextAttribute(hConsole, 14);
				cout<<"\nthanks for playing/visiting.. :)";
				exit(0);	
			default:
				SetConsoleTextAttribute(hConsole, 12);
				cout<<"Invalid Choice!"<<endl;
				SetConsoleTextAttribute(hConsole, 7);		
		}		
	}while(choice != 3);
	return 0;
}
