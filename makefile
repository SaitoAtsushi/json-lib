
CXXFLAGS = -std=c++03
EXT=.exe
.PHONY : test clean

all : libjson.a

test : jsontest$(EXT)
	./$< < testcase/case01.json
	./$< < testcase/case02.json
	./$< < testcase/case03.json
	./$< < testcase/case04.json

jsontest$(EXT) : test.o libjson.a
	g++ test.o -L./ -ljson -o $@ -s

libjson.a : json.o parse.o
	ar ru libjson.a json.o parse.o

test.o : test.cpp
json.o : json.cpp json.h
parse.o : parse.cpp json.h

clean :
	rm -f *.o *$(EXT) *.a *~
