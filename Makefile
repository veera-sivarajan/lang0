CC = g++
CFLAGS = -Wall -Wextra -std=c++20 
OBJECTS = $(filter-out test.cpp, $(wildcard *.cpp)) #Source files
NAME = interpreter #Desired filename
TODELETE = *.o # the *.o should be the same as the objects

mt-collatz : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

.PHONY: clean run

clean:
	rm -f $(TODELETE)

run:
	./interpreter
