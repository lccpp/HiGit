all:affinity
affinity:
	gcc -o affinity -Wall affinity.c -pthread
clean:
	rm affinity
