#makefile

all: main

main: main.cpp
	 g++ -std=c++11 main.cpp frontend.cpp backend.cpp AI.cpp -lncurses++ -lmenu -lpanel -lform -lutil -lncurses