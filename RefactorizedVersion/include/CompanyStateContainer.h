#ifndef COMPANYSTATECONTAINER_H
#define COMPANYSTATECONTAINER_H

#include "CompanyState.h"
#include <cstddef>

class CompanyStateContainer {
public:
    CompanyStateContainer();
    CompanyStateContainer(int containerSize, int shmid);
    ~CompanyStateContainer();

    size_t getSize() const;
    int getShmid() const;
    CompanyState* getCompanyStates(int index) const;

    void add(const CompanyState& state);
    void add(const CompanyStateContainer& other);
    void print();
    void clear();

private:
    size_t size;
    size_t currentIndex = 0;
    CompanyState** companyStates;
    int shmid = -1;
};

#endif