#ifndef RANDOMNUMBER_H
#define RANDOMNUMBER_H

class RandomNumber{
    public:
        RandomNumber();
        ~RandomNumber();

        static int generateRandomNumber();     
        static int generateRandomNumberInRange(int min, int max);     
};

#endif