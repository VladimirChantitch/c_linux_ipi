#ifndef SEMAPHORE_H
#define SEMAPHORE_H

class Semaphore {
public:
    Semaphore(int key);
    Semaphore();
    ~Semaphore();

    void P();
    void V();
    int getSemID() const;
    int getSemKey();



    void print();

private:
    int sem_id;
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_union;
    int sem_key;
};

#endif