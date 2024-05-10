#ifndef SHM_H
#define SHM_H

#include <set>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

class Shm {
public:
    Shm(int key, int size);
    Shm(int key);
    ~Shm();

    void addSerializedCompanyState(const std::string& serializedState);
    std::string getAllSerializedCompanyStates() const;
    static Shm* createCompanyContainerSharedMemory(int size);

    int getShmKey();
    int getShmId();

    char* shm_ptr = static_cast<char*>(shmat(shm_id, NULL, 0));
private:
    int size;
    int shm_key;
    int shm_id;
};

#endif