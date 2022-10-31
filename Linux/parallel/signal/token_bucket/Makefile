CFLAGS+=-I. -Wall

token_bucket:main.o token_bucket.o
	gcc $^ -o $@

clean:
	rm -rf *.o token_bucket