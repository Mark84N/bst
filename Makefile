all:
	gcc -o main main.c queue/queue.c -g -Wall
clean:
	rm main
