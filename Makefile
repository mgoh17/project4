CC = gcc
CFLAGS = -ansi -Wall -g -O0 -Wwrite-strings -Wshadow -pedantic-errors -fstack-protector-all -Wextra
PROGS =  public01 public02 public03 public04 public05 student_tests

all: $(PROGS)

calendar.o: calendar.c calendar.h 
	$(CC) $(CFLAGS) -c calendar.c

my_memory_checker_216.o : my_memory_checker_216.c my_memory_checker_216.h
	$(CC) $(CFLAGS) -c my_memory_checker_216.c


public01: calendar.o public01.o my_memory_checker_216.o
	$(CC) -o public01 public01.o calendar.o my_memory_checker_216.o

public02: calendar.o public02.o my_memory_checker_216.o
	$(CC) -o public02 public02.o calendar.o my_memory_checker_216.o

public03: calendar.o public03.o my_memory_checker_216.o
	$(CC) -o public03 public03.o calendar.o my_memory_checker_216.o

public04: calendar.o public04.o my_memory_checker_216.o
	$(CC) -o public04 public04.o calendar.o my_memory_checker_216.o

public05: calendar.o public05.o my_memory_checker_216.o
	$(CC) -o public05 public05.o calendar.o my_memory_checker_216.o

student_tests: calendar.o student_tests.o my_memory_checker_216.o
	$(CC) -o student_tests student_tests.o calendar.o my_memory_checker_216.o


public01.o: public01.c my_memory_checker_216.h calendar.h
	$(CC) $(CFLAGS) -c public01.c

public02.o: public02.c my_memory_checker_216.h calendar.h
	$(CC) $(CFLAGS) -c public02.c

public03.o: public03.c my_memory_checker_216.h calendar.h
	$(CC) $(CFLAGS) -c public03.c

public04.o: public04.c my_memory_checker_216.h calendar.h
	$(CC) $(CFLAGS) -c public04.c

public05.o: public05.c my_memory_checker_216.h calendar.h
	$(CC) $(CFLAGS) -c public05.c

student_tests.o: student_tests.c my_memory_checker_216.h calendar.h
	$(CC) $(CFLAGS) -c student_tests.c


clean:
	@echo "Delete all object files and executables."
	rm -f *.o $(PROGS)

