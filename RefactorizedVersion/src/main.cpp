#include "Semaphore.h"
#include "CompanyStateContainer.h"
#include "ProcessMemory.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <ctime> 
#include <cstdlib>

#define MAX_DEPTH 3 // it will start at 0
#define DIVISION_AMOUNT 1 // min 1
#define REGIMENT_AMOUNT 1 // min 1
#define COMPANIES_AMOUNT 1 // min 1

void clearMemory(Semaphore* semaphore, CompanyStateContainer* shmaddr) {
    int shmid = shmaddr->getShmid();
    int semid = semaphore->getSemID();
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, 0);
}

int getExpectedAmountOfChildren(int currentDepth) {
    switch(currentDepth) {
        case 0:
            return DIVISION_AMOUNT;
        case 1:
            return REGIMENT_AMOUNT;
        case 2:
            return COMPANIES_AMOUNT;
    }
    return 0;
}

int getExpectedContainerSize(int currentDepth){
        switch(currentDepth) {
        case 0:
            return DIVISION_AMOUNT * REGIMENT_AMOUNT * COMPANIES_AMOUNT;
        case 1:
            return REGIMENT_AMOUNT * COMPANIES_AMOUNT;
        case 2:
            return COMPANIES_AMOUNT;
    }
    return 0;
}

int generateRandomNumber() {
    srand(time(nullptr));
    int randomNumber = rand();

    return randomNumber;
}

CompanyStateContainer* createCompanyContainerSharedMemory(int containerSize) {
    key_t shm_key = ftok("shmfile", generateRandomNumber());

    int shmid = shmget(shm_key, sizeof(CompanyStateContainer) + containerSize * sizeof(CompanyState), IPC_CREAT | 0666);

    CompanyStateContainer* shmaddr = (CompanyStateContainer*) shmat(shmid, NULL, 0);

    new (shmaddr) CompanyStateContainer(containerSize, shmid);

    return shmaddr;
}

Semaphore* createSemaphore() {
    key_t sem_key = ftok("semfile", generateRandomNumber());
    Semaphore* semaphore = new Semaphore(sem_key);
    return semaphore;
}

void createProcess(int depth, int max_depth, Semaphore* parentSemaphore, CompanyStateContainer* parentContainer, ProcessMemory* parentProcessMemory) {
    if (depth >= max_depth) {
        std::cout << "I'm the child of depth " << depth << "\n";
        while(true){
            sleep(1);
            parentProcessMemory->ReportToParent();
        }
        return;
    }

    int expectedChildren = getExpectedAmountOfChildren(depth);
    for (int i = 0; i < expectedChildren; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "Fork failed \n";
            exit(1);
        } else if (pid == 0) {
            CompanyStateContainer* childrenContainer = createCompanyContainerSharedMemory(getExpectedContainerSize(depth));
            Semaphore* childrenSemaphore = createSemaphore();
            if (depth + 1 < max_depth){
                ProcessMemory processMemory(&*parentContainer, &*parentSemaphore, &*childrenContainer, &*childrenSemaphore);

                createProcess(depth + 1, max_depth, childrenSemaphore, childrenContainer, &processMemory);

                clearMemory(&*childrenSemaphore, &*childrenContainer);
                exit(0);
            }else{
                ProcessMemory processMemory(&*parentContainer, &*parentSemaphore, true);

                createProcess(depth + 1, max_depth, childrenSemaphore, childrenContainer, &processMemory);

                clearMemory(&*childrenSemaphore,&* childrenContainer);
                exit(0);
            }
        } else {
            std::cout << "I'm parent with pid :: " << pid << "\n";
            while(true){
                sleep(1);
                parentProcessMemory->ReportToParent();
            }
        }
    } 
}

int main(int argc, char *argv[]) {
    CompanyStateContainer* childrenContainer = createCompanyContainerSharedMemory(getExpectedContainerSize(0));
    Semaphore* childrenSemaphore = createSemaphore();
    ProcessMemory processMemory(&*childrenContainer, &*childrenSemaphore);
    createProcess(0, MAX_DEPTH, &*childrenSemaphore, &*childrenContainer, &processMemory);

    clearMemory(&*childrenSemaphore, &*childrenContainer);
    return 0;
}