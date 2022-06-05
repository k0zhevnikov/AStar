CC=g++
CFLAGS=-O2 -Wall

astar:
	g++ main.cpp astar.cpp -o astar
clean:
	${RM} astar