#include <iostream>
#include <Semaphore.h>
#include <Shm.h>
#include <Helper.h>
#include <unistd.h>
#include <CompanyState.h>
#include "RandomNumber.h"

int main(int argc, char *argv[]) {
    Semaphore *parentSemaphore = nullptr;
    Semaphore *childrenSemaphore = nullptr;
    Shm *shmParent = nullptr;
    Shm *shmChildren = nullptr;

    std::string id = argv[2];

    Helper::Get(argv, parentSemaphore, shmParent, id);
    Helper::Gen(childrenSemaphore, shmChildren, REGIMENT_AMOUNT*COMPANY_AMOUNT);
    Helper::CreateDivisions(childrenSemaphore, shmChildren, REGIMENT_AMOUNT, "Regiment", id);
    
    // std::cout << parentSemaphore->getSemKey() << std::endl;
    // std::cout << shmParent->getShmKey() << std::endl;

    for (int i = 0; i < REGIMENT_AMOUNT * 100; i++) {
        std::string data = Helper::GetData(childrenSemaphore, shmChildren, true);

        data += Helper::GetData(parentSemaphore, shmParent);
        Helper::SetData(parentSemaphore, shmParent, data.c_str());
        
        sleep(RandomNumber::generateRandomNumberInRange(0,5));
    }

    Helper::ClearMemory(childrenSemaphore, shmChildren);
    return 0;
}