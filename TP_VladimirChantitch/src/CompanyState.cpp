#include "CompanyState.h"
#include <iostream>
#include <sstream>
#include "RandomNumber.h"

CompanyState::CompanyState() {}

CompanyState::CompanyState(int divisionId, int regimentId, int companyId)
    : companyId(companyId), regimentId(regimentId), divisionId(divisionId), deadAmount(deadAmount), killAmount(killAmount), prisonerAmount(prisonerAmount) {}

CompanyState::~CompanyState() {}

void CompanyState::print() const {
    std::cout << "Company " << companyId << " of Division " << divisionId
              << " of Regiment " << regimentId << " reports: " << deadAmount << " loss, "
              << killAmount << " kills and " << prisonerAmount << " prisoner. \n";
}

std::string CompanyState::serialize(){
    std::stringstream stream;

    deadAmount = RandomNumber::generateRandomNumberInRange(5,50);
    totalSoldiers -= deadAmount;
    killAmount = RandomNumber::generateRandomNumberInRange(800,50000);
    prisonerAmount = RandomNumber::generateRandomNumberInRange(1,20);

    stream  << companyId << " " << regimentId << " " << divisionId << " "
            << deadAmount << " " << killAmount << " " << prisonerAmount << " ";
    return stream.str();
}

std::vector<CompanyState> CompanyState::deserialize(const std::string& str) {
    std::vector<CompanyState> result;
    std::istringstream iss(str);

    CompanyState state;
    while (iss >> state.companyId >> state.regimentId >> state.divisionId
              >> state.deadAmount >> state.killAmount >> state.prisonerAmount) {
        result.push_back(state);
    }

    return result;
}

