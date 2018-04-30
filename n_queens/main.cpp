#include <iostream>
#include <time.h> 
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <thread>  
#include "Board.h"

using namespace std;
using Algo = const std::function<bool(Board&, int)>&;

// [min, max[ 
int rand_in_range(int min, int max) {
	return (rand() % max) + min;
}

bool BacktrackQueen(Board &chessBoard, int startCol) {
	// If chessboard already filled, no job to do here. 
	if (startCol >= chessBoard.size) { 
		return true; 
	}
	for (int i = 0; i < chessBoard.size; i++) {
		if (chessBoard.InsertQueen(startCol, i)) {
			if (BacktrackQueen(chessBoard, startCol + 1))
				return true;
			// Could not place all queens Backtrack
			chessBoard.RemoveQueen(startCol, i);
		}
	}
	//No solution found for given initial board
	return false;
}

bool LasVegasQueen(Board &chessBoard, int k) {
	int currentCol = 0;
	list<int> initialPossibleColumnIndex;
	list<int> possibleColIndex;

	for (int i = 0; i < chessBoard.size; ++i)
		initialPossibleColumnIndex.push_back(i);
	
	possibleColIndex = (initialPossibleColumnIndex);

	// Fill the first k columns with LasVegas algorithm
	while (currentCol != k) {
		// Get a random row index to try inserting a queen and remove it from the list 
		// to prevent re-using it if insertion wasn't valid.
		int rowIndex = *next(possibleColIndex.begin(), rand() % possibleColIndex.size());
		possibleColIndex.remove(rowIndex);

		if (chessBoard.InsertQueen(currentCol, rowIndex)) {
			// Queen succesfully inserted continue with next column.
			currentCol++;
			possibleColIndex = (initialPossibleColumnIndex);
		}
		else if (possibleColIndex.size() == 0) {
			// Tried all possibilities for current column restarting the fill from scratch.
			chessBoard = Board(chessBoard.size);
			currentCol = 0;
			possibleColIndex = (initialPossibleColumnIndex);
		}
	}

	// Fill the rest with backtracking alrogithm
	return BacktrackQueen(chessBoard, k);
}

void run(Algo algo, Board board, int k, string algoName, bool printResult, bool printTime) {
	using namespace std::chrono;

	if (board.size == 0 || board.size == 2 || board.size == 3) {
		cout << "No solution for board size " << board.size << "X" << board.size << ". Use a board size 4 or larger" << endl;
		return;
	}

	bool result;

	auto start = steady_clock::now();
	if (algoName == "BackVegas") {
		do {
			result = algo(board, k);
		} while (!result);
	}
	else {
		result = algo(board, k);
	}	
	auto end = steady_clock::now();

	if (printTime) {
		duration<double> elapsedSeconds = (end - start);
		if (algoName == "BackVegas")
			cout << "Time for BackVegas with board size " << board.size << " and LasVegas Fill " << k << ": " << fixed << elapsedSeconds.count() << endl;
		else
			cout << "Time for " << algoName << " with board size " << board.size << ": " << fixed << elapsedSeconds.count() << endl;
	}
	if (printResult && result) {
		cout << board.ToString() << endl;
	}
	
}

int main()
{
	srand(time(NULL));	
	thread PLV(run, LasVegasQueen, Board(60), 60, "Pure LasVegas", false, true);
	thread MLV(run, LasVegasQueen, Board(60), 50, "BackVegas", false, true);
	thread PBack(run, BacktrackQueen, Board(20), 0, "Pure BackTrack", false, true);
	PLV.join();
	MLV.join();
	PBack.join();

	cin.get();
	return 0;
}