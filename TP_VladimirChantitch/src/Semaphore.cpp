#include "Semaphore.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>
#include "RandomNumber.h"

Semaphore::Semaphore(int key) {
    sem_key = key;
    sem_id = semget(key, 1, IPC_CREAT | 0666);
    sem_union.val = 1;
    semctl(sem_id, 0, SETVAL, sem_union);
}

Semaphore::Semaphore()
{
    key_t sem_key = ftok("semfile", RandomNumber::generateRandomNumber());

    this->sem_key = sem_key;
    sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
    sem_union.val = 1;
    semctl(sem_id, 0, SETVAL, sem_union);
}

Semaphore::~Semaphore() {
    semctl(sem_id, 0, IPC_RMID);
}

void Semaphore::P() {
    struct sembuf sops = {0, -1, 0};
    semop(sem_id, &sops, 1);
}

void Semaphore::V() {
    struct sembuf sops = {0, 1, 0};
    semop(sem_id, &sops, 1);
}

int Semaphore::getSemID() const {
    return sem_id;
}

int Semaphore::getSemKey()
{
    return sem_key;
}

void Semaphore::print()
{
    std::cout << "sem_id = " << sem_id << " && sem_key = " << sem_key << std::endl;
}
