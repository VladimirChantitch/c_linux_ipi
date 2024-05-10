#include <iostream>
#include <Semaphore.h>
#include <Shm.h>
#include <Helper.h>
#include <unistd.h>
#include <CompanyState.h>
#include <cstring>
#include "RandomNumber.h"

int main(int argc, char *argv[]) {
    Semaphore *parentSemaphore;
    Shm *shmParent;

    std::string id = argv[2];

    std::vector<int> idVec = Helper::Get(argv, parentSemaphore, shmParent, id);

    CompanyState* s = new CompanyState(idVec[0],idVec[1],idVec[2]);

    while (s->totalSoldiers > 0)
    {
        std::string data = Helper::GetData(parentSemaphore, shmParent);

        data += s->serialize().c_str();
        Helper::SetData(parentSemaphore, shmParent, data.c_str());

        sleep(RandomNumber::generateRandomNumberInRange(0,5));
    }
}