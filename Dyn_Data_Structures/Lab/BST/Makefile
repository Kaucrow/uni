bin/bst: main.o bst.o linked_list.o linked_list_iter.o panic.o
	g++ main.o bst.o linked_list.o linked_list_iter.o panic.o -o bin/bst

main.o:
	g++ -c src/main.cpp

bst.o:
	g++ -c src/lib/BST/bst.cpp

linked_list.o:
	g++ -c ../Linked_List/src/linked_list/linked_list.cpp

linked_list_iter.o:
	g++ -c ../Linked_List/src/linked_list/iterator.cpp -o linked_list_iter.o

panic.o:
	g++ -c ../Stack/src/lib/misc/panic/panic.cpp

clean:
	del /s main.o bst.o linked_list.o linked_list_iter.o panic.o