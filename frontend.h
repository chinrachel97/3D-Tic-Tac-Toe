/* header file of all functions related to front end of the game */
#include <string>
#include "backend.h"

#include "AI.h"

using namespace std;

class Frontend{
public:
	// connection to backend
	Backend be;

	// connection to ai
	AI_t ai_test;

	// AI
	void AI_move(WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, WINDOW* layer4, std::vector<int> &board);

	// make the winning columns blink
	void win_blink(std::vector<std::vector<int> > v, int game_status);
	
	// assist win blink
	void write_char(std::vector<std::vector<int> > v, std::string c);

	// constructor
	Frontend();
	
	// destructor
	~Frontend();
	
	// initialize the screen
	void init_screen();

	// print colored text
	void print_colored_text(const char* text, int color_code);

	// print the splash screen 
	void print_splash();

	// print the instruction screen
	void print_instruction();

	// print the high scores and instructions 
	void print_hs_instruction();

	// print hard-coded literals for high scores and instructions screen
	void print_hs_inst_lits();

	// print the game board and execute moves, returns true if user chooses to play again
	bool play_game();

	// create a window
	WINDOW *create_newwin(int height, int width, int starty, int startx);

	// refresh windows
	void refresh_win(int x_curr, WINDOW* layer1, WINDOW* layer2, 
		WINDOW* layer3, WINDOW* layer4);

	// move cursor in a particular layer given actual coordinates
	void move_cursor(WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, 
		WINDOW* layer4, int x_curr, int y_wcoord, int x_wcoord);

	// reset coordinates after left key press
	void coord_reset_left(int* x_wcoord, int* x_curr);

	// reset coordinates after right key press
	void coord_reset_right(int* x_wcoord, int* x_curr);

	// get const values
	int get_cvals(string s);

	// return user's chosen char to play (X or O)
	string get_user_char();

	// get the high scores given index
	const char* get_hs(vector<string> scores, int index, int valid);

	// get user's initials
	const char* get_initials();

	// find score given user's initials
	void get_score(const char* initials);

	// check game status
	int get_game_status();

	// print end of game message, returns true if user chooses to play again
	bool print_EOG_msg(int status);

	// function for left key pressed
	void key_left(int* x_wcoord, int* x_curr);

	// function for right key pressed
	void key_right(int* x_wcoord, int* x_curr);

	// function for up key pressed
	void key_up(int* y_wcoord, int* y_curr);

	// function for down key pressed
	void key_down(int* y_wcoord, int* y_curr);

	// print user move onto the board
	void key_enter(WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, 
		WINDOW* layer4, int x_curr, int y_wcoord, int x_wcoord);

	// take in user move and refresh the board
	void execute_moves(int* x_wcoord, int* x_curr, int* y_wcoord, int* y_curr,
		WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, WINDOW* layer4);
};