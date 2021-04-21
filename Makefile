CC = g++ -std=c++17 -O2 -fopenmp
INCLUDE = -I./include -I./deps
LIBRARY = -L./lib
LINK_FLAG = -lglm_static
OUTPUT = ./bin/program.exe
OBJS = ./src/main.cpp

all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE) $(LIBRARY) $(LINK_FLAG) -o $(OUTPUT)
