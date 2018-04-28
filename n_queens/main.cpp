#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <time.h> 
#include <windows.h>
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
	if (startCol >= chessBoard.size) { 
		return true; 
	}
	for (int i = 0; i < chessBoard.size; i++) {
		if (chessBoard.InsertQueen(startCol, i)) {
			if (BacktrackQueen(chessBoard, startCol + 1))
				return true;
			// On n'a pas pu placer toutes les reines, on backtrack
			chessBoard.RemoveQueen(startCol, i);
		}
	}
	//Aucun placement possible pour la configuration initiale donnée
	return false;
}

bool PureLasVegasQueen(Board &chessBoard, int k = 0) {
	int currentCol = 0;
	list<int> initialPossibleColumnIndex;
	list<int> possibleColIndex;
	for (int i = 0; i < chessBoard.size; ++i)
		initialPossibleColumnIndex.push_back(i);
	possibleColIndex = list<int>(initialPossibleColumnIndex);
	while (currentCol != chessBoard.size) {
		int rowIndex = *next(possibleColIndex.begin(), rand() % possibleColIndex.size());
		possibleColIndex.remove(rowIndex);
		if (chessBoard.InsertQueen(currentCol,rowIndex)) {
			currentCol++;
			possibleColIndex = list<int>(initialPossibleColumnIndex);
		}
		else if (possibleColIndex.size() == 0) {
			chessBoard = Board(chessBoard.size);
			currentCol = 0;
			possibleColIndex = list<int>(initialPossibleColumnIndex);
		}		
	}
	return true;
}

bool MutatedLasVegasQueen(Board &chessBoard, int k) {
	int currentCol = 0;
	list<int> initialPossibleColumnIndex;
	list<int> possibleColIndex;
	for (int i = 0; i < chessBoard.size; ++i)
		initialPossibleColumnIndex.push_back(i);
	possibleColIndex = list<int>(initialPossibleColumnIndex);

	// Remplir les premieres k reines avec l'algo LasVegas
	while (currentCol != k) {
		int rowIndex = *next(possibleColIndex.begin(), rand() % possibleColIndex.size());
		possibleColIndex.remove(rowIndex);
		if (chessBoard.InsertQueen(currentCol, rowIndex)) {
			currentCol++;
			possibleColIndex = list<int>(initialPossibleColumnIndex);
		}
		else if (possibleColIndex.size() == 0) {
			chessBoard = Board(chessBoard.size);
			currentCol = 0;
			possibleColIndex = list<int>(initialPossibleColumnIndex);
		}
	}

	// Remplir le reste avec l'algo BackTrack
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
	result = algo(board, k);
	auto end = steady_clock::now();
	duration<double> elapsedSeconds = (end-start);
	if (printTime) {
		if (algoName == "Mutated LasVegas")
			cout << "Time for Mutated LasVegas with board size " << board.size << " and LasVegas Fill " << k << ": " << fixed << elapsedSeconds.count() << endl;
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
	thread PLV(run, PureLasVegasQueen, Board(60), 0, "Pure LasVegas", false, true);
	thread MLV(run, MutatedLasVegasQueen, Board(60), 50, "Mutated LasVegas", false, true);
	thread PBack(run, BacktrackQueen, Board(20), 0, "Pure BackTrack", false, true);
	PLV.join();
	MLV.join();
	PBack.join();

	cin.get();
	return 0;
}