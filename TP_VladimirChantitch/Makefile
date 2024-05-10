CC=g++
CFLAGS=-c -Wall -Iinclude
LDFLAGS=-Wall

all: clean WarGame Division Regiment Company

WarGame: General.o Shm.o Semaphore.o CompanyState.o RandomNumber.o  Helper.o
	$(CC) $(LDFLAGS) General.o CompanyState.o Semaphore.o Helper.o Shm.o RandomNumber.o -o WarGame

Division: Division.o Shm.o
	$(CC) $(LDFLAGS) Division.o Semaphore.o CompanyState.o Helper.o Shm.o RandomNumber.o -o Division

Regiment: Regiment.o Shm.o
	$(CC) $(LDFLAGS) Regiment.o Semaphore.o  CompanyState.o Helper.o Shm.o RandomNumber.o -o Regiment

Company: Company.o Shm.o
	$(CC) $(LDFLAGS) Company.o Semaphore.o  CompanyState.o Helper.o Shm.o RandomNumber.o -o Company

General.o: src/General.cpp
	$(CC) $(CFLAGS) src/General.cpp -o General.o

Regiment.o: src/Regiment.cpp
	$(CC) $(CFLAGS) src/Regiment.cpp -o Regiment.o

Division.o: src/Division.cpp
	$(CC) $(CFLAGS) src/Division.cpp -o Division.o
	
Company.o: src/Company.cpp
	$(CC) $(CFLAGS) src/Company.cpp -o Company.o

Semaphore.o: src/Semaphore.cpp include/Semaphore.h
	$(CC) $(CFLAGS) src/Semaphore.cpp -o Semaphore.o

CompanyState.o: src/CompanyState.cpp include/CompanyState.h
	$(CC) $(CFLAGS) src/CompanyState.cpp -o CompanyState.o

Shm.o: src/Shm.cpp include/Shm.h
	$(CC) $(CFLAGS) src/Shm.cpp -o Shm.o

Helper.o: src/Helper.cpp include/Helper.h
	$(CC) $(CFLAGS) src/Helper.cpp -o Helper.o

RandomNumber.o: src/RandomNumber.cpp include/RandomNumber.h
	$(CC) $(CFLAGS) src/RandomNumber.cpp -o RandomNumber.o

clean:
	rm -rf *.o WarGame Division Regiment Company