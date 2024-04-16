CC=g++
CFLAGS=-c -Wall -Iinclude

all: WarGame

WarGame: main.o Semaphore.o CompanyStateContainer.o CompanyState.o ProcessMemory.o
	$(CC) main.o Semaphore.o CompanyStateContainer.o CompanyState.o ProcessMemory.o -o WarGame

main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o main.o

Semaphore.o: src/Semaphore.cpp include/Semaphore.h
	$(CC) $(CFLAGS) src/Semaphore.cpp -o Semaphore.o

CompanyState.o: src/CompanyState.cpp include/CompanyState.h
	$(CC) $(CFLAGS) src/CompanyState.cpp -o CompanyState.o

CompanyStateContainer.o: src/CompanyStateContainer.cpp include/CompanyStateContainer.h
	$(CC) $(CFLAGS) src/CompanyStateContainer.cpp -o CompanyStateContainer.o

ProcessMemory.o: src/ProcessMemory.cpp include/ProcessMemory.h
	$(CC) $(CFLAGS) src/ProcessMemory.cpp -o ProcessMemory.o

clean:
	rm -rf *.o WarGame
