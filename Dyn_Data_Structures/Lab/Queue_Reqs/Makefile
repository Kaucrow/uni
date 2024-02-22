bin/queue_reqs: main.o request.o queue.o panic.o
	g++ main.o request.o queue.o panic.o -o bin/queue_reqs

main.o:
	g++ -c src/main.cpp

request.o:
	g++ -c src/lib/request/request.cpp

queue.o:
	g++ -c ../Queue/src/lib/queue/queue.cpp

panic.o:
	g++ -c src/lib/misc/panic/panic.cpp

clean:
	del /s main.o request.o queue.o panic.o