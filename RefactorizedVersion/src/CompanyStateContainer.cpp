#include "CompanyStateContainer.h"
#include <iostream>
#include <cstring>

CompanyStateContainer::CompanyStateContainer() {}

CompanyStateContainer::CompanyStateContainer(int containerSize, int shmid) {
    companyStates = new CompanyState*[containerSize];
    size = containerSize;
    this->shmid = shmid;
    
    for (size_t i = 0; i < size; ++i) {
        companyStates[i] = nullptr;
    }
}

CompanyStateContainer::~CompanyStateContainer() {
    for (size_t i = 0; i < size; ++i) {
        delete companyStates[i];
    }

    delete[] companyStates;
}

size_t CompanyStateContainer::getSize() const {
    return size;
}

int CompanyStateContainer::getShmid() const {
    return shmid;
}

CompanyState* CompanyStateContainer::getCompanyStates(int index) const {
    return companyStates[index];
}

void CompanyStateContainer::add(const CompanyState& state) {
    if (currentIndex < size) {
        companyStates[currentIndex++] = new CompanyState(state);
    } else {
        std::cerr << "Error: Full. Cannot add more company states.\n";
    }
}

void CompanyStateContainer::add(const CompanyStateContainer& other) {
    if (currentIndex + other.getSize() <= size) {
        for (size_t i = 0; i < other.getSize(); ++i) {
            companyStates[currentIndex++] = new CompanyState(*(other.getCompanyStates(i)));
        }
    } else {
        std::cerr << "Error: Not enough space to add all states from the other container.\n";
    }
}

void CompanyStateContainer::print() {
    for (size_t i = 0; i < size; i++) {
        if (companyStates[i] != nullptr) {
            companyStates[i]->print();
        }
    }
}

void CompanyStateContainer::clear() {
    for (size_t i = 0; i < size; i++) {
        delete companyStates[i];
        companyStates[i] = nullptr; 
    }
    currentIndex = 0;
}