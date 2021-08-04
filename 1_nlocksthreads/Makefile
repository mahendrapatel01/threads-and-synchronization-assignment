nofThreads = 10

all : nlocks nthreads

nlocks : nlocks.c
	gcc -pthread nlocks.c -o nlocks

nthreads : threads.c
	gcc -pthread threads.c -o nthreads
	
run : 
	./nthreads $(nofThreads)
	./nlocks
