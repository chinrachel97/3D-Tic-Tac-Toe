#include <iostream>
#include <vector>
#include <ncurses.h>

#include "frontend.h"

using namespace std;

int main(){
	
	Frontend fe;
	fe.init_screen();
	fe.print_splash();
	fe.print_instruction();
	bool play_again = true;
	
	// loop through the game if player chooses to play again
	while(play_again){
		fe.print_hs_instruction();
		play_again = fe.play_game();
		clear();
	}
	endwin();
	
	return 0;
}