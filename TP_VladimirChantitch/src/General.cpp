#include "Semaphore.h"
#include "Shm.h"
#include "Helper.h"
#include "CompanyState.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <string>


int main(int argc, char *argv[]) {
    Shm* shmChildren = nullptr;
    Semaphore* childrenSemaphore = nullptr;
    Helper::Gen(childrenSemaphore, shmChildren, DIVISION_AMOUNT*REGIMENT_AMOUNT*COMPANY_AMOUNT);
    Helper::CreateDivisions(childrenSemaphore, shmChildren, DIVISION_AMOUNT, "Division", "");


    std::vector<CompanyState> states;

    for (int i = 0; i < DIVISION_AMOUNT*100; i++) {
        if (shmChildren != nullptr)
            std::cout << "Report" << std::endl;
        std::cout << "__________________" << std::endl;
        std::string data = Helper::GetData(childrenSemaphore, shmChildren, true);
    
        states = CompanyState::deserialize(data);

        for (auto it = states.begin(); it != states.end(); ++it){
            (*it).print();
        }
        std::cout << "__________________" << std::endl << std::endl;
        sleep(1);
    }

    Helper::ClearMemory(childrenSemaphore, shmChildren);
    return 0;
}