#ifndef Helper_H
#define Helper_H

#include "Semaphore.h"
#include "Shm.h"

#define DIVISION_AMOUNT 2
#define REGIMENT_AMOUNT 2
#define COMPANY_AMOUNT 2

/// @brief Helper class
class Helper{
    public:
        Helper
    ( );
        ~Helper
    ();

        static std::vector<int> Get(char *argv[], Semaphore* &parentSemaphore, Shm* &shmParent, std::string id);
        static void Gen(Semaphore* &childrensemaphore, Shm* &childrenShm, int size);
        static void CreateDivisions(Semaphore* &parentSemaphore, Shm* &parentShm, int amount, char* file, std::string identification);
        static void ClearMemory(Semaphore* &semaphore, Shm* &shmaddr);
        static std::string GetData(Semaphore* &semaphore, Shm* &shm, bool clear = false);
        static void SetData(Semaphore* &semaphore, Shm* &shm, const char* data);
};

#endif