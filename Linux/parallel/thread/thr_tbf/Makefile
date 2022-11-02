CFLAGS += -pthread -I . -Wall
LDFLAGS += -pthread

all:tbf

tbf:main.o mytbf.o
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS)

main.o:main.c
	cc $^ -c -o $@ $(CFLAGS) $(LDFLAGS)

mytbf.o:mytbf.c
	cc $^ -c -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o tbf