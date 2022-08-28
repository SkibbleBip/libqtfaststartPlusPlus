OBJS	= test.o
SOURCE	= test.cpp
HEADER	= QtFastStartCPP.hpp
OUT	= build/qtfs
CC	 = g++
FLAGS	 = -c -Wall -Wextra -I../src
LFLAGS	 = ../src/build/libQtFastStart.a

all: $(OBJS)
	mkdir -p build
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

test.o: test.cpp
	$(CC) $(FLAGS) test.cpp


clean:
	rm -f $(OBJS) $(OUT)
