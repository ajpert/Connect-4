/***************************************************************
 ** Program: assignment4.cpp
 ** Author: Andy James
 ** Date: 3/13/2021
 ** Description: runs connect 4 game
 ** Input: row and col in the command line
 ** Output: none
 ***************************************************************/
#include <iostream>
#include <cmath>

using namespace std;

bool check_command(int argc, char const*argv[]);
void get_size(int&direction, string name);
void delete_board(char**board, int row, int col);
void print_board(char** board, int row, int col);
void reset_board(char** board, int row, int col);
void play_game(char** board, int row, int col);
void switch_piece(char&piece);
void get_move(char** board, int row, int col, int& row_move, int& col_move,  char piece);
void empty_col(char** board, int row, int col, int& row_move, int& col_move,  char piece);
bool game_over(char** board, int row, int col, int row_move, int col_move, char piece);
bool check_rows(char** board, int row, int col, int row_move, int col_move, char piece);
bool check_diagonal_left(char** board, int row, int col, int row_move, int col_move, char piece);
bool check_diagonal_right(char** board, int row, int col, int row_move, int col_move, char piece);
bool is_int(string num);
int get_int(string prompt);

/****************************************************************************************************
 ** Function: int main(int argc, char const *argv[])
 ** Description: checks for errors in the command line and runs the game
 ** Parameters: int argc, char const*argv[]
 ** Pre-condition: input 2 c style strings into the command line after ./a 
 ** Post-condition: none
 ***************************************************************************************************/
int main(int argc, char const *argv[])
{
	char** board;
	int row = 0;
	int col = 0;

	if(check_command(argc,argv)) { //if there is an error in the command line
		cout << "invlaid command line input, please do manualy" << endl;
		get_size(row, "rows");
		get_size(col, "cols");
	}
	else { // type in manuly if there was an error
		row = get_int(argv[1]);
		col = get_int(argv[2]);
	}

	board = new char*[row];

	for(int i = 0; i < row; i++) { // make new board
		board[i] = new char[col];
	}
	
	play_game(board, row, col);
	delete_board(board,row,col);
}
/****************************************************************************************************
 ** Function: delete_board(char**board, int row, int col)
 ** Description: deletes the dynamic array to free up memory
 ** Parameters: char**board, int row, int col
 ** Pre-condition: pass in a 2d char array and the row size and col size 
 ** Post-condition: none
 ***************************************************************************************************/
void delete_board(char**board, int row, int col) {
	for(int i = 0; i < row; i++) {
		delete [] board[i];
	}
	delete [] board;
}
/****************************************************************************************************
 ** Function: check_command(int argc, char const*argv[])
 ** Description: checks to see if there are errors in the command line
 ** Parameters: int argc, char const*argv[]
 ** Pre-condition: pass in argc (number of arguments in the command line) and a 2d char array
 ** Post-condition: output true if there is an error, false if else
 ***************************************************************************************************/
bool check_command(int argc, char const*argv[]) {
	if(argc != 3) { // if there are two many arguments
		return true;
	}
	else if(is_int(argv[1]) && (get_int(argv[1]) < 4 || get_int (argv[1]) > 20)) { 
		return true;
	}
	else if(is_int(argv[2]) && (get_int(argv[2]) < 4 || get_int (argv[2]) > 20)) {
		return true;
	}
	return false;
}
/****************************************************************************************************
 ** Function: get_size(int&direction, string name)
 ** Description: manuly gets the row and col if there is an error in the command line
 ** Parameters: int&direction, string name
 ** Pre-condition: pass in a int pointer and a string that says either col or row
 ** Post-condition: none
 ***************************************************************************************************/
void get_size(int&direction, string name) {
	string size = "";
	cout << "how many " << name << ": ";
	getline(cin,size);
	if(is_int(size) == false) {
		cout << "invalid input, please input an int" << endl;
		get_size(direction,name);
	}
	else if(get_int(size) < 4 || get_int(size) > 20) {
		cout << "invlid input, can only be between 4 and 20" << endl;
		get_size(direction,name);
	}
	else {
		direction = get_int(size);
	}
}
/****************************************************************************************************
 ** Function: reset_board(char** board, int row, int col)
 ** Description: set every spot in the board back to ' '
 ** Parameters: char** board, int row, int col :) hi there
 ** Pre-condition: pass in a 2d char array, and pass two ints that describe its size
 ** Post-condition: none
 ***************************************************************************************************/
void reset_board(char** board, int row, int col) {
	for(int i = 0; i < row; i++) {
		for(int j = 0; j < col; j++) {
			board[i][j] = ' ';
		}
	}
}
/****************************************************************************************************
 ** Function: print_board(char** board, int row, int col)
 ** Description: prints out the board
 ** Parameters: char** board, int row, int col
 ** Pre-condition: pass in a 2d char array, and pass two ints that describe its size
 ** Post-condition: none
 ***************************************************************************************************/
void print_board(char** board, int row, int col){
	for (int i = 0; i < col; ++i) {
		cout << "  "<< i+1;
		(i >= 9) ? cout << "" : cout << " "; // if it is 9 or greater, dont put an extra space 
	}
	
	cout << endl;

	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (i % 2 == 0 && j % 2 == 0)
				cout << "|\e[48;5;73m " << board[i][j] << " "; 
			else if (i % 2 == 1 && j % 2 == 1)
				cout << "|\e[48;5;73m " << board[i][j] << " ";
			else
				cout << "|\e[48;5;31m " << board[i][j] << " ";
			cout << "\033[0m";
		}
		cout << endl;
	}

}
/****************************************************************************************************
 ** Function: play_game(char** board, int row, int col)
 ** Description: runs the seperate tasks for connect 4 and repeats the game if the user wants to
 ** Parameters: char** board, int row, int col
 ** Pre-condition: pass in a 2d char array, and pass two ints that describe its size
 ** Post-condition: none
 ***************************************************************************************************/
void play_game(char** board, int row, int col) { // goes over by 3 lines, but cannot segment further down without being redundent, declaring varibales takes up most of the room
	char piece = 'X';
	int col_move = 0;
	int row_move = 0;
	int i = 0;
	string cont = "y";

	while(cont == "y") {
	i = 0;
	reset_board(board,row,col);
	print_board(board, row, col);
	for(i; i < row*col; i++) {
		get_move(board, row, col, row_move, col_move, piece);
		print_board(board, row, col);
		if(game_over(board,row,col,row_move, col_move,piece)) //break out of the for loop if win con
			break;
		switch_piece(piece);
	}
	(i == row*col) ? cout << "tie" << endl: cout << ""; // if it ran the whole for loop, it is a tie
	cout << "would you like to go again, y for yes, else for no: ";
	getline(cin,cont);
	}
}
/****************************************************************************************************
 ** Function: switch_piece(char&piece)
 ** Description: switches the pieces around
 ** Parameters: char&piece
 ** Pre-condition: pass in a char pointer
 ** Post-condition: none
 ***************************************************************************************************/
void switch_piece(char&piece) {
	if(piece == 'X') {
			piece = 'O';
		}
		else{
			piece = 'X';
		}
}
/****************************************************************************************************
 ** Function: void get_move(char** board, int row, int col, int& row_move, int& col_move,  char piece)
 ** Description: gets the column from the user and checks for legality
 ** Parameters: char** board, int row, int col, int& row_move, int& col_move,  char piece
 ** Pre-condition: pass in a 2d array, two ints for its size, and two int pointers for moves, and a char
 ** Post-condition: none
 ***************************************************************************************************/
void get_move(char** board, int row, int col, int& row_move, int& col_move,  char piece) {

	string move_string = "";

	cout << piece <<"'s turn, what column: ";

	getline(cin,move_string);

	if(is_int(move_string) && (get_int(move_string) > 0 && get_int(move_string) <= col)) { // if the input is an int or is in bounds of the board
		if(board[0][get_int(move_string) - 1] == ' ') { // if the top space is empty, the col is not full
			col_move = get_int(move_string) - 1;
			empty_col(board, row, col, row_move, col_move, piece);
		}
		else {
			cout << "the board is full there please enter a new col: " << endl;
			get_move(board, row, col, row_move, col_move, piece);
		}	
	}
	else {
		cout << "that is not a legal move" << endl;
		get_move(board, row, col, row_move, col_move, piece);
	}
}
/****************************************************************************************************
 ** Function: empty_col(char** board, int row, int col, int& row_move, int& col_move,  char piece)
 ** Description: puts the piece on the furthest spot on the column the user chose
 ** Parameters: char** board, int row, int col, int& row_move, int& col_move,  char piece
 ** Pre-condition: pass in a 2d array, two ints for its size, and two int pointers for moves, and a char
 ** Post-condition: none
 ***************************************************************************************************/
void empty_col(char** board, int row, int col, int& row_move, int& col_move,  char piece) {
	for(int i = row - 1; i >= 0; i--) {
		if(board[i][col_move] == ' ') { // if there is an empty spot, put a piece down and break out of the for loop
			row_move = i;
			board[row_move][col_move] = piece;
			break;
		}
	}
}
/************************************************************************************************************
 ** Function: game_over(char** board, int row, int col, int row_move, int col_move, char piece)
 ** Description: checks if there is 4 in a row of any piece
 ** Parameters: char** board, int row, int col, int& row_move, int& col_move,  char piece
 ** Pre-condition: pass in a 2d array, two ints for its size, and two int pointers for moves, and a char piece
 ** Post-condition: return true if check_rows or check_diagonals return true (4 in a row) false if else
 ************************************************************************************************************/
bool game_over(char** board, int row, int col, int row_move, int col_move, char piece) {
	if(check_rows(board,row,col,row_move,col_move,piece)) {
		cout << "piece " << piece << " won" << endl;
		return true;
	}
	else if(check_diagonal_left(board,row,col,row_move,col_move,piece)) {
		cout << "piece " << piece << " won" << endl;
		return true;
	}
	else if(check_diagonal_right(board,row,col,row_move,col_move,piece)) {
		cout << "piece " << piece << " won" << endl;
		return true;
	}
	
	else {
		return false;
	}
	
}
/************************************************************************************************************
 ** Function: check_rows(char** board, int row, int col, int row_move, int col_move, char piece)
 ** Description: checks the whole row and col of the last move to see if there is 4 in a row
 ** Parameters: char** board, int row, int col, int& row_move, int& col_move,  char piece
 ** Pre-condition: pass in a 2d array, two ints for its size, and two int pointers for moves, and a char piece
 ** Post-condition: return true if there is 4 in a row, false if else
 ************************************************************************************************************/
bool check_rows(char** board, int row, int col, int row_move, int col_move, char piece) { // goes over by 2 lines but I feel it is much better this way then seperated into two
	int count = 0;
	for(int i = 0; i < col; i++) { // check the whole column of the last move for 4 in a row
		if(board[row_move][i] == piece) {
			count++;
		}
		else if(board[row_move][i] != piece) {
			count = 0;
		}
		if(count == 4) {
			return true;
		}
	}
	count = 0;
	for(int i = 0; i < row; i++) { // check the whole row of the last move for 4 in a row
		if(board[i][col_move] == piece) {
			count++;
		}
		else if(board[i][col_move] != piece) {
			count = 0;
		}
		if(count == 4) {
			return true;
		}
	}
	return false;
}
/************************************************************************************************************
 ** Function: check_diagonal_left(char** board, int row, int col, int row_move, int col_move, char piece)
 ** Description: checks the whole diagonal going left on the board of where the last move was played
 ** Parameters: char** board, int row, int col, int& row_move, int& col_move,  char piece
 ** Pre-condition: pass in a 2d array, two ints for its size, and two int pointers for moves, and a char piece
 ** Post-condition: return true if there is 4 in a row, false if else
 ************************************************************************************************************/
bool check_diagonal_left(char** board, int row, int col, int row_move, int col_move, char piece) { // goes over by 1 line but cant further break it down
	int row_diagonal = row_move;
	int col_diagonal = col_move;
	int count = 0;
	while(row_diagonal < row - 1 && col_diagonal > 0) { //find the left edge of the board location from where they played
		row_diagonal++;
		col_diagonal--;
	}

	while(row_diagonal >= 0 && col_diagonal < col) { // go along to the other edge of the board and check for pieces

		if(board[row_diagonal][col_diagonal] == piece) {
			count++;
		}
		else if(board[row_diagonal][col_diagonal] != piece) {
			count = 0;
		}
		if(count == 4) {
			return true;
		}
		row_diagonal--;
		col_diagonal++;
	}
	return false;
}
/************************************************************************************************************
 ** Function: check_diagonal_right(char** board, int row, int col, int row_move, int col_move, char piece)
 ** Description: checks the whole diagonal going right on the board of where the last move was played
 ** Parameters: char** board, int row, int col, int& row_move, int& col_move,  char piece
 ** Pre-condition: pass in a 2d array, two ints for its size, and two int pointers for moves, and a char piece
 ** Post-condition: return true if there is 4 in a row, false if else
 ************************************************************************************************************/
bool check_diagonal_right(char** board, int row, int col, int row_move, int col_move, char piece) { // goes over by 1 line but cant further break it down
	int row_diagonal = row_move;
	int col_diagonal = col_move;
	int count = 0;
	while(row_diagonal < row - 1 && col_diagonal < col - 1) { // find the right edge of the board from where they played diagonaly
		row_diagonal++;
		col_diagonal++;
	}

	while(row_diagonal >= 0 && col_diagonal >= 0) { // go to the other edge diagonlay and check for 4 in a row

		if(board[row_diagonal][col_diagonal] == piece) {
			count++;
		}
		else if(board[row_diagonal][col_diagonal] != piece) {
			count = 0;
		}
		if(count == 4) {
			return true;
		}
		row_diagonal--;
		col_diagonal--;
	}
	return false;
}
/***************************************************************
 ** Function: is_int(string)
 ** Description: determines if given string is an int
 ** Parameters: string num
 ** Pre-conditions: take in a string parameter
 ** Post-conditions: return true if it is an int, false if not
 ***************************************************************/
bool is_int(string num) {
   //if the first number is not a negative or a number
   if(num[0] != '-' && (num[0] < '0' || num[0] > '9')) {
      return false;
   }
   //if the first char is a negative and that is all there is
   if(num[0] == '-' && num.length() == 1) {

      return false;
   }
//if you dont count 0 following by numbers an int
/*   
   //if the number is 0 but has numbers following
   if(num[0] == '0' && num.length() > 1) {
      return false;
   }
   //if the number is -0
   if(num[0] == '-' && num[1] == '0') {
      return false;
   }
*/
   //if the number starts with - check
   if(num[0] == '-') {
      for(int i = 1; i < num.length(); i++) {
	      if(num[i] < '0' || num[i] > '9') {
	         return false;
	      }
      }
      return true;
   }
   //if the number dosent have negative
   for(int i = 0; i < num.length(); i++) {
      if(num[i] < '0' || num[i] > '9') {
	      return false;
      }
   }	  
   return true;
}
/***************************************************************
 ** Function: get_int(string)
 ** Description: returns an int from a string input
 ** Parameters: string prompt
 ** Pre-conditions: take in a string parameter
 ** Post-conditions: return an int that was given by a string
 ***************************************************************/
int get_int(string prompt) {
   int num = 0;
   //if the number is negative
   if(prompt[0] == '-') {  
      for(int i = 1; i < prompt.length(); i++) {
	 num += ((int)prompt[i] - 48) * pow(10,prompt.length() - 1 - i);
      }
      num *= -1;
   }

   //if the number is posotive
   else {
      for(int i = 0; i < prompt.length(); i++) {
	 num += ((int)prompt[i] - 48) * pow(10,prompt.length() - 1 - i);
      }
   }
   return num;
}
