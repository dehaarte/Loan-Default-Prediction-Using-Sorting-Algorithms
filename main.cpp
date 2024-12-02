#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

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
    float riskScore;

    loanRecord(const std::string& _loanID, int _age, int _income, int _loanAmount, int _creditScore,
        int _monthsEmployed, int _numCreditLines, float _interestRate, int _loanTerm, float _dtiRatio,
        const std::string& _education, const std::string& _employment, const std::string& _maritalStatus, const std::string& _hasMortgage,
        const std::string& _hasDependents, const std::string& _loanPurpose, const std::string& _hasCosigner, int _defaults)
        : loanID(_loanID), age(_age), income(_income), loanAmount(_loanAmount), creditScore(_creditScore),
        monthsEmployed(_monthsEmployed), numCreditLines(_numCreditLines), interestRate(_interestRate),
        loanTerm(_loanTerm), dtiRatio(_dtiRatio), education(_education), employment(_employment),
        maritalStatus(_maritalStatus), hasMortgage(_hasMortgage), hasDependents(_hasDependents),
        loanPurpose(_loanPurpose), hasCosigner(_hasCosigner), defaults(_defaults), riskScore(0.0f) {}
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

// This function retrieves the the user's chosen attribute from the dataset
float chosenAttribute(const loanRecord& record, const std::string& attribute) {
    if (attribute == "age") return record.age;
    if (attribute == "income") return record.income;
    if (attribute == "loanAmount") return record.loanAmount;
    if (attribute == "creditScore") return record.creditScore;
    if (attribute == "monthsEmployed") return record.monthsEmployed;
    if (attribute == "numCreditLines") return record.numCreditLines;
    if (attribute == "interestRate") return record.interestRate;
    if (attribute == "loanTerm") return record.loanTerm;
    if (attribute == "dtiRatio") return record.dtiRatio;
    // Might add more if statements to sort other attributes if necessary;
}

void heapifyDown(std::vector<loanRecord>& dataset, int size, int root, const std::string& attribute, int order) {
    int maxOrMin = root;
    int left = 2 * root + 1;
    int right = 2 * root + 2;

    if (order == 1) {
        if (left < size && chosenAttribute(dataset[left], attribute) > chosenAttribute(dataset[maxOrMin], attribute)) {
            maxOrMin = left;
        }
        if (right < size && chosenAttribute(dataset[right], attribute) > chosenAttribute(dataset[maxOrMin], attribute)) {
            maxOrMin = right;
        }
    }
    if (order == 2) {
        if (left < size && chosenAttribute(dataset[left], attribute) < chosenAttribute(dataset[maxOrMin], attribute)) {
            maxOrMin = left;
        }
        if (right < size && chosenAttribute(dataset[right], attribute) < chosenAttribute(dataset[maxOrMin], attribute)) {
            maxOrMin = right;
        }
    }
    if (maxOrMin != root) {
        std::swap(dataset[root], dataset[maxOrMin]);
        heapifyDown(dataset, size, maxOrMin, attribute, order);
    }
}

void heapSort(std::vector<loanRecord>& dataset, const std::string& attribute, int order) {
    int n = dataset.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyDown(dataset, n, i, attribute, order);
    }

    for (int i = n - 1; i > 0; i--) {
        std::swap(dataset[0], dataset[i]);
        heapifyDown(dataset, i, 0, attribute, order);
    }
}

// Calculates risk scored by weighted values
float calculateRiskScore(const loanRecord& record) {
    const float creditScoreWeight = .30f; //30%
    const float dtiRatioWeight = .20f; //20%
    const float incomeWeight = .15f; // 15 %
    const float loanAmountWeight = .10f; //10%
    const float monthsEmployedWeight = .10f; // 10%
    const float interestRateWeight = .05f; // 5%
    const float loanTermWeight = .05f; // 5%
    const float numCreditLinesWeight = .05f; //5%

    float riskScore = 0.0f; //default

    //Credit Score Inversion and risk score factor calculation
    float maxCreditScore = 850.0f;
    float minCreditScore = 300.0f;
    float creditScoreComponent = (maxCreditScore - record.creditScore) / (maxCreditScore - minCreditScore);
    riskScore += creditScoreWeight * creditScoreComponent;

    //dti ration risk score factoring.
    //Higher DTI = Higher risk
    float dtiRatioComponent = record.dtiRatio / 100.0f;
    riskScore += dtiRatioWeight * dtiRatioComponent;

    //Income risk score
    float maxIncome = 150000.0f; //Highest income in the data set is 149999
    float incomeComponent = 1.0f - (static_cast<float>(record.income) / maxIncome); //Converts income from parsed data to float to allow caluculation
    if (incomeComponent < 0.0f) {
        incomeComponent = 0.0f;
    }
    riskScore += incomeWeight * incomeComponent;


    //Loan risk score factor
    float maxLoanAmount = 250000.0f;
    float loanAmountComponent = static_cast<float>(record.loanAmount) / maxLoanAmount;
    riskScore += loanAmountWeight * loanAmountComponent;

    //Months Employed risk factor
    float maxMonthsEmployed = 120.0f;
    float monthsEmployedComponent = 1.0f - (static_cast<float>(record.monthsEmployed) / maxMonthsEmployed);
    if (monthsEmployedComponent < 0.0f) {
        monthsEmployedComponent = 0.0f;
    }
    riskScore += monthsEmployedWeight * monthsEmployedComponent;

    //Interest Rate Risk Factor
    float maxInterestRate = 25.0f;
    float interestRateComponent = record.interestRate / maxInterestRate;
    riskScore += interestRateWeight * interestRateComponent;

    //Loan Term Risk Factor
    float maxLoanTerm = 60.0f;
    float loanTermComponent = static_cast<float>(record.loanTerm) / maxLoanTerm;
    riskScore += loanTermWeight * loanTermComponent;

    //Num Credit Lines Risk Factor
    float maxCreditLines = 4.0f;
    float numCreditLinesComponent = static_cast<float>(record.numCreditLines) / maxCreditLines;
    riskScore += numCreditLinesWeight * numCreditLinesComponent;

    if (riskScore > 1.0f) {
        riskScore = 1.0f;
    }
    if (riskScore < 0.0f) {
        riskScore = 0.0f;
    }

    return riskScore;
}

//Default status function, if risk score >= .70 then default = 1
int defaultStatus(float riskScore, float threshold = .7f) {
    if (riskScore >= threshold) {
        return 1;
    }
    else {
        return 0;
    }
}

int main() {

    // placeholder code below to stop error, the actually main function code will be implemented later.
    std::cout << "Program starts" << std::endl;
    return 0;
}