bin/queue_reqs: main.o request.o queue.o linked_list.o linked_list_iter.o panic.o
	g++ main.o request.o queue.o linked_list.o linked_list_iter.o panic.o -o bin/queue_reqs

main.o:
	g++ -c src/main.cpp

request.o:
	g++ -c src/lib/request/request.cpp

queue.o:
	g++ -c ../Queue/src/lib/queue/queue.cpp

linked_list.o:
	g++ -c ../Linked_List/src/linked_list/linked_list.cpp

linked_list_iter.o:
	g++ -c ../Linked_List/src/linked_list/iterator.cpp -o linked_list_iter.o

panic.o:
	g++ -c src/lib/misc/panic/panic.cpp

clean:
	del /s main.o request.o queue.o linked_list.o linked_list_iter.o panic.o