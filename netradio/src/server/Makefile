CFLAGS+=-I../include/ -pthread
LDFLAGS+=-pthread

server:server.o
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

server.o:server.c
	gcc -c $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o server