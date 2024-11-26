#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct loanRecord {
    std::string loanID;
    int age;
    int income;
    int loanAmount;
    int creditScore;
    int monthsEmployed;
    int numCreditLines;
    float interestRate;
    int loanTerm;
    float dtiRatio;
    std::string education;
    std::string employment;
    std::string maritalStatus;
    std::string hasMortgage;
    std::string hasDependents;
    std::string loanPurpose;
    std::string hasCosigner;
    int defaults; // initialized for now until risk score and default calculations are implemented

    loanRecord(const std::string& _loanID, int _age, int _income, int _loanAmount, int _creditScore,
        int _monthsEmployed, int _numCreditLines, float _interestRate, int _loanTerm, float _dtiRatio,
        const std::string& _education, const std::string& _employment, const std::string& _maritalStatus, const std::string& _hasMortgage,
        const std::string& _hasDependents, const std::string& _loanPurpose, const std::string& _hasCosigner, int _defaults)
            : loanID(_loanID), age(_age), income(_income), loanAmount(_loanAmount), creditScore(_creditScore),
              monthsEmployed(_monthsEmployed), numCreditLines(_numCreditLines), interestRate(_interestRate),
              loanTerm(_loanTerm), dtiRatio(_dtiRatio), education(_education), employment(_employment),
              maritalStatus(_maritalStatus), hasMortgage(_hasMortgage), hasDependents(_hasDependents),
              loanPurpose(_loanPurpose), hasCosigner(_hasCosigner), defaults(_defaults) {}
};

// This function pares the csv file and loads the data into the records vector
void loadDataset(const std::string& filename, std::vector<loanRecord>& records) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "File failed to open: " << filename << std::endl;
        return;
    }
    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string loanID, education, employment, maritalStatus, hasMortgage, hasDependents, loanPurpose, hasCosigner;
        std::string token;
        int age, income, loanAmount, creditScore, monthsEmployed, numCreditLines, loanTerm, defaults;
        float interestRate, dtiRatio;

        getline(ss, loanID, ',');

        getline(ss, token, ',');
        age = std::stoi(token);

        getline(ss, token, ',');
        income = std::stoi(token);

        getline(ss, token, ',');
        loanAmount = std::stoi(token);

        getline(ss, token, ',');
        creditScore = std::stoi(token);

        getline(ss, token, ',');
        monthsEmployed = std::stoi(token);

        getline(ss, token, ',');
        numCreditLines = std::stoi(token);

        getline(ss, token, ',');
        interestRate = std::stof(token);

        getline(ss, token, ',');
        loanTerm = std::stoi(token);

        getline(ss, token, ',');
        dtiRatio = std::stof(token);

        getline(ss, education, ',');
        getline(ss, employment, ',');
        getline(ss, maritalStatus, ',');
        getline(ss, hasMortgage, ',');
        getline(ss, hasDependents, ',');
        getline(ss, loanPurpose, ',');
        getline(ss, hasCosigner, ',');

        getline(ss, token, ',');
        defaults = std::stoi(token);

        records.emplace_back(loanID, age, income, loanAmount, creditScore, monthsEmployed, numCreditLines,
                             interestRate, loanTerm, dtiRatio, education, employment, maritalStatus, hasMortgage,
                             hasDependents, loanPurpose, hasCosigner, defaults);
        }
        file.close();
}