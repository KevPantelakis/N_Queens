#include "Board.h"

Board::Board(int size)
{
	this->size = size;
	board = std::vector<std::vector<int>>();
	for (int i = 0; i < size; ++i) {
		board.push_back(std::vector<int>());
		for (int j = 0; j < size; ++j) {
			board[i].push_back(0);
		}
	}
}

Board::~Board()
{
}

void Board::ReinitializeBoard() {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			board[i][j] = 0;
		}
	}
}

bool Board::InsertQueen(int col, int row) {

	//Verifiy insert queen legality
	// Check Row
	for (int i = 0; i < col; ++i) {
		if (board[row][i] == 1)
			return false;
	}

	int indX = row, indY = col;
	// Check upper left diagonal
	while (indX >= 0 && indY >= 0) {
		if (board[indX][indY] == 1)
			return false;
		indX--;
		indY--;
	}

	indX = row; indY = col;
	// Check lower left diagonal
	while (indX < size && indY >= 0) {
		if (board[indX][indY] == 1)
			return false;
		indX++;
		indY--;
	}

	// All is good InsertQueen
	board[row][col] = 1;
	return true;
}

void Board::RemoveQueen(int col, int row) {	
	board[row][col] = 0;
}

std::string Board::ToString() {
	std::string str = "| ";
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (board[i][j] == 1) {
				str += "Q | ";
			}
			else {
				str += "X | ";
			}
		}
		str += "\n\n| ";
	}
	return str;
}