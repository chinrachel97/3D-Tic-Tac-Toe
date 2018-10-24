#ifndef backend_H
#define backend_H

#include <vector>
#include<sstream>

using namespace std;

class Backend{
public:

	// ***************
	// CONSTRUCTOR
	// ***************
	Backend();
	
	
	// ***************
	// DESTRUCTOR
	// ***************
	~Backend();


	// *****************
	// DATA STRUCT(s)
	// *****************
	struct player_scores {
		std::string player_name;
		int player_score;

	    bool operator<(const player_scores& a) const{
        	return player_score > a.player_score;
    	}
	};

	
	// *****************
	// DATA
	// *****************

	// stores all the player struct objects
	std::vector<Backend::player_scores> v;

	// stores the winning cordinates for win blink
	std::vector<std::vector<int> > win_cords;
	std::vector<std::vector<int> > get_blink(std::vector<int> &board);

	// stored current user initals
	std::string user_initals;
	
	// stored current user score
	int user_score;
	
	// coordinates for the AI to ncurses (int to cord)
	int AI_move_layer;
	int AI_move_x;
	int AI_move_y;

	
	// *****************
	// FUNCTIONS
	// *****************

	// read in the highscore
	void read_hs();
	
	// get the player at that index
	player_scores get_player(int index);
	
	// write highscores
	void write_sort_hs();
	
	// vector to transport scores to be printed
	std::vector<std::string> pass_scores();
	
	// returns string version of user highscore for print
	std::string get_user_hs(const char* initials);

	// resets game_board_data to an 'empty' (all 64 values to zero) state
	void reset_game_board_data(std::vector<int> &board);

	// check if a move is valid
	bool is_valid_move(int game_board_data_index, int player_value, std::vector<int> &board);

	// finds correct index within game_board_data to hold layer entry
	bool layer_coord_to_index(int y_wcoord, int x_wcoord, int player_value, std::vector<int> &board);

	// updates game_board_data with player and AI data
	bool update_game_board_data(int x_curr, int y_wcoord, int x_wcoord, int player_value, std::vector<int> &board);

	// converts AI index to cordinates to print
	void index_to_layer_cord(int game_board_index);

	// takes the winning indexes and converts to cordinates
	std::vector<int> index_to_layer_WINS(int game_board_index);

	// check row win
	int check_row_win(int row_starting_index, std::vector<int> &board);

	// check column win
	int check_column_win(int column_starting_index, std::vector<int> &board);

	// check corner win
	int check_corner_win(int corner_starting_index, std::vector<int> &board);

	// check layer win
	int check_layer_win(std::vector<int> &board);

	// check horizontal win
	int check_horizontal_win(int horizontal_starting_index, std::vector<int> &board);

	// check vertical diagonal win
	int check_vertical_diagonal_win(int vertical_diagonal_starting_index, std::vector<int> &board);

	// check horizontal diagonal win
	int check_horizontal_diagonal_win(int horizontal_diagonal_starting_index, std::vector<int> &board);

	// check back-left-top to front-right-bottom cross-section
	int check_blt_to_frb_win(std::vector<int> &board);

	// check front-left-top to back-right-bottom cross-section
	int check_flt_to_brb_win(std::vector<int> &board);

	// check back-right-top to front-left-bottom cross-section
	int check_brt_to_flb_win(std::vector<int> &board);

	// check front-right-top to back-left-bottom cross-section
	int check_frt_to_blb_win(std::vector<int> &board);

	// check cross-section win
	int check_cross_section_win(std::vector<int> &board);

	// check multi-dimensional win
	int check_3D_win(std::vector<int> &board);

	// check game status
	int check_game_status(std::vector<int> &board);
};

#endif