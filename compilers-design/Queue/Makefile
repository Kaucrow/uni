bin/queue: main.o queue.o panic.o
	g++ main.o queue.o panic.o -o bin/queue

main.o:
	g++ -c src/main.cpp

queue.o:
	g++ -c src/lib/queue/queue.cpp

panic.o:
	g++ -c src/lib/misc/panic/panic.cpp

clean:
	del /s main.o queue.o panic.o