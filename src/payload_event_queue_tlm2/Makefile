LIB_DIR=-L/proj/mtk10109/systemc-2.3.1/lib-linux64
INC_DIR=-I/proj/mtk10109/systemc-2.3.1/include
LIB=-lsystemc
main=Main

main:
	mkdir -p out && clear
	g++ -o out/$(main) *.cc -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

all: main

clean:
	rm out/*
