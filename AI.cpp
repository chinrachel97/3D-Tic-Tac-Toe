
// *****************
// INCLUDES
// *****************
#include <iostream>
#include <fstream>
#include <exception>
#include "AI.h"

using namespace std;

// CONSTRUCTOR
AI_t::AI_t(){}

// DESTRUCTOR
AI_t::~AI_t(){}

// 76 total rows/cols/diags
int wins[76][4] = { 
	// rows				// cols				// rods				// diags			// more diags
	{0, 1, 2, 3},		{0, 4, 8, 12},		{0, 16, 32, 48},	{0, 5, 10, 15},		{0, 20, 40, 60},
	{4, 5, 6, 7},		{1, 5, 9, 13},		{1, 17, 33, 49},	{16, 21, 26, 31},	{1, 21, 41, 61},
	{8, 9, 10, 11},		{2, 6, 10, 14},		{2, 18, 34, 50},	{32, 37, 42, 47},	{2, 22, 42, 62},
	{12, 13, 14, 15},	{3, 7, 11, 15},		{3, 19, 35, 51},	{48, 53, 58, 63},	{3, 23, 43, 63},
	{16, 17, 18, 19},	{16, 20, 24, 28},	{4, 20, 36, 52},	{3, 6, 9, 12},		{12, 24, 36, 48},
	{20, 21, 22, 23},	{17, 21, 25, 29},	{5, 21, 37, 53},	{19, 22, 25, 28},	{13, 25, 37, 49},
	{24, 25, 26, 27},	{18, 22, 26, 30},	{6, 22, 38, 54},	{35, 38, 41, 44},	{14, 26, 38, 50},
	{28, 29, 30, 31},	{19, 23, 27, 31},	{7, 23, 39, 55},	{51, 54, 57, 60},	{15, 27, 39, 51},
	{32, 33, 34, 35},	{32, 36, 40, 44},	{8, 24, 40, 56},	{0, 17, 34, 51},	{0, 21, 42, 63},
	{36, 37, 38, 39},	{33, 37, 41, 45},	{9, 25, 41, 57},	{4, 21, 38, 55},	{3, 22, 41, 60},
	{40, 41, 42, 43},	{34, 38, 42, 46},	{10, 26, 42, 58},	{8, 25, 42, 59},	{12, 25, 38, 51},
	{44, 45, 46, 47},	{35, 39, 43, 47},	{11, 27, 43, 59},	{12, 29, 46, 63},	{15, 26, 37, 48},
	{48, 49, 50, 51},	{48, 52, 56, 60},	{12, 28, 44, 60},	{3, 18, 33, 48},
	{52, 53, 54, 55},	{49, 53, 57, 61},	{13, 29, 45, 61},	{7, 22, 37, 52},
	{56, 57, 58, 59},	{50, 54, 58, 62},	{14, 30, 46, 62},	{11, 26, 41, 56},
	{60, 61, 62, 63},	{51, 55, 59, 63},	{15, 31, 47, 63},	{15, 30, 45, 60}
};

// checks if someone wins, returns 0 if game still going
int AI_t::win(const std::vector<int> board) {
    for (int index = 0; index < 76; ++index) {
        if (board[wins[index][0]] != 0 && 
		board[wins[index][0]] == board[wins[index][1]] && 
		board[wins[index][0]] == board[wins[index][2]] && 
		board[wins[index][0]] == board[wins[index][3]]) 
		{
            return board[wins[index][3]];
        }
    }
    return 0;
}


// calculates utility score for a board-state
int AI_t::utility(const std::vector<int> board) {
    int utilityScore = 0; int total4X = 0; int total4O = 0; int total3X = 0; int total3O = 0;
    int total2X = 0; int total2O = 0; int total1X = 0; int total1O = 0; int total3X1O = 0;
	for (int numGroup = 0; numGroup < 76; ++numGroup) {
        int numX = 0; int numO = 0; int numBlanks = 0;
		for (int numIndex = 0; numIndex < 4; ++numIndex) {
			// counting all X/O/blanks in all r/c/d (X=1, O=-1, blank=0)
			if (board[wins[numGroup][numIndex]] == 1) { numX++; }
			else if (board[wins[numGroup][numIndex]] == -1) { numO++; }
			else if (board[wins[numGroup][numIndex]] == 0) { numBlanks++; }
		}
		// determine # of X's/blanks in each r/c/d
		if (numX == 4) { total4X++; } 
		else if (numX == 3 && numBlanks == 1) { total3X++; }
		else if (numX == 2 && numBlanks == 2) { total2X++; }
		else if (numX == 1 && numBlanks == 3) { total1X++; }
		else if (numX == 3 && numO == 1) { total3X1O++; }
		// determine # of O's/blanks in each r/c/d
		if (numO == 4) { total4O++; } 
		else if (numO == 3 && numBlanks == 1) { total3O++; }
		else if (numO == 2 && numBlanks == 2) { total2O++; }
		else if (numO == 1 && numBlanks == 3) { total1O++; }
	}
	return (AI_t::utility_calc(total4X, total4O, total3X1O, total3X, total3O, total2X, total2O, total1X, total1O));
}

// helper function for calculating utility
int AI_t::utility_calc(int t4X,int t4O,int t3X1O,int t3X,int t3O,int t2X,int t2O,int t1X,int t1O) {
    // calculate utility score
    int utilityScore = 1000 * (t4X - t4O) + 
                    - 1000 * (t3X1O) +
                    100 * (t3X - t3O) + 
                     10 * (t2X - t2O) + 
                          (t1X - t1O);
    
    return utilityScore;
}



// This function is implmented with more than 24 lines as a conscious design choice..
    // .. we could of split minimax into 2 separate functions (min/max respectively)
    // .. but implementing in a single function would result in less stack frames.
// minimax algorithm with depth-limited search  
int AI_t::minimax(vector<int> board, int depth, int isMax, int alpha, int beta) {

	// terminate at this depth (affects performance)
    if (depth == 15) {
        return 0;
    }
	
    // maximizer's move (AI = -1)
    if (isMax) {
		// get utility score for current board-state
        int score = utility(board);
        return score;
        int best = -1000;
        
        // iterate through all moves, recursive call to minimax, then undo all moves
        for (int boardIndex = 0; boardIndex < board.size(); boardIndex++) {
            if (depth != 15 || win(board) != -1) {
                if (board[boardIndex] == 0) {
                    board[boardIndex] = -1;
                    best = max(best, minimax(board, depth+1, !isMax, alpha, beta));
                    alpha = max(alpha, best);
                    board[boardIndex] = 0;
                }
            }
            if (beta <= alpha) { break; }
            else { break; }
        }
        return best;
    }
    // minimizer's move (Player = 1)
    else {
		// get utility score for current board-state
        int score = utility(board);
        return -score;
        int best = 1000;
        
        // iterate through all moves, recursive call to minimax, then undo all moves
        for (int boardIndex = 0; boardIndex < board.size(); ++boardIndex) {
            if (depth != 15 || win(board) != 1) {
                if (board[boardIndex] == 0) {
                    board[boardIndex] = 1;
                    best = min(best, minimax(board, depth+1, !isMax,alpha,beta));
                    beta = max(alpha, best);
                    board[boardIndex] = 0;
                }
            } 
			if (beta <= alpha) { break; }
            else { break; }
        }
        return best;
    }
}

// returns best AI move
int AI_t::computerMove(vector<int>& board) { 
    int bestMoveScore = -100;
    int bestMoveIndex = 0; int depth = 0;
    for (int boardIndex = 0; boardIndex < board.size(); ++boardIndex) {     
        if (board[boardIndex] == 0) {
            // make AI move
            board[boardIndex] = -1;   
            int score = minimax(board, depth, false, -1000, 1000);  
            // undo AI move
            board[boardIndex] = 0;   
            // get best score   
            if (score > bestMoveScore) {    
                bestMoveScore = score;  
                bestMoveIndex = boardIndex;   
            }   
        }   
    } 
    board[bestMoveIndex] = -1;   
    return bestMoveIndex;
}

// calls the minmax and returns the index the AI chose
int AI_t::update_all(std::vector<int>& board){
    int accepted_move = computerMove(board);
    return accepted_move;
}