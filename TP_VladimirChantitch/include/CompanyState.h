#ifndef COMPANYSTATE_H
#define COMPANYSTATE_H

#include <sstream>
#include <string>
#include <vector>

class CompanyState {
public:
    CompanyState();
    CompanyState(int divisionId, int regimentId, int companyId);
    ~CompanyState();

    std::string serialize();

    static std::vector<CompanyState> deserialize(const std::string& str);

    void print() const;

    int companyId; 
    int regimentId;
    int divisionId;
    int deadAmount;
    int killAmount;
    int prisonerAmount;
    int totalSoldiers = 1000;
};

#endif