all: index
	gcc -o index index.o
index: index.c
	gcc -c -o index.o index.c
clean:
	rm -rf *.o index
