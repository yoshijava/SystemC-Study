LIB_DIR=-L/proj/mtk05583/systemc-2.3.1/lib-linux64
INC_DIR=-I/proj/mtk05583/systemc-2.3.1/include
LIB=-lsystemc
exam1=tlm2_getting_started_1
exam2=tlm2_getting_started_2
exam3=tlm2_getting_started_3
exam4=tlm2_getting_started_4
exam5=tlm2_getting_started_5
exam6=tlm2_getting_started_6
training1=Training_1

all: exam1 exam2 exam3 exam4 exam5 exam6 training1

training1:
	g++ -o $(training1) src/$(training1).cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

exam1:
	g++ -o $(exam1) src/$(exam1).cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

exam2:
	g++ -o $(exam2) src/$(exam2).cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

exam3:
	g++ -o $(exam3) src/$(exam3).cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

exam4:
	g++ -o $(exam4) src/$(exam4).cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

exam5:
	g++ -o $(exam5) src/$(exam5).cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

exam6:
	g++ -o $(exam6) src/$(exam6).cpp -g $(LIB_DIR) $(INC_DIR) -static $(LIB) -lm -lpthread

clean:
	rm ${exam1} ${exam2} ${exam3} ${exam4} ${exam5} ${exam6} ${training1}
