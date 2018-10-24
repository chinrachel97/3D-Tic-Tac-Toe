#include <iostream>
#include <vector>
#include <string>
#include <ncurses.h>
#include <unistd.h>

#include "frontend.h"
#include "backend.h"

const char* tic_tac_toe_title = 
	"	      ____  __  ___     ____  __    ___     ____  __  ____\n"
	"	     (_  _)(  )/ __)___(_  _)/ _\\  / __)___(_  _)/  \\(  __)\n"
	"	       )(   )(( (__(___) )( /    \\( (__(___) )( (  O )) _)\n"
	"	      (__) (__)\\___)    (__)\\_/\\_/ \\___)    (__) \\__/(____)\n";
		
const char* _3d_title = 
	"			  .--,-``-.\n"
	"			 /   /:::::'.      ,---,\n"
	"			/ ../::::::::;   .'  .':`\\\n"
	"			\\ ``\\::.`-::::',---.':::::\\\n"
	"			 \\___\\/   \\::::|   |::.`\\::|\n"
	"			      \\   ::::|:   ::|  '::|\n"
	"			      /  /:::/ |   ':'  ;:::\n"
	"			      \\  \\:::\\ '   |:;  .::|\n"
	"			  ___ /   ::::||   |::  |::'\n"
	"			 /   /\\   /::::'   ::| /::;\n"
	"			/ ,,/  ',-::::.|   |:'`:,/\n"
	"			\\ ''\\::::::::; ;   :::.'\n"
	"			 \\   \\:::::.'  |   ,.'\n"
	"			  `--`-,,-'    '---'\n\n\n\n";

// constant values for user and AI
const int HUMAN = 1;
const int AI = -1;
std::vector<int> board(64);

// constructor
Frontend::Frontend(){
	be = Backend();
	ai_test = AI_t();
	std::vector<int> board(64);
}

// destructor
Frontend::~Frontend(){}

// print AI move onto the board
void Frontend::AI_move(WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, WINDOW* layer4, std::vector<int> &board){
	int ai_chosen_move = ai_test.update_all(board); // calls the AI function to determine the best move ... this is the int
	be.index_to_layer_cord(ai_chosen_move); // converts AI int to the cordinates needed to print
	std::string c = "O";
	int temp_xcur = be.AI_move_x;
	if (be.AI_move_layer == 1){
		be.AI_move_x = be.AI_move_x-10; be.AI_move_y = be.AI_move_y-10;
		mvwprintw(layer1, be.AI_move_y, be.AI_move_x, c.c_str());
	}
	if (be.AI_move_layer == 2){
		be.AI_move_x = be.AI_move_x-22; be.AI_move_y = be.AI_move_y-10;
		mvwprintw(layer2, be.AI_move_y, be.AI_move_x, c.c_str());
	}
	if (be.AI_move_layer == 3){
		be.AI_move_x = be.AI_move_x-34; be.AI_move_y = be.AI_move_y-10;
		mvwprintw(layer3, be.AI_move_y, be.AI_move_x, c.c_str());
	}
	if (be.AI_move_layer == 4){
		be.AI_move_x = be.AI_move_x-46; be.AI_move_y = be.AI_move_y-10;
		mvwprintw(layer4, be.AI_move_y, be.AI_move_x, c.c_str());
	}
	move_cursor(layer1, layer2, layer3, layer4, temp_xcur, be.AI_move_y, be.AI_move_x);
}

// assist win blink
void Frontend::write_char(vector<vector<int> > v, string c){
	for(int i=0; i<4; ++i){
		move(v[i][1], v[i][0]);
		addstr(c.c_str());
	}
}
	
// reprint borderlines
void print_borders(){
	for(int i=0; i<4; ++i)
		for(int j=0; j<5; ++j){
			move(i+11, (8+j)*2);
			printw("|");
		}
	for(int i=0; i<4; ++i)
		for(int j=0; j<5; ++j){
			move(i+13, (14+j)*2);
			printw("|");
		}
	for(int i=0; i<4; ++i)
		for(int j=0; j<5; ++j){
			move(i+15, (20+j)*2);
			printw("|");
		}	
	for(int i=0; i<4; ++i)
		for(int j=0; j<5; ++j){
			move(i+17, (26+j)*2);
			printw("|");
		}
	refresh();
}
	
// make the winning columns blink
void Frontend::win_blink(vector<vector<int> > v, int game_status){
	int color = 2;
	for(int i=0; i<7; ++i){
		color = (color == 1) ? 2 : 1;
		if(game_status == 2){
			attron(COLOR_PAIR(color));
			write_char(v, "X");
			attroff(COLOR_PAIR(color));
			usleep(400000);
		}
		else if(game_status == 0){
			attron(COLOR_PAIR(color));
			write_char(v, "O");
			attroff(COLOR_PAIR(color));
			usleep(400000);
		}
		print_borders();
		refresh();
	}
}

// initialize the screen
void Frontend::init_screen(){
	system("resize -s 40 80");
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
}

	
// print colored text
void Frontend::print_colored_text(const char* text, int color_code) {
	start_color();
	
	// color codes for colored text
	init_pair(1, COLOR_RED, COLOR_BLACK);		// red text, black background
	init_pair(2, COLOR_GREEN, COLOR_BLACK);		// green text, black background
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);	// yellow text, black background
	init_pair(4, COLOR_BLUE, COLOR_BLACK);		// blue text, black background
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);	// magenta text, black background
	init_pair(6, COLOR_CYAN, COLOR_BLACK);		// cyan text, black background
	
	attron(COLOR_PAIR(color_code) | A_BOLD);
	printw(text);
	attroff(COLOR_PAIR(color_code) | A_BOLD);
}

// print the splash screen 
void Frontend::print_splash(){	
	printw("\n\n\n\n");
	print_colored_text(tic_tac_toe_title, 6);
	printw("\n");
	print_colored_text(_3d_title, 5);
	move(28, 35);
	attron(A_UNDERLINE);
	print_colored_text("Team 5: 27\n", 2);
	attroff(A_UNDERLINE);
	print_colored_text("                                   -> Darby\n"
					   "                                   -> Rachel\n"
					   "                                   -> Jacob\n"
					   "                                   -> Grant\n\n"
					   "                                    ", 1);
	attron(A_STANDOUT);
	addstr("<ENTER>\n");
	refresh();
	attroff(A_STANDOUT);
	
	// don't continue until user clicks ENTER
	while(getch() != '\n'){}
	clear();
}

// print the instruction screen
void Frontend::print_instruction(){
	print_colored_text(	"\n\n\n\n"
						"                            //                   \\\\\n"
						"                           //                     \\\\\n"
						"                           ||    <How To Play>    ||\n"
						"	                   \\\\                     //\n"
						"	                    \\\\                   //\n\n\n\n"
						"             1. Use the arrow keys to move to a certain cell.\n\n"
						"             2. Press ENTER to place a move.\n\n"
						"             3. Try to get four X's in a row, column or diagonal\n"
						"                before the computer gets four O's in a row, column,\n"
						"                or diagonal, and vice versa.\n\n\n\n"
						"                       ", 2);
	attron(A_STANDOUT);
	addstr("<Press ENTER to continue to game>\n");
	refresh();
	attroff(A_STANDOUT);
	
	// don't continue until user clicks ENTER
	while(getch() != '\n'){}
	clear();
}

// get the high scores given index
const char* Frontend::get_hs(vector<string> scores, int index, int valid){
	string ret = string("");
	if(valid == -1){
		if(index == 0) { ret = string("    1) ") +  string("") + string("\n");}
		else if(index == 1){ ret = string("    2) ") +  string("") + string("\n");}
		else if(index == 2){ ret = string("    3) ") +  string("") + string("\n");}
		else if(index == 3){ ret = string("    4) ") + string("") + string("\n");}
		else if(index == 4){ ret = string("    5) ") + string("") + string("\n");}
	} else{
		if(index == 0){ ret = string("    1) ") + scores.at(index) + string("\n");}
		else if(index == 1){ ret = string("    2) ") + scores.at(index) + string("\n");}
		else if(index == 2){ ret = string("    3) ") + scores.at(index) + string("\n");}
		else if(index == 3){ ret = string("    4) ") + scores.at(index) + string("\n");}
		else if(index == 4){ ret = string("    5) ") + scores.at(index) + string("\n");}
	} 
	return ret.c_str();
}

// get user's initials
const char* Frontend::get_initials(){
	move(7, 14);
	string ret = string("");
	int ch;
	int i = 0;
	while(i < 3){
		ch = getch();
		if(ch != '\n'){
			string curr_ch = string("");
			curr_ch += ch;
			printw(curr_ch.c_str());
			ret += ch;
			++i;
		}
	}
	return ret.c_str();
}

// find score given user's initials
void Frontend::get_score(const char* initials){
	string score = be.get_user_hs(initials);
	move(8, 23);
	printw(score.c_str());
}

// check game status
int Frontend::get_game_status(){
	// 2: win
	// 1: tie
	// 0: lose
	// -1: game has not ended
	return be.check_game_status(board);
}

// print end of game message, returns true if user chooses to play again
bool Frontend::print_EOG_msg(int status){
	if(status == -1)	return true;
	if(status == 2){
		win_blink(be.get_blink(board), status);
		move(23, 25);
		printw("You win! ");
		be.user_score+=2;
	} else if(status == 1){
		move(23, 25);
		printw("It's a tie! ");
		be.user_score++;
	} else if(status == 0){
		win_blink(be.get_blink(board), status);
		move(23, 25);
		printw("You lose. ");
	}
	be.write_sort_hs();
	printw("Play again? (y/n)");
	int ch = getch();
	while(ch != 'y' && ch != 'n')	ch = getch();
	if(ch == 'y') {
		be.reset_game_board_data(board);
		return true;
	}
	else{ return false;}	
}

// print hard-coded literals for high scores and instructions screen
void Frontend::print_hs_inst_lits(){
	print_colored_text(
		"    Initials:\n"
		"    Player Best Score:\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", 4);
	
	print_colored_text(
		"--------------------------------------------------------------------------------", 1);
		
	print_colored_text(
		"\n                                <INSTRUCTIONS>\n\n"
		"            1. Key in initials (maximum of 3 characters), and press \n"
		"               ENTER to start the game.\n\n"
		"            2. Use the arrow keys to move to a certain cell.\n\n"
		"            3. Press ENTER to place a move.\n\n"
		"            4. Try to get four X's in a row, column or diagonal\n"
		"               before the computer gets four O's in a row, column,\n"
		"               or diagonal, and vice versa.\n\n\n\n", 2);
}

// print the high scores and instructions 
void Frontend::print_hs_instruction(){
	be.read_hs();
	vector<string> scores= be.pass_scores();
	
	print_colored_text("\n    High Scores:\n", 1);
	
	if (scores.size() == 0)
		for(int i = 0; i < 5; i++) {
			print_colored_text(get_hs(scores, i, -1), 3);
		}
	else {
		for(int i=0; i<scores.size(); ++i)
			print_colored_text(get_hs(scores, i, 1), 3);
		
		if(scores.size() < 5)
			for(int i = scores.size(); i < 5; i++)
				print_colored_text(get_hs(scores, i, -1), 3);
	}
	print_hs_inst_lits();
	get_score(get_initials());
	refresh();
}

// create a window
WINDOW *Frontend::create_newwin(int height, int width, int starty, int startx){
	WINDOW *local_win = newwin(height, width, starty, startx);
	box(local_win, '*', '*');
	wborder(local_win, '|', '|', '-','-','+','+','+','+');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	for(int i=0; i<4; ++i)
		for(int j=0; j<5; ++j)
			mvwprintw(local_win, i+1, j*2, "|");
	wrefresh(local_win);
	
	return local_win;
}

// refresh windows
void Frontend::refresh_win(int x_curr, WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, WINDOW* layer4){
	if(x_curr >= 11 && x_curr <= 17)
		wrefresh(layer1);
	else if(x_curr >= 23 && x_curr <= 29)
		wrefresh(layer2);
	else if(x_curr >= 35 && x_curr <= 41)
		wrefresh(layer3);
	else if(x_curr >= 47 && x_curr <= 53)
		wrefresh(layer4);
}

// move cursor in a particular layer given actual coordinates
void Frontend::move_cursor(WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, WINDOW* layer4, int x_curr, int y_wcoord, int x_wcoord){
	// clear possibly displayed error message
	move(23, 30);
	printw("                                      ");
	
	if(x_curr >= 23 && x_curr < 35)
		wmove(layer2, y_wcoord, x_wcoord);
	else if(x_curr >= 35 && x_curr < 47)
		wmove(layer3, y_wcoord, x_wcoord);
	else if(x_curr >= 47)
		wmove(layer4, y_wcoord, x_wcoord);
	else
		wmove(layer1, y_wcoord, x_wcoord);
	refresh();
	refresh_win(x_curr, layer1, layer2, layer3, layer4);
}

// reset coordinates after left key press
void Frontend::coord_reset_left(int* x_wcoord, int* x_curr){
	if(*x_curr == 23){
		*x_wcoord = 7;
		*x_curr = 17;
	}
	else if(*x_curr == 35){
		*x_wcoord = 7;
		*x_curr = 29;
	}
	else if(*x_curr == 47){
		*x_wcoord = 7;
		*x_curr = 41;
	}
}

// reset coordinates after right key press
void Frontend::coord_reset_right(int* x_wcoord, int* x_curr){
	if(*x_curr == 17){
		*x_wcoord = 1;
		*x_curr = 23;
	}
	else if(*x_curr == 29){
		*x_wcoord = 1;
		*x_curr = 35;
	}
	else if(*x_curr == 41){
		*x_wcoord = 1;
		*x_curr = 47;
	}
}

// get const values
int Frontend::get_cvals(string s){
	if(s == "HEIGHT_LYR")
		return 6;
	else if(s == "WIDTH_LYR")
		return 9;
	else if(s == "Y_VAL")
		return 10;
	else if(s == "X_VAL_LYR1")
		return 16;
	else if(s == "X_VAL_LYR2")
		return 28;
	else if(s == "X_VAL_LYR3")
		return 40;
	else if(s == "X_VAL_LYR4")
		return 52;
}

// function for left key pressed
void Frontend::key_left(int* x_wcoord, int* x_curr){
	if(*x_wcoord <= 8 && *x_wcoord >= 2){
		*x_wcoord -= 2;
		*x_curr -= 2;
	}
	else
		coord_reset_left(x_wcoord, x_curr);
}

// function for right key pressed
void Frontend::key_right(int* x_wcoord, int* x_curr){
	if(*x_wcoord <= 6 && *x_wcoord >= 1){
		*x_wcoord += 2;
		*x_curr += 2;
	}
	else 
		coord_reset_right(x_wcoord, x_curr);
}

// function for up key pressed
void Frontend::key_up(int* y_wcoord, int* y_curr){
	if(*y_wcoord <= 4 && *y_wcoord >= 2){
		--*y_wcoord;
		--*y_curr;
	}
}

// function for down key pressed
void Frontend::key_down(int* y_wcoord, int* y_curr){
	if(*y_wcoord <= 3 && *y_wcoord >= 1){
		++*y_wcoord;
		++*y_curr;
	}
}

// return user's chosen char to play (X or O)
string Frontend::get_user_char(){
	return "X";
}
 
// print user move onto the board
void Frontend::key_enter(WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, WINDOW* layer4, int x_curr, int y_wcoord, int x_wcoord){
	string c = get_user_char();
	if(x_curr >= 23 && x_curr < 35)
		mvwprintw(layer2, y_wcoord, x_wcoord, c.c_str());
	else if(x_curr >= 35 && x_curr < 47)
		mvwprintw(layer3, y_wcoord, x_wcoord, c.c_str());
	else if(x_curr >= 47)
		mvwprintw(layer4, y_wcoord, x_wcoord, c.c_str());
	else
		mvwprintw(layer1, y_wcoord, x_wcoord, c.c_str());
}

// take in user move and refresh the board
void Frontend::execute_moves(int* x_wcoord, int* x_curr, int* y_wcoord, int* y_curr,
	WINDOW* layer1, WINDOW* layer2, WINDOW* layer3, WINDOW* layer4){
	int ch = getch();
	do{
		switch(ch){
			case KEY_LEFT:
				key_left(x_wcoord, x_curr); break;
			case KEY_RIGHT:
				key_right(x_wcoord, x_curr); break;
			case KEY_UP:
				key_up(y_wcoord, y_curr); break;
			case KEY_DOWN:
				key_down(y_wcoord, y_curr); break;
			case '\n':
			if(be.update_game_board_data(*x_curr, *y_wcoord, *x_wcoord, HUMAN, board)){
				key_enter(layer1, layer2, layer3, layer4, *x_curr, *y_wcoord, *x_wcoord); 
				if(get_game_status() == -1)
					AI_move(layer1, layer2, layer3, layer4, board);
				break;
			}
			else{ break; }	
		}
		move_cursor(layer1, layer2, layer3, layer4, *x_curr, *y_wcoord, *x_wcoord);
	}while(get_game_status() == -1 && (ch = getch()));
}

// print the game board and execute moves, returns true if user chooses to play again
bool Frontend::play_game(){
	WINDOW *layer1 = create_newwin(get_cvals("HEIGHT_LYR"), get_cvals("WIDTH_LYR"), 
		get_cvals("Y_VAL"), get_cvals("X_VAL_LYR1"));
	WINDOW *layer2 = create_newwin(get_cvals("HEIGHT_LYR"), get_cvals("WIDTH_LYR"), 
		get_cvals("Y_VAL") + 2, get_cvals("X_VAL_LYR2"));
	WINDOW *layer3 = create_newwin(get_cvals("HEIGHT_LYR"), get_cvals("WIDTH_LYR"), 
		get_cvals("Y_VAL") + 4, get_cvals("X_VAL_LYR3"));
	WINDOW *layer4 = create_newwin(get_cvals("HEIGHT_LYR"), get_cvals("WIDTH_LYR"), 
		get_cvals("Y_VAL") + 6, get_cvals("X_VAL_LYR4"));	
	int x_wcoord = 1;
	int y_wcoord = 1;
	int x_curr = 11;
	int y_curr = 11;
	wmove(layer1, y_wcoord, x_wcoord);
	wrefresh(layer1);
	execute_moves(&x_wcoord, &x_curr, &y_wcoord, &y_curr, 
		layer1, layer2, layer3, layer4);
	return print_EOG_msg(get_game_status());
}