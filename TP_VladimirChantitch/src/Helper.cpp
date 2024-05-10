#include "Helper.h"
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <cstring>
#include <sstream>

Helper::Helper()
{
}

Helper::~Helper()
{
}

std::vector<int> Helper::Get(char *argv[], Semaphore *&parentSemaphore, Shm *&shmParent, std::string id)
{
    parentSemaphore = new Semaphore(std::stol(argv[0]));
    shmParent = new Shm(std::stol(argv[1]));

    std::vector<int> result;
    std::istringstream iss(id);
    int num;
    while (iss >> num) {
        result.push_back(num);
    }
    return result;
}

void Helper::Gen(Semaphore *&childrensemaphore, Shm *&childrenShm, int size)
{
    childrenShm = Shm::createCompanyContainerSharedMemory(size);
    childrensemaphore = new Semaphore();
}

void Helper::CreateDivisions(Semaphore * &parentSemaphore, Shm * &parentShm, int amount, char* file, std::string identification)
{
    std::string newId = identification;
    for (int i = 0; i < amount; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            std::cerr << "Fork failed \n";
            exit(1);
        } else if (pid == 0) {
            std::stringstream stream;
            stream << " " << i;
            newId += stream.str();
            execl(file,
                std::to_string(parentSemaphore != nullptr ? parentSemaphore->getSemKey() : -1).c_str(),
                std::to_string(parentShm != nullptr ? parentShm->getShmKey() : -1).c_str(),
                newId.c_str(),
                NULL
            );

            std::cerr << "Execl failed \n";
            exit(1);
        }

        newId = identification;
    } 
}

void Helper::ClearMemory(Semaphore *&semaphore, Shm *&shmaddr)
{
    int shmid = shmaddr->getShmId();
    int semid = semaphore->getSemID();
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, 0);
}

std::string Helper::GetData(Semaphore *&semaphore, Shm *&shm, bool clear)
{
    semaphore->P();
    if (shm == nullptr)
        return "Null";

    char* data = shm->shm_ptr;
    std::string res = data;

    if(clear){
        std::strcpy(((char*)shm->shm_ptr), "");
    }

    semaphore->V();

    return res;
}

void Helper::SetData(Semaphore *&semaphore, Shm *&shm, const char *data)
{
    semaphore->P();

    std::strcpy(((char*)shm->shm_ptr), data);

    semaphore->V();
}
