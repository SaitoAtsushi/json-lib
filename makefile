
CXXFLAGS = -std=c++03

all : sample.exe libjson.a

sample.exe : sample.o libjson.a
	g++ sample.o -L./ -ljson -o $@ -s

libjson.a : json.o
	ar ru libjson.a json.o

sample.o : sample.cpp
json.o : json.cpp json.h

clean :
	rm -f *.o *.exe *.a
