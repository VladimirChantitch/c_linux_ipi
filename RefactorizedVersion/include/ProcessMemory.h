#ifndef PROCESSMEMORY_H
#define PROCESSMEMORY_H

#include "CompanyStateContainer.h"
#include "Semaphore.h"

class ProcessMemory{
public :
    ProcessMemory( CompanyStateContainer* parentContainer,  Semaphore* parentSemaphore,  CompanyStateContainer* childrenContainer,  Semaphore* childrenSemaphore);
    ProcessMemory( CompanyStateContainer* childrenContainer,  Semaphore* childrenSemaphore);
    ProcessMemory( CompanyStateContainer* parentContainer,  Semaphore* parentSemaphore, bool _b);
    ~ProcessMemory();

    CompanyStateContainer* getParentContainer() const;
    CompanyStateContainer* getChildrenContainer() const;

    void ReportToParent();

private :
    CompanyStateContainer* parentContainer;
    CompanyStateContainer* childrenContainer;
    Semaphore* parentSemaphore;
    Semaphore* childrenSemaphore;
};

#endif