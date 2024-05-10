#include "Shm.h"
#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "RandomNumber.h"
#include <unistd.h>
#include <sys/ipc.h>
#include <string.h>
#include <cstring>

Shm::Shm(int key, int size) : size(size) {
    this->shm_key = key;
    int segment_size = sizeof(std::vector<std::string>);
    int shmid = shmget(shm_key, segment_size, IPC_CREAT | 0666);
    if (shmid == -1) {
        std::cout << "Cannot create or access SHM with key: " << key << std::endl;
    } else {
        this->shm_id = shmid;
        char* shmaddr = (char*) shmat(shmid, nullptr, 0);
        if (shmaddr == (char*) -1) {
            std::cout << "Failed to attach to shared memory" << std::endl;
        } else {
            this->shm_ptr = shmaddr;
        }
    }
}

Shm::Shm(int key) : size(0) {
    this->shm_key = key;
    int shmid = shmget(shm_key, 0, 0);
    if (shmid == -1) {
        std::cout << "Cannot find SHM of id :: " << key << std::endl;
    } else {
        this->shm_id = shmid;
        char* shmaddr = (char*) shmat(shmid, nullptr, 0);
        if (shmaddr == (char*) -1) {
            std::cout << "Can't find shared memory" << std::endl;
        } else {
            this->shm_ptr = shmaddr;
            if(shmaddr == nullptr)
                std::cout << "Can't attach the shared memory" << std::endl;
        }
    }
}

Shm::~Shm() {
    // if (shmaddr != nullptr) {
    //     shmdt(shmaddr);
    // }
}

void Shm::addSerializedCompanyState(const std::string& serializedState) {
    std::strcpy(shm_ptr, serializedState.c_str());
}

std::string Shm::getAllSerializedCompanyStates() const {
    if ((char*)shm_ptr == (char*) -1) {
        perror("shmat");
        return "";
    }

    // Read data from the shared memory segment
    std::string received_data(shm_ptr);
    std::cout << "Received data: " << received_data << std::endl;

    // Detach from the shared memory segment
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        return "";
    }

    return received_data;
}

Shm* Shm::createCompanyContainerSharedMemory(int size) {
    int shm_key = RandomNumber::generateRandomNumber(); // You can use any key value here
    return new Shm(shm_key, size);
}

int Shm::getShmKey()
{
    return shm_key;
}

int Shm::getShmId()
{
    return shm_id;
}
