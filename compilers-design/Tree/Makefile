bin/tree: main.o tree.o linked_list.o linked_list_iter.o panic.o
	g++ main.o tree.o linked_list.o linked_list_iter.o panic.o -o bin/tree

main.o:
	g++ -c src/main.cpp

tree.o:
	g++ -c src/lib/tree/tree.cpp

linked_list.o:
	g++ -c ../Linked_List/src/linked_list/linked_list.cpp

linked_list_iter.o:
	g++ -c ../Linked_List/src/linked_list/iterator.cpp -o linked_list_iter.o

panic.o:
	g++ -c ../Stack/src/lib/misc/panic/panic.cpp

clean:
#	del /s main.o tree.o linked_list.o linked_list_iter.o panic.o
	rm -f main.o tree.o linked_list.o linked_list_iter.o panic.o