ifdef OS
	RM=del
	BIN_NAME=blockbuster.exe
	ECHO_NL=echo.
else ifeq ($(shell uname), Linux)
	RM=rm -f
	BIN_NAME=blockbuster
	ECHO_NL=echo
endif

$(BIN_NAME): main.o
	g++ main.o -o ./bin/$(BIN_NAME)
	@echo INFO: Binary was built in ./bin/$(BIN_NAME)
	@$(ECHO_NL)

main.o: ./src/main.cpp
	@$(ECHO_NL)
	g++ ./src/main.cpp -c

clean:
	@$(RM) *.o