#
# In order to execute this "Makefile" just type "make"
#	A. Delis (ad@di.uoa.gr)
#
OBJS	= ArtificialFS.o ByteBuffer.o main.o
SOURCE	= ArtificialFS.cpp ByteBuffer.cpp main.cpp
HEADER	= ArtificialFS.hpp ByteBuffer.hpp QtFastStartCPP.hpp
OUT	= build/libQtFastStart.so
CC	 = g++

FLAGS	 = -c -Wall -fexceptions -O2 -Wextra -fPIC
LFLAGS	 =
# -g option enables debugging mode
# -c flag generates object code for separate files


all: $(OBJS)
	mkdir -p build
	$(CC) -shared $(OBJS) -o $(OUT) $(LFLAGS)
	ar -r -s build/libQtFastStart.a $(OBJS)

# create/compile the individual files >>separately<<
ArtificialFS.o: ArtificialFS.cpp
	$(CC) $(FLAGS) ArtificialFS.cpp -std=c++14

ByteBuffer.o: ByteBuffer.cpp
	$(CC) $(FLAGS) ByteBuffer.cpp -std=c++14

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++14


# clean house
clean:
	rm -fr $(OBJS) $(OUT) build
