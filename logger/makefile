CFLAGS+=-I. -Wall -pthread -g
all:main

main:main.o logger.o utils.o
	g++ $^ -lyaml-cpp -o $@ $(CFLAGS)
main.o:main.cpp logger.h
	g++ -o $@ -c main.cpp $(CFLAGS)
logger.o:logger.cpp logger.h mutex.h utils.h
	g++ logger.cpp -c -o $@ $(CFLAGS)
utils.o:utils.cpp utils.h
	g++ -o $@ -c utils.cpp $(CFLAGS)

clean:
	rm -rf *.o main 