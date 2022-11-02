CFLAGS+=-I. -Wall 

all:string

string:main.o myString.o utils.o
	g++ $^ -o $@ $(CFLAGS)

myString.o:myString.cpp myString.h utils.h
	g++ myString.cpp -c -o $@ $(CFLAGS)

main.o:main.cpp myString.h
	g++ main.cpp -c -o $@ $(CFLAGS)

utils.o:utils.cpp utils.h
	g++ utils.cpp -c -o $@ $(CFLAGS)

clean:
	rm -rf *.o string