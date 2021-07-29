CC = g++
CFLAGS = -Wall -Wextra -std=c++20
OBJECTS = $(filter-out test.cpp, $(wildcard *.cpp)) #Source files
NAME = dlox #Desired filename
TODELETE = *.o # the *.o should be the same as the objects

mt-collatz : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

.PHONY: test clean run

test:
	$(CC) $(CFLAGS) test.cpp -o test
	./test

clean:
	rm -f $(TODELETE)

run:	dlox
	valgrind -s --tool=memcheck --track-origins=yes ./dlox
