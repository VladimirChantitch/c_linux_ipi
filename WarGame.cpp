#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <cstring>

#define SHM_SIZE 1024
#define MAX_DEPTH 3 // it will start at 0
#define DIVISION_AMOUNT 1
#define REGIMENT_AMOUNT 1
#define COMPANIES_AMOUNT 1

class Semaphore {
public:
    Semaphore(int key) {
        sem_id = semget(key, 1, IPC_CREAT | 0666);
        sem_union.val = 1;
        semctl(sem_id, 0, SETVAL, sem_union);
    }

    ~Semaphore() {
        semctl(sem_id, 0, IPC_RMID);
    }

   void P() {
        struct sembuf sops = {0, -1, 0};
        semop(sem_id, &sops, 1);
    }

    void V() {
        struct sembuf sops = {0, 1, 0};
        semop(sem_id, &sops, 1);
    }

    int getSemID() const{
        return sem_id;
    }

private:
    int sem_id;
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_union;
};

class CompanyState {
public:
    CompanyState(){}

    CompanyState(int companyId, int regimentId, int divisionId, int deadAmount, int killAmount, int prisonerAmount)
    : companyId(companyId), regimentId(regimentId), divisionId(divisionId),
        deadAmount(deadAmount), killAmount(killAmount), prisonnerAmount(prisonerAmount) {}

    ~CompanyState() {

    }
    
    int getCompanyId() const { return companyId; }
    int getRegimentId() const { return regimentId; }
    int getDivisionId() const { return divisionId; }
    int getDeadAmount() const { return deadAmount; }
    int getKillAmount() const { return killAmount; }
    int getPrisonnerAmount() const { return prisonnerAmount; }

    void print() const {
        std::cout << "Company " << companyId << " of Division " << divisionId
            << " of Regiment " << regimentId << " reports: " << deadAmount << " loss, "
            << killAmount << " kills and " << prisonnerAmount << " prisonner. \n";
    }

private:
    int companyId;
    int regimentId;
    int divisionId;
    int deadAmount;
    int killAmount;
    int prisonnerAmount;
};

class CompanyStateContainer {
public:
    CompanyStateContainer(){}

    CompanyStateContainer(int containerSize, int shmid){
        companyStates = new CompanyState*[containerSize];
        size = containerSize;
        this->shmid = shmid;
        
        for (size_t i = 0; i < size; ++i) {
            companyStates[i] = nullptr;
        }
    }

    ~CompanyStateContainer(){
        for (size_t i = 0; i < size; ++i) {
            delete companyStates[i];
        }

        delete[] companyStates;
    }

    size_t getSize() const {
        return size;
    }

    int getShmid() const{
        return shmid;
    }

    CompanyState* getCompanyStates(int index) const{
        return companyStates[index];
    }

    void add(const CompanyState& state) {
        if (currentIndex < size) {
            companyStates[currentIndex++] = new CompanyState(state);
        } else {
            std::cerr << "Error: Full. Cannot add more company states.\n";
        }
    }

    void add(const CompanyStateContainer& other) {
        if (currentIndex + other.getSize() <= size) {
            for (size_t i = 0; i < other.getSize(); ++i) {
                companyStates[currentIndex++] = new CompanyState(*(other.getCompanyStates(i)));
            }
        } else {
            std::cerr << "Error: Not enough space to add all states from the other container.\n";
        }
    }

    void print(){
        for (size_t i = 0; i < size; i++) {
            if (companyStates[i] != nullptr) {
                companyStates[i]->print();
            }
        }
    }

    void clear() {
        for (size_t i = 0; i < size; i++) {
            delete companyStates[i];
            companyStates[i] = nullptr; 
        }
        currentIndex = 0;
    }

private:
    size_t size;
    size_t currentIndex = 0;
    CompanyState** companyStates;
    int shmid = -1;
};

CompanyStateContainer* createCompanyContainerSharedMemory(int containerSize){
    key_t shm_key = ftok("shmfile", 65);
    int shmid = shmget(shm_key, sizeof(CompanyStateContainer), IPC_CREAT | 0666);
    CompanyStateContainer* shmaddr = (CompanyStateContainer*) shmat(shmid, (void*)0, 0);

    new (shmaddr) CompanyStateContainer(containerSize, shmid);

    return shmaddr;
}

Semaphore createSemaphore(){
    key_t sem_key = ftok("semfile", 65);
    Semaphore semaphore(sem_key);
    return semaphore;
}

void clearMemory(Semaphore semaphore, CompanyStateContainer* shmaddr){
    int shmid = shmaddr->getShmid();
    int semid = semaphore.getSemID();
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, 0);
}

int getExpectedAmountOfChildren(int currentDepth){
    switch(currentDepth){
        case 0:
            return DIVISION_AMOUNT;
        case 1:
            return REGIMENT_AMOUNT;
        case 2:
            return COMPANIES_AMOUNT;
    }

    return 0;
}

void createProcess(int depth, int max_depth, Semaphore& semaphore, CompanyStateContainer* shmaddr){
    if (depth >= max_depth){ // stop propagation condition
        std::cout << "im the child of depth " << depth << "\n";
        return;
    }

    int expectedChildren = getExpectedAmountOfChildren(depth);
    for (int i = 0; i < expectedChildren; i++){
        pid_t pid = fork();

        if (pid < 0){
            std::cerr << "Fork failled \n";
            exit(1);
        } else if (pid == 0){
            semaphore.P();
            CompanyState companyState(1, 2, 3, 4, 5, 6);
            //memcpy(shmaddr, &companyState, sizeof(CompanyState));

            CompanyStateContainer retrievedCompanyContainer;
            memcpy(&retrievedCompanyContainer, shmaddr, sizeof(CompanyStateContainer));

            retrievedCompanyContainer.add(companyState);

            memcpy(shmaddr, &retrievedCompanyContainer, sizeof(CompanyState));
            
            memcpy(&retrievedCompanyContainer, shmaddr, sizeof(CompanyStateContainer));

            retrievedCompanyContainer.print();

            retrievedCompanyContainer.clear();
            memcpy(shmaddr, &retrievedCompanyContainer, sizeof(CompanyState));

            semaphore.V();
            createProcess(depth + 1, MAX_DEPTH, semaphore, shmaddr);
            exit(0);
        }else{
            std::cout << "I'm parent with pid :: " << pid << "\n";
            // comportement en fonction de la valeur de la depth
                // regarder dans la shma enfant;
            wait(NULL);
        }
    } 
}

int main(int argc, char *argv[]){
    CompanyStateContainer* shmaddr = createCompanyContainerSharedMemory(DIVISION_AMOUNT);
    Semaphore semaphore = createSemaphore();
    createProcess(0, MAX_DEPTH, semaphore, shmaddr);

    clearMemory(semaphore, shmaddr);
    return 0;
}
