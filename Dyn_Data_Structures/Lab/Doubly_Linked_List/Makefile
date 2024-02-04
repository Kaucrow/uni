bin/linked_list: main.o panic.o linked_list.o linked_list_iterator.o student.o csv_ops.o
	g++ main.o panic.o linked_list.o linked_list_iterator.o student.o csv_ops.o -o bin/linked_list

main.o:
	g++ -c src/main.cpp

panic.o:
	g++ -c src/lib/misc/panic.cpp

linked_list.o:
	g++ -c src/lib/linked_list/linked_list.cpp

linked_list_iterator.o:
	g++ -c src/lib/linked_list/iterator.cpp -o linked_list_iterator.o

student.o:
	g++ -c src/lib/student/student.cpp

csv_ops.o:
	g++ -c src/lib/misc/csv_ops.cpp

clean:
	del /s main.o panic.o student.o linked_list.o linked_list_iterator.o csv_ops.o