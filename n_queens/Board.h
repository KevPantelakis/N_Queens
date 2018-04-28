#pragma once

#include <vector>

class Board
{
public:
	int size;
	Board(int size);
	~Board();
	void ReinitializeBoard();
	bool InsertQueen(int col, int row);
	void RemoveQueen(int col, int row);
	std::string ToString();

private:
	std::vector<std::vector<int>> board;
};

