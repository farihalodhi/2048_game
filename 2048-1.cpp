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

class MoveNode {
public:
    char move;
    MoveNode* next;
    
    MoveNode(char m) {
        move = m;
        next = NULL;
    }
};

class MoveHistory {
private:
    MoveNode* head;
    
public:
    MoveHistory() {
        head = NULL;
    }
    
    void addMove(char move) {
        MoveNode* newNode = new MoveNode(move);
        newNode->next = head;
        head = newNode;
    }
    
    void displayMoves() {
        MoveNode* curr = head;
        cout << "Recent Moves: ";
        
        if(curr == NULL) {
            cout << "None yet";
            return;
        }
        
        int count = 0;
        while(count < 5 && curr != NULL) {
            cout << curr->move;
            curr = curr->next;
            count++;
            
            if(curr != NULL && count < 5) {
                cout << " -> ";
            }
        }
    }
    
    void clear() {
        MoveNode* curr = head;
        while(curr != NULL) {
            MoveNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        head = NULL;
    }
    
    ~MoveHistory() {
        clear();
    }
};

int board[4][4];
int score = 0;
string name;
MoveHistory moveHistory;

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
	
	// ADD THESE LINES:
	cout << "\n\t\t";
	SetConsoleTextAttribute(hConsole, 11);
	moveHistory.displayMoves();
	SetConsoleTextAttribute(hConsole, 7);
	
	cout<<"\nControls: WASD = MOVE, U = UNDO, H = HINT, R = ANALYZE, Q = QUIT\n";
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
// Add PlayerData struct
struct PlayerData {
    string name;
    int score;
    int gamesPlayed;
    
    PlayerData() {
        name = "";
        score = 0;
        gamesPlayed = 0;
    }
    
    PlayerData(string n, int s, int g) {
        name = n;
        score = s;
        gamesPlayed = g;
    }
};

// Linear Search - Find players above score
void linearSearchByScore(int minScore) {
    leaderboard.LoadFromFile();
    
    cout << "\n=== LINEAR SEARCH: Players with score >= " << minScore << " ===\n\n";
    
    bool found = false;
    int count = 0;
    
    for(int i = 0; i < leaderboard.TABLE_SIZE; i++) {
        if(!leaderboard.table[i].isEmpty) {
            if(leaderboard.table[i].highScore >= minScore) {
                SetConsoleTextAttribute(hConsole, 11);
                cout << ++count << ". " << leaderboard.table[i].name;
                SetConsoleTextAttribute(hConsole, 7);
                cout << " - Score: " << leaderboard.table[i].highScore 
                     << " | Games: " << leaderboard.table[i].gamesPlayed << "\n";
                found = true;
            }
        }
    }
    
    if(!found) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "No players found with score >= " << minScore << "\n";
        SetConsoleTextAttribute(hConsole, 7);
    } else {
        cout << "\nTotal players found: " << count << "\n";
    }
}

// Convert hash table to array
int convertHashToArray(PlayerData arr[]) {
    int count = 0;
    
    for(int i = 0; i < leaderboard.TABLE_SIZE; i++) {
        if(!leaderboard.table[i].isEmpty) {
            arr[count].name = leaderboard.table[i].name;
            arr[count].score = leaderboard.table[i].highScore;
            arr[count].gamesPlayed = leaderboard.table[i].gamesPlayed;
            count++;
        }
    }
    
    return count;
}

// Selection Sort by score (descending)
void sortPlayersByScore(PlayerData arr[], int n) {
    for(int i = 0; i < n - 1; i++) {
        int maxIndex = i;
        
        for(int j = i + 1; j < n; j++) {
            if(arr[j].score > arr[maxIndex].score) {
                maxIndex = j;
            }
        }
        
        if(maxIndex != i) {
            PlayerData temp = arr[i];
            arr[i] = arr[maxIndex];
            arr[maxIndex] = temp;
        }
    }
}

// Find player by name in sorted array
int binarySearchByName(PlayerData arr[], int n, string targetName) {
    for(int i = 0; i < n; i++) {
        if(arr[i].name == targetName) {
            return i + 1;
        }
    }
    return -1;
}

// Main function - Find player rank
void findPlayerRank(string playerName) {
    leaderboard.LoadFromFile();
    
    cout << "\n=== FINDING RANK FOR: " << playerName << " ===\n\n";
    
    Player* p = leaderboard.search(playerName);
    
    if(p == NULL) {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "Player '" << playerName << "' not found!\n";
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }
    
    cout << "Player found! Score: " << p->highScore << "\n\n";
    
    PlayerData players[100];
    int totalPlayers = convertHashToArray(players);
    
    cout << "Total players: " << totalPlayers << "\n";
    cout << "Sorting...\n";
    sortPlayersByScore(players, totalPlayers);
    
    int rank = binarySearchByName(players, totalPlayers, playerName);
    
    if(rank != -1) {
        SetConsoleTextAttribute(hConsole, 14);
        cout << "\n=============================\n";
        cout << "   RANK: #" << rank << " / " << totalPlayers << "\n";
        cout << "=============================\n";
        SetConsoleTextAttribute(hConsole, 7);
        
        cout << "\n--- Context ---\n";
        int start = (rank - 2 > 0) ? rank - 2 : 1;
        int end = (rank + 1 <= totalPlayers) ? rank + 1 : totalPlayers;
        
        for(int i = start - 1; i < end; i++) {
            if(i + 1 == rank) {
                SetConsoleTextAttribute(hConsole, 10);
                cout << ">>> ";
            } else {
                cout << "    ";
            }
            
            cout << "#" << (i + 1) << " " << players[i].name 
                 << " - " << players[i].score << "\n";
            SetConsoleTextAttribute(hConsole, 7);
        }
    }
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

class MoveEvaluation {
public:
    char move;           // W, A, S, D
    int scoreGain;       // Points gained from this move
    int maxTile;         // Highest tile after move
    int emptyCells;      // Number of empty cells
    int totalScore;      // Combined evaluation score
    
    MoveEvaluation(char m, int sg, int mt, int ec) {
        move = m;
        scoreGain = sg;
        maxTile = mt;
        emptyCells = ec;
        // Calculate total evaluation score
        totalScore = scoreGain * 2 + emptyCells * 10 + maxTile;
    }
};

void copyBoard(int source[4][4], int dest[4][4]) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

int findMaxTile(int board[4][4]) {
    int maxTile = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(board[i][j] > maxTile) {
                maxTile = board[i][j];
            }
        }
    }
    return maxTile;
}

int countEmptyCells(int board[4][4]) {
    int count = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(board[i][j] == 0) {
                count++;
            }
        }
    }
    return count;
}

string suggestBestMove(int board[4][4], int currentScore) {
    queue<MoveEvaluation> moveQueue;
    char directions[4] = {'W', 'A', 'S', 'D'};
    
    for(int i = 0; i < 4; i++) {
        int tempBoard[4][4];
        copyBoard(board, tempBoard);
        int tempScore = currentScore;
        bool moved = false;
        
        switch(directions[i]) {
            case 'W':
                moved = moveUp(tempBoard, tempScore);
                break;
            case 'A':
                moved = moveLeft(tempBoard, tempScore);
                break;
            case 'S':
                moved = moveDown(tempBoard, tempScore);
                break;
            case 'D':
                moved = moveRight(tempBoard, tempScore);
                break;
        }
        
        if(moved) {
            int scoreGain = tempScore - currentScore;
            int maxTile = findMaxTile(tempBoard);
            int emptyCells = countEmptyCells(tempBoard);
            
            MoveEvaluation eval(directions[i], scoreGain, maxTile, emptyCells);
            moveQueue.push(eval);
        }
    }
    
    if(moveQueue.empty()) {
        return "Hint: No valid moves available!";
    }
    
    MoveEvaluation bestMove = moveQueue.front();
    moveQueue.pop();
    
    while(!moveQueue.empty()) {
        MoveEvaluation current = moveQueue.front();
        moveQueue.pop();
        
        if(current.totalScore > bestMove.totalScore) {
            bestMove = current;
        }
    }
    
    string suggestion = "AI Hint: ";
    switch(bestMove.move) {
        case 'W': suggestion += "Try UP (W)"; break;
        case 'A': suggestion += "Try LEFT (A)"; break;
        case 'S': suggestion += "Try DOWN (S)"; break;
        case 'D': suggestion += "Try RIGHT (D)"; break;
    }
    
    return suggestion;
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

bool canReach2048(int board[4][4], int depth) {
    // Base Case 1: Already has 2048
    int maxTile = findMaxTile(board);
    if(maxTile >= 2048) {
        return true;
    }
    
    // Base Case 2: Depth limit reached (prevent infinite recursion)
    if(depth <= 0) {
        return false;
    }
    
    // Base Case 3: Game over, no moves possible
    if(isGameOver(board)) {
        return false;
    }
    
    // Recursive Case: Try all 4 directions
    char directions[4] = {'W', 'A', 'S', 'D'};
    
    for(int i = 0; i < 4; i++) {
        int tempBoard[4][4];
        int tempScore = 0;
        copyBoard(board, tempBoard);
        
        bool moved = false;
        
        switch(directions[i]) {
            case 'W':
                moved = moveUp(tempBoard, tempScore);
                break;
            case 'A':
                moved = moveLeft(tempBoard, tempScore);
                break;
            case 'S':
                moved = moveDown(tempBoard, tempScore);
                break;
            case 'D':
                moved = moveRight(tempBoard, tempScore);
                break;
        }
        
        // If move valid, check recursively
        if(moved) {
            // Add a random tile (simulate game progression)
            addRandomTile(tempBoard);
            
            // Recursive call with reduced depth
            if(canReach2048(tempBoard, depth - 1)) {
                return true;  // Found a path to 2048!
            }
        }
    }
    
    // All paths exhausted, 2048 not reachable
    return false;
}

int countPossibleMoves(int board[4][4], int depth) {
    // Base Case: Reached desired depth
    if(depth <= 0) {
        return 1;  // This is one complete path
    }
    
    // If game over, no more moves possible
    if(isGameOver(board)) {
        return 0;
    }
    
    int totalPaths = 0;
    char directions[4] = {'W', 'A', 'S', 'D'};
    
    // Try all 4 directions
    for(int i = 0; i < 4; i++) {
        int tempBoard[4][4];
        int tempScore = 0;
        copyBoard(board, tempBoard);
        
        bool moved = false;
        
        switch(directions[i]) {
            case 'W':
                moved = moveUp(tempBoard, tempScore);
                break;
            case 'A':
                moved = moveLeft(tempBoard, tempScore);
                break;
            case 'S':
                moved = moveDown(tempBoard, tempScore);
                break;
            case 'D':
                moved = moveRight(tempBoard, tempScore);
                break;
        }
        
        // If move is valid, count paths from this state
        if(moved) {
            // Recursively count paths from new state
            totalPaths += countPossibleMoves(tempBoard, depth - 1);
        }
    }
    
    return totalPaths;
}
void PlayGame(){
	srand(time(0));
	
	leaderboard.LoadFromFile();
	InitializeBoard(board);
	score = 0 ;
	history = stack<BoardState>();
	moveHistory.clear(); 
	
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
		        if(moved) moveHistory.addMove('A'); 
		        break;
		        
		    case 'd': case 'D':			
		        moved = moveRight(board,score);
		        if(moved) moveHistory.addMove('D');
		        break;
		        
		    case 'w': case 'W':			
		        moved = moveUp(board,score);
		        if(moved) moveHistory.addMove('W');
		        break;
		        
		    case 's': case 'S':			
		        moved = moveDown(board,score);
		        if(moved) moveHistory.addMove('S');
		        break;
		        
		    case 'h': case 'H':
		        SetConsoleTextAttribute(hConsole, 10);
		        cout << "\n" << suggestBestMove(board, score) << "\n";
		        SetConsoleTextAttribute(hConsole, 7);
		        cout << "Press any key to continue...\n";
		        getch();
		        history.pop();
		        continue;
		        
		    case 'r': case 'R':
		        {  // ? ADD BRACES HERE
		            system("cls");
		            SetConsoleTextAttribute(hConsole, 14);
		            cout << "\n=== RECURSIVE GAME ANALYSIS ===\n\n";
		            SetConsoleTextAttribute(hConsole, 7);
		            
		            cout << "Current Max Tile: " << findMaxTile(board) << "\n\n";
		            
		            cout << "Analyzing reachability (depth 4)...\n";
		            if(canReach2048(board, 4)) {
		                SetConsoleTextAttribute(hConsole, 10);
		                cout << "? 2048 appears reachable!\n";
		            } else {
		                SetConsoleTextAttribute(hConsole, 12);
		                cout << "? 2048 might not be reachable in 4 moves\n";
		            }
		            SetConsoleTextAttribute(hConsole, 7);
		            
		            cout << "\nCounting move possibilities (depth 3)...\n";
		            int count = countPossibleMoves(board, 3);
		            cout << "Possible 3-move sequences: " << count << "\n";
		            
		            cout << "\nPress any key to continue...\n";
		            getch();
		            history.pop();
		            continue;
		        }  // ? CLOSE BRACES HERE
		        
		    case 'q': case 'Q':
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
		cout<<"1) Play Game\n";
		cout<<"2) Show Leaderboard\n";
		cout<<"3) Search Players by Score\n";
		cout<<"4) Find My Rank\n";
		cout<<"5) Quit\n";
		cout<<"\nEnter Your Choice: ";
		cin >> choice;
	
		switch(choice){
			case 1:
				PlayGame();
				break;
			case 2:
				showLeaderBoard(); // through heap pehle hashtable mein sara data ayega then usse max heap also hash table checks for duplicates
				break;
			case 3: {
			    int minScore;
			    cout << "\nEnter minimum score to search for: ";
			    cin >> minScore;
			    linearSearchByScore(minScore);
			    cout << "\nPress any key to continue...\n";
			    getch();
			    break;
			}
			
			case 4: {
			    string searchName;
			    cout << "\nEnter player name to find rank: ";
			    cin >> searchName;
			    findPlayerRank(searchName);
			    cout << "\nPress any key to continue...\n";
			    getch();
			    break;
			}
			
			case 5:
			    leaderboard.saveToFile();
			    SetConsoleTextAttribute(hConsole, 14);
			    cout<<"\nThanks for playing/visiting.. :)";
			    exit(0);	
			default:
				SetConsoleTextAttribute(hConsole, 12);
				cout<<"Invalid Choice!"<<endl;
				SetConsoleTextAttribute(hConsole, 7);		
		}		
	}while(choice != 5);
	return 0;
}
