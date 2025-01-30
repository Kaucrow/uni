bin/doubly_linked_list_example: main.o panic.o student.o csv_ops.o | bin
	g++ main.o panic.o student.o csv_ops.o -o bin/doubly_linked_list_example

bin:
	mkdir -p bin

main.o:
	g++ -c src/main.cpp

panic.o:
	g++ -c src/misc/panic.cpp

student.o:
	g++ -c src/classes/student/student.cpp

csv_ops.o:
	g++ -c src/misc/csv_ops.cpp

clean:
	rm main.o panic.o student.o csv_ops.o