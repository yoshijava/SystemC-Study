LIB_DIR=-L/proj/mtk10109/systemc-2.3.1/lib-linux64
INC_DIR=-I/proj/mtk10109/systemc-2.3.1/include
LIB=-lsystemc

main:
	mkdir -p out
	g++ -o out/risc_cpu *.cc -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

all: main

clean:
	rm out/*
