#include "ProcessMemory.h"
#include <iostream>

ProcessMemory::ProcessMemory(CompanyStateContainer* parentContainer, Semaphore* parentSemaphore, CompanyStateContainer* childrenContainer, Semaphore* childrenSemaphore){
    this->parentContainer = parentContainer;
    this->parentSemaphore = parentSemaphore;
    this->childrenContainer = childrenContainer;
    this->childrenSemaphore = childrenSemaphore;
}

ProcessMemory::ProcessMemory(CompanyStateContainer* childrenContainer, Semaphore* childrenSemaphore){
    this->parentContainer = nullptr;
    this->parentSemaphore = nullptr;
    this->childrenContainer = childrenContainer;
    this->childrenSemaphore = childrenSemaphore;
}

ProcessMemory::ProcessMemory( CompanyStateContainer* parentContainer,  Semaphore* parentSemaphore, bool _b){
    this->parentContainer = parentContainer;
    this->parentSemaphore = parentSemaphore;
    this->childrenContainer = nullptr;
    this->childrenSemaphore = nullptr;
}

ProcessMemory::~ProcessMemory() {}

CompanyStateContainer* ProcessMemory::getParentContainer() const {
    return parentContainer;
}

CompanyStateContainer* ProcessMemory::getChildrenContainer() const {
    return childrenContainer;
}

void ProcessMemory::ReportToParent() {
    if (parentContainer != nullptr && childrenContainer != nullptr) {//has a parent and just reports
        std::cout << "I'm just a relay" << "\n";
        // parentSemaphore->P();
        // childrenSemaphore->P();

        // parentContainer->add(*childrenContainer);
        // childrenContainer->clear();

        childrenContainer->print();

        // childrenSemaphore->V();
        // parentSemaphore->V();
    }else if (childrenContainer == nullptr && parentContainer != nullptr){ //its the last child
        std::cout << "I'm a company" << "\n";
        parentSemaphore->P();
    
        CompanyState companyState(1, 2, 3, 4, 5, 6);
        parentContainer->add(companyState);

        parentSemaphore->V();
    }else if (childrenContainer != nullptr && parentContainer == nullptr){ //its the first parent
        std::cout << "I'm the general" << "\n";
        childrenContainer->print();
        childrenSemaphore->P();

        childrenContainer->clear();

        childrenSemaphore->V();
    }
}