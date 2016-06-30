LIB_DIR=-L/proj/mtk05583/systemc-2.3.1/lib-linux64
INC_DIR=-I/proj/mtk05583/systemc-2.3.1/include
LIB=-lsystemc
fir=main

all: fir

fir:
	mkdir -p out
	g++ -o out/$(fir) *.cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

clean:
	rm out/*
