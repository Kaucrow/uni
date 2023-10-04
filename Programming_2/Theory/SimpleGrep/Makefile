sgrep: main.o
	g++ main.o -o ./bin/sgrep

main.o: ./src/main.cpp
	g++ ./src/main.cpp -c

clean:
	rm -f main.o