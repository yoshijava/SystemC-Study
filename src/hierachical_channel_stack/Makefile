LIB_DIR=-L/proj/mtk10109/systemc-2.3.1/lib-linux64
INC_DIR=-I/proj/mtk10109/systemc-2.3.1/include
LIB=-lsystemc

all:
	mkdir -p out
	g++ -o out/stack.out *.cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread
clean:
	rm -f out/*



