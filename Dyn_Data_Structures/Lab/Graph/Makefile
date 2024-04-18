bin/graph: main.o graph.o dungeon.o linked_list.o linked_list_iter.o stack.o random_gen.o panic.o
	g++ main.o graph.o dungeon.o linked_list.o linked_list_iter.o stack.o random_gen.o panic.o -o bin/graph

main.o:
	g++ -c src/main.cpp

graph.o:
	g++ -c src/lib/graph/graph.cpp

dungeon.o:
	g++ -c src/lib/dungeon_gen/dungeon.cpp

linked_list.o:
	g++ -c ../Linked_List/src/linked_list/linked_list.cpp

linked_list_iter.o:
	g++ -c ../Linked_List/src/linked_list/iterator.cpp -o linked_list_iter.o

stack.o:
	g++ -c ../Stack/src/lib/stack/stack.cpp

random_gen.o:
	g++ -c ../../../Tools/Cpp/random_gen.cpp

panic.o:
	g++ -c ../Stack/src/lib/misc/panic/panic.cpp

clean:
	del /s main.o graph.o dungeon.o linked_list.o linked_list_iter.o stack.o random_gen.o panic.o
#	rm -f main.o graph.o linked_list.o linked_list_iter.o panic.o