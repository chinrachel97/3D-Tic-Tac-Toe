#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream> 
#include <ncurses.h>

#include "backend.h"

using namespace std;

// ***************
// CONSTRUCTOR
// ***************
Backend::Backend(){
	v = std::vector<player_scores> ();
}

// ***************
// DESTRUCTOR
// ***************
Backend::~Backend(){}

// ***************
// FUNCTIONS
// ***************


	// ------******------
	// - Read in a txt file (format: "string int char")
	// 		- ending char is arbitrary as is simply used to guarantee tokenizing success
	// - If file does not exist, does not matter (user is var until game end)
	// - *note* struct implements overloaded functionality for reading as token
	// ------******------

	void Backend::read_hs(){
		player_scores player;
		ifstream fin("high_scores.txt");
		if (fin){
			v.clear();
			string a;
			int b;
			char c;
			while (fin >> a >> b >> c) {
				player.player_name = a;
				player.player_score = b;
				v.push_back(player);
			}
		}
		fin.close();
	}

	
	// ------******------
	// - Returns a vector of player struct objects at specified index
	// - struct object contains (int name, string score)
	// - *note* usage example: "be.get_hs(0).player_name;
	// ------******------

	Backend::player_scores Backend::get_player(int index){
		return v.at(index);
	}

	
	// ------******------
	// - Returns (string) score assoicated with the passed player initals
	// - string user_initals is the current user playing 
	// - finds the current user in the struct object vector (from file)
	// - if the user is found, will create a string from their values (name, score)
	// - if the user is not found than we set their score to initally 0
	// ------******------

	std::string Backend::get_user_hs(const char* initials){
		user_initals = initials;
		string user_score_string = "FAILURE TO RETRIEVE SCORE";

		bool user_found = false;
		int user_found_at_index = 0;

		for (int i = 0; i < v.size(); i++){
			if(v.at(i).player_name == user_initals){
				user_found = true;
				user_found_at_index = i;
			}
		}
		if(user_found){
			user_score = v.at(user_found_at_index).player_score;
			user_score_string = std::to_string(user_score);
		}
		else{
			user_score = 0;
			user_score_string = std::to_string(user_score);
		}
		return user_score_string;
	}

	
	// ------******------
	// - Takes the current users score in comparison to scores on file
	// - Sorts scores in descending order via overloaded op
	// - Writes the top 5 sorted scores to file
	// - *note* format: "name score char \n"
	// - *note* if the user does not exist, create a player object and put in vec
	// ------******------

	void Backend::write_sort_hs(){
		bool user_exists = false;
		if(v.size() != 0){
			for(int i = 0; i < v.size(); i++){
				if (v.at(i).player_name == user_initals){
					user_exists = true;
					v.at(i).player_score = user_score;
				}
			}
		}
		if (user_exists == false){
			player_scores end_game_player;
			end_game_player.player_name = user_initals;
			end_game_player.player_score = user_score;
			v.push_back(end_game_player);
		}
		if (v.size() >= 2)
			std::sort(v.begin(), v.end());
		ofstream newFile("high_scores.txt");
		//ensures that we only write the top 5
		int max = (v.size() < 5) ? v.size() : 5;
		for(int i = 0; i < max; i++)
			newFile << v.at(i).player_name << " " << v.at(i).player_score << '.' << "\n";
		newFile.close();
	}

	
	// ------******------
	// - Creates and passes a vector of strings for h_s printing
	// - *note* must cast / convert the player score from int to string
	// ------******------

	std::vector<std::string> Backend::pass_scores(){
		std::vector<std::string> scores_vec;
		string player_string;
		for(int i = 0; i < v.size(); i++){
			player_string = v.at(i).player_name + " " + std::to_string(v.at(i).player_score);
			scores_vec.push_back(player_string);
		}
		return scores_vec;
	}

	
	// ------******------
	// - Checks if the desired move index is occupied / available
	// - If occupied presents the user with a error message
	// - No need to print message for AI
	// ------******------

	bool Backend::is_valid_move(int game_board_data_index, int player_value, std::vector<int> &board) {
		if(player_value == 1){
			if(board[game_board_data_index] == 0)
				return true;
			else{
				return false;
			}
		}
		else{
			if(board[game_board_data_index] == 0){ return true; }
			else{ return false; }
		}
	}

	
	// ------******------
	// - Additional computations required to convert ncurses cordinates to integer
	// - Used to allow for backend computation and manipulations in the AI and player boards 
	// ------******------

	bool Backend::layer_coord_to_index(int y_wcoord, int x_wcoord, int player_value, std::vector<int> &board) {
		bool validity = false;
		if(y_wcoord == 1 && is_valid_move((x_wcoord / 2), player_value, board)) { 		// row 1
			board[x_wcoord / 2] = player_value;
			validity = true;	
		}
		else if(y_wcoord == 2 && is_valid_move(((x_wcoord / 2) + 4), player_value, board)) {	// row 2
			board[(x_wcoord / 2) + 4] = player_value;
			validity = true;	
		}
		else if(y_wcoord == 3 && is_valid_move(((x_wcoord / 2) + 8),player_value, board)) {	// row 3
			board[(x_wcoord / 2) + 8] = player_value;	
			validity = true;
		}
		else {						// row 4
			if(is_valid_move(((x_wcoord / 2) + 12), player_value, board)){
				board[(x_wcoord / 2) + 12] = player_value;
				validity = true;
			}
		}
		return validity;		
	}

	
	// ------******------
	// - Computations required to convert ncurses cordinates to integer
	// - Finds correct index within game_board_data to hold layer entry
	// - Used to allow for backend computation and manipulations in the AI and player boards 
	// ------******------

	bool Backend::update_game_board_data(int x_curr, int y_wcoord, int x_wcoord, int player_value, std::vector<int> &board) {
		bool validity = false;
		if(x_curr >= 23 && x_curr < 35) {		// layer 2
			validity = layer_coord_to_index(y_wcoord, x_wcoord + 32, player_value, board);
		}
		else if(x_curr >= 35 && x_curr < 47) {	// layer 3
			validity = layer_coord_to_index(y_wcoord, x_wcoord + 64, player_value, board);
		}
		else if(x_curr >= 47) {					// layer 4
			validity = layer_coord_to_index(y_wcoord, x_wcoord + 96, player_value, board);
		}
		else {									// layer 1
			validity = layer_coord_to_index(y_wcoord, x_wcoord, player_value, board);
		}
		return validity;
	}

	
	// ------******------
	// - convert index to layer coordinates
	// ------******------
	
	void Backend::index_to_layer_cord(int game_board_index){
		int index = game_board_index;
		if (game_board_index >= 0 && game_board_index <= 15) {
			AI_move_x = 11; AI_move_y = 11; AI_move_layer = 1;
		}
		if (game_board_index >= 16 && game_board_index <= 31) {
			AI_move_x = 23; AI_move_y = 11; AI_move_layer = 2;
		}
		if (game_board_index >= 32 && game_board_index <= 47) {
			AI_move_x = 35; AI_move_y = 11; AI_move_layer = 3;
		}
		if (game_board_index >= 48 && game_board_index <= 63) {
			AI_move_x = 47; AI_move_y = 11; AI_move_layer = 4;
		}
		index = game_board_index % 16;
		while (index >= 4){
			index = index - 4;
			++AI_move_y;
		}
		AI_move_x = (index * 2) + AI_move_x;
	}

	
	// ------******------
	// - convert index to layer coordinates
	// - returns a pair of coordinates
	// ------******------
	
	std::vector<int> Backend::index_to_layer_WINS(int game_board_index){
		std::vector<int> cordinate_pair;
		int index = game_board_index;
		if (game_board_index >= 0 && game_board_index <= 15) {
			AI_move_x = 17; AI_move_y = 11; AI_move_layer = 1;
		}
		if (game_board_index >= 16 && game_board_index <= 31) {
			AI_move_x = 29; AI_move_y = 11+2; AI_move_layer = 2;
		}
		if (game_board_index >= 32 && game_board_index <= 47) {
			AI_move_x = 41; AI_move_y = 11+4; AI_move_layer = 3;
		}
		if (game_board_index >= 48 && game_board_index <= 63) {
			AI_move_x = 53; AI_move_y = 11+6; AI_move_layer = 4;
		}
		index = game_board_index % 16;
		while (index >= 4){
			index = index - 4;
			++AI_move_y;
		}
		AI_move_x = (index * 2) + AI_move_x;
		cordinate_pair.push_back(AI_move_x);
		cordinate_pair.push_back(AI_move_y);
		return cordinate_pair;
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks row win
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_row_win(int row_starting_index, std::vector<int> &board) {
		int line_total = 0;
		
		for(int i = row_starting_index; i < row_starting_index + 4; i++){
			line_total += board[i];
		}
		
		if(line_total == 4){ return 2; }
		else if(line_total == -4){ return 0;}
		else { return -1; }
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks column win
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_column_win(int column_starting_index, std::vector<int> &board) {
		int line_total = 0;

		for(int i = column_starting_index; i < column_starting_index + 16; i += 4){
			line_total += board[i];
		}
		
		if(line_total == 4){ return 2; }
		else if(line_total == -4){ return 0; }
		else { return -1; }
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks corner win
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_corner_win(int corner_starting_index, std::vector<int> &board) {
		int line_total = 0;

		for(int i = corner_starting_index; i < corner_starting_index + 16; i += 5){
			line_total += board[i];
		}
		
		if(line_total == 4){ return 2; }
		else if(line_total == -4){ return 0; }
		
		line_total = 0;

		for(int i = corner_starting_index + 3; i <= corner_starting_index + 12; i += 3){
			line_total += board[i];
		}
		
		if(line_total == 4){ return 2;}
		else if(line_total == -4){ return 0; }
		else { return -1; }
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks horizontal win
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_horizontal_win(int horizontal_starting_index, std::vector<int> &board) {
		int line_total = 0;
		for(int i = horizontal_starting_index; i < 64; i += 16){
			line_total += board[i];
		}

		if(line_total == 4){ return 2; }
		else if(line_total == -4){ return 0;}
		else { return -1; }
	}
	
	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks vertical diagonal win
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_vertical_diagonal_win(int vertical_diagonal_starting_index, std::vector<int> &board) {
		int line_total = 0;
		for(int i = vertical_diagonal_starting_index; i < 64; i += 20){
			line_total += board[i];
		}

		if(line_total == 4){ return 2; }
		else if(line_total == -4){ return 0; }
		
		line_total = 0;

		if(vertical_diagonal_starting_index >= 12){
			for(int i = vertical_diagonal_starting_index; i <= vertical_diagonal_starting_index + 36; i += 12){
				line_total += board[i];
			}
		}

		if(line_total == 4){ return 2; }
		else if(line_total == -4){ return 0; }
		else{ return -1; }
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks horizontal diagonal win
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_horizontal_diagonal_win(int horizontal_diagonal_starting_index, std::vector<int> &board) {
		int line_total = 0;
		if(horizontal_diagonal_starting_index == 0 || horizontal_diagonal_starting_index == 4 || 
			horizontal_diagonal_starting_index == 8 || horizontal_diagonal_starting_index == 12){ 
			for(int i = horizontal_diagonal_starting_index; i < 64; i += 17){
				line_total += board[i];
			}
		}

		if(line_total == 4){ return 2; }
		else if(line_total == -4){ return 0; }

		line_total = 0;
		if(horizontal_diagonal_starting_index == 3 || horizontal_diagonal_starting_index == 7 || 
			horizontal_diagonal_starting_index == 11 || horizontal_diagonal_starting_index == 15) {
			for(int i = horizontal_diagonal_starting_index; i < 64; i += 15){
				line_total += board[i];
			}
		}

		if(line_total == 4) { return 2; }
		else if(line_total == -4){ return 0; }
		else{ return -1; }
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks back-left-top to front-right-bottom cross-section
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_blt_to_frb_win(std::vector<int> &board) {
		int line_total = 0;
		line_total += board[0] + board[21] + board[42] + board[63];
		return line_total;
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks front-left-top to back-right-bottom cross-section
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_flt_to_brb_win(std::vector<int> &board) {
		int line_total = 0;
		line_total += board[12] + board[25] + board[38] + board[51];
		return line_total;
	}

	
	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks back-right-top to front-left-bottom cross-section
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_brt_to_flb_win(std::vector<int> &board) {
		int line_total = 0;
		line_total += board[3] + board[22] + board[41] + board[60];
		return line_total;
	}


	// ------******------
	// - This is one of Base Tier Game Win Checks
	// - Checks front-right-top to back-left-bottom cross-section
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_frt_to_blb_win(std::vector<int> &board) {
		int line_total = 0;
		line_total += board[15] + board[26] + board[37] + board[48];
		return line_total;
	}


	// ------******------
	// - This is one of Third Tier Game Win Checks
	// - Is called from Secondary Checks and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_cross_section_win(std::vector<int> &board) {	
		if(check_blt_to_frb_win(board) == 4 || check_flt_to_brb_win(board) == 4 || 
			check_brt_to_flb_win(board) == 4 || check_frt_to_blb_win(board) == 4)
			return 2;
		else if(check_blt_to_frb_win(board) == -4 || check_flt_to_brb_win(board) == -4 || 
			check_brt_to_flb_win(board) == -4 || check_frt_to_blb_win(board) == -4)
			return 0;

		return -1;
	}


	// ------******------
	// - This is one of Secondary Game Win Checks
	// - Is called from Master Check and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_3D_win(std::vector<int> &board) {
		for(int i = 0; i < 16; i++) {
			if(check_horizontal_win(i,board) == 2 || check_vertical_diagonal_win(i,board) == 2 || 
				check_horizontal_diagonal_win(i,board) == 2)
				return 2;
			else if(check_horizontal_win(i,board) == 0 || check_vertical_diagonal_win(i, board) == 0 || 
				check_horizontal_diagonal_win(i,board) == 0)
				return 0;
		}
		if(check_cross_section_win(board) == 2)
			return 2;
		else if(check_cross_section_win(board) == 0)
			return 0;
		else
			return -1;
	}


	// ------******------
	// - This is one of Secondary Game Win Checks
	// - Is called from Master Check and subsequently calls all sublayers to determine win
	// ------******------
	int Backend::check_layer_win(std::vector<int> &board) {
		for(int i = 0; i < 64; i += 16) {
			if(check_row_win(i,board) == 2 || check_row_win(i + 4,board) == 2 || 
				check_row_win(i + 8,board) == 2 || check_row_win(i + 12,board) == 2) 
				return 2;
			else if(check_row_win(i,board) == 0 || check_row_win(i + 4,board) == 0 || 
				check_row_win(i + 8,board) == 0 || check_row_win(i + 12,board) == 0)
				return 0;
			else if(check_column_win(i,board) == 2 || check_column_win(i + 1,board) == 2 || 
				check_column_win(i + 2,board) == 2 || check_column_win(i + 3,board) == 2)
				return 2;
			else if(check_column_win(i,board) == 0 || check_column_win(i + 1,board) == 0 || 
				check_column_win(i + 2,board) == 0 || check_column_win(i + 3,board) == 0)
				return 0;
			else if(check_corner_win(i,board) == 2)
				return 2;
			else if(check_corner_win(i,board) == 0)
				return 0;
		}
		return -1;
	}


	// ------******------
	// - This is the Master Game Win Check
	// - Is called from front and subsequently calls all sublayers to determine win
	// ------******------

	int Backend::check_game_status(std::vector<int> &board) {
		if(check_layer_win(board) == 2 || check_3D_win(board) == 2){ return 2; }
		else if(check_layer_win(board) == 0 || check_3D_win(board) == 0){ return 0; }
		else if(!(find(board.begin(), board.end(), 0) != board.end())){ return 1; }
		else{ return -1; }
	}


	// ------******------
	// - resets board to an 'empty' (all 64 values to zero) state
	// ------******------
	void Backend::reset_game_board_data(std::vector<int> &board) {
		for(int i = 0; i < board.size(); i++) {
			board[i] = 0;
		}
	}


	// 76 total rows/cols/diags
	vector<vector<int>> wins_final = { 
		// rows
		{0, 1, 2, 3}    , {4, 5, 6, 7}    , {8, 9, 10,11}   , {12, 13, 14, 15},
		{16, 17, 18, 19}, {20, 21, 22, 23}, {24, 25, 26, 27}, {28, 29, 30, 31},
		{32, 33, 34, 35}, {36, 37, 38, 39}, {40, 41, 42, 43}, {44, 45, 46, 47},
		{48, 49, 50, 51}, {52, 53, 54, 55}, {56, 57, 58, 59}, {60, 61, 62, 63},
		// cols
		{0, 4, 8, 12}   , {1, 5, 9, 13}   , {2, 6, 10, 14}  , {3, 7, 11, 15},
		{16, 20, 24, 28}, {17, 21, 25, 29}, {18, 22, 26, 30}, {19, 23, 27, 31},
		{32, 36, 40, 44}, {33, 37, 41, 45}, {34, 38, 42, 46}, {35, 39, 43, 47},
		{48, 52, 56, 60}, {49, 53, 57, 61}, {50, 54, 58, 62}, {51, 55, 59, 63},
		// rods
		{0, 16, 32, 48} , {1, 17, 33, 49} , {2, 18, 34, 50}   , {3, 19, 35, 51},
		{4, 20, 36, 52} , {5, 21, 37, 53} , {6, 22, 38, 54}   , {7, 23, 39, 55},
		{8, 24, 40, 56} , {9, 25, 41, 57} , {10, 26, 42, 58}  , {11, 27, 43, 59},
		{12, 28, 44, 60}, {13, 29, 45, 61}, {14, 30, 46, 62}  , {15, 31, 47, 63},
		// diags
		{0, 5, 10, 15}, {16, 21, 26, 31}, {32, 37, 42, 47}, {48, 53, 58, 63},
		{3, 6, 9, 12} , {19, 22, 25, 28}, {35, 38, 41, 44}, {51, 54, 57, 60},
		{0, 17, 34, 51}, {4, 21, 38, 55}, {8, 25, 42, 59} , {12, 29, 46, 63},
		{3, 18, 33, 48}, {7, 22, 37, 52}, {11, 26, 41, 56}, {15, 30, 45, 60},
		{0, 20, 40, 60} , {1, 21, 41, 61} , {2, 22, 42, 62} , {3, 23, 43, 63},
		{12, 24, 36, 48}, {13, 25, 37, 49}, {14, 26, 38, 50}, {15, 27, 39, 51},
		{0, 21, 42, 63}, {3, 22, 41, 60}, {12, 25, 38, 51}, {15, 26, 37, 48}
	};

	// ------******------
	// - returns the coordinates with the winning row/col/diag for blinking
	// ------******------
	
	std::vector<std::vector<int> > Backend::get_blink(std::vector<int> &board){
		std::vector<std::vector<int> > vec_of_positions;
		int a,b,c,d = 0;
		for (int i=0; i<76; ++i) {
			bool is_winning_set = true;
			for(int j=0; j<4; ++j){
				if(board[wins_final[i][j]] != -1){
					is_winning_set = false;
					break;
				}
			}
			if(is_winning_set){
				a = wins_final[i][0];
				b = wins_final[i][1];
				c = wins_final[i][2];
				d = wins_final[i][3];
				break;
			}
		}
		vec_of_positions.push_back(index_to_layer_WINS(a));
		vec_of_positions.push_back(index_to_layer_WINS(b));
		vec_of_positions.push_back(index_to_layer_WINS(c));
		vec_of_positions.push_back(index_to_layer_WINS(d));
		return vec_of_positions;
	}