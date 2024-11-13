bin/stack: main.o stack.o panic.o
	g++ main.o stack.o panic.o -o bin/stack

main.o:
	g++ -c src/main.cpp

stack.o:
	g++ -c src/lib/stack/stack.cpp

panic.o:
	g++ -c src/lib/misc/panic/panic.cpp

clean:
	del /s main.o stack.o panic.o