CFLAGS+=-I../include/ -Wall

client:client.o
	gcc -o $@ $^ $(CFLAGS)  

client.o:client.c client.h
	gcc -c client.c $(CFLAGS)

clean:
	rm -rf *.o client