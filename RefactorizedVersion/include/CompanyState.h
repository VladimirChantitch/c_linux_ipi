#ifndef COMPANYSTATE_H
#define COMPANYSTATE_H

class CompanyState {
public:
    CompanyState();
    CompanyState(int companyId, int regimentId, int divisionId, int deadAmount, int killAmount, int prisonerAmount);
    ~CompanyState();

    int getCompanyId() const;
    int getRegimentId() const;
    int getDivisionId() const;
    int getDeadAmount() const;
    int getKillAmount() const;
    int getPrisonerAmount() const;

    void print() const;

private:
    int companyId;
    int regimentId;
    int divisionId;
    int deadAmount;
    int killAmount;
    int prisonerAmount;
};

#endif