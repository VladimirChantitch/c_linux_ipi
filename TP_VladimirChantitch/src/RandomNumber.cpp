#include "RandomNumber.h"

#include <random>
#include <ctime> 

RandomNumber::RandomNumber()
{
}

RandomNumber::~RandomNumber()
{
}

int RandomNumber::generateRandomNumber()
{
    std::random_device rd;
    std::mt19937 gen(rd()); 

    std::uniform_int_distribution<int> distribution(1, 1000000);
    srand(time(0));
    int randomNumber = 0;
    for (int i = 0; i < 10; ++i) 
        randomNumber = distribution(gen);

    return randomNumber;
}

int RandomNumber::generateRandomNumberInRange(int min, int max)
{
    std::random_device rd;
    std::mt19937 gen(rd()); 

    std::uniform_int_distribution<int> distribution(min, max);
    srand(time(0));
    int randomNumber = 0;
    for (int i = 0; i < 10; ++i) 
        randomNumber = distribution(gen);

    return randomNumber;
}
