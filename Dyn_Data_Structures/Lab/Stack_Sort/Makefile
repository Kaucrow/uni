bin/stack_sort: main.o stack.o panic.o
	g++ main.o stack.o panic.o -o bin/stack_sort

main.o:
	g++ -c src/main.cpp

stack.o:
	g++ -c ../Stack/src/lib/stack/stack.cpp

panic.o:
	g++ -c ../Stack/src/lib/misc/panic/panic.cpp

clean:
	del /s main.o stack.o panic.o