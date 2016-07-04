LIB_DIR=-L/proj/mtk10109/mtk_git/APOEverest_p63/alps/external/yohu/systemc-2.3.1/lib-linux64
INC_DIR=-I/proj/mtk10109/mtk_git/APOEverest_p63/alps/external/yohu/systemc-2.3.1/include
LIB=-lsystemc

all:
	mkdir -p out
	g++ -o out/exor main.cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread
clean:
	rm -f out/*


