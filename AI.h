//
// Created by Darby Traylor on 3/26/18.
//

// *****************
// INCLUDES
// *****************
#ifndef AI_H
#define AI_H

#include <vector>
#include <ncurses.h>

class AI_t{
public:

    //CONSTRUCTOR
    AI_t();

    // DESTRUCTOR
    ~AI_t();

    // checks if someone wins, returns 0 if game still going
    int win(const std::vector<int> board);

    // calculates utility score for a board-state
    int utility(const std::vector<int> board);

    // minimax algorithm with depth-limited search  
    int minimax(std::vector<int> board, int depth, int isMax, int alpha, int beta);

	// returns best AI move
    int computerMove(std::vector<int>& board);

    // calls the minmax and returns the index the AI chose
    int update_all(std::vector<int>& board);


    // helper function for calculating utility
    int utility_calc(int t4X,int t4O,int t3X1O,int t3X,int t3O,int t2X,int t2O,int t1X,int t1O);
	
};
#endif