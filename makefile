
CXXFLAGS = -std=c++03

all : sample.exe libjson.a

sample.exe : sample.o libjson.a
	g++ sample.o -L./ -ljson -o $@ -s

libjson.a : json.o parse.o
	ar ru libjson.a json.o parse.o

sample.o : sample.cpp
json.o : json.cpp json.h
parse.o : parse.cpp json.h

clean :
	rm -f *.o *.exe *.a
