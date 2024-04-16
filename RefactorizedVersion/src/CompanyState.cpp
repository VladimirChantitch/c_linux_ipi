#include "CompanyState.h"
#include <iostream>

CompanyState::CompanyState() {}

CompanyState::CompanyState(int companyId, int regimentId, int divisionId, int deadAmount, int killAmount, int prisonerAmount)
    : companyId(companyId), regimentId(regimentId), divisionId(divisionId), deadAmount(deadAmount), killAmount(killAmount), prisonerAmount(prisonerAmount) {}

CompanyState::~CompanyState() {}

int CompanyState::getCompanyId() const { return companyId; }
int CompanyState::getRegimentId() const { return regimentId; }
int CompanyState::getDivisionId() const { return divisionId; }
int CompanyState::getDeadAmount() const { return deadAmount; }
int CompanyState::getKillAmount() const { return killAmount; }
int CompanyState::getPrisonerAmount() const { return prisonerAmount; }

void CompanyState::print() const {
    std::cout << "Company " << companyId << " of Division " << divisionId
              << " of Regiment " << regimentId << " reports: " << deadAmount << " loss, "
              << killAmount << " kills and " << prisonerAmount << " prisoner. \n";
}