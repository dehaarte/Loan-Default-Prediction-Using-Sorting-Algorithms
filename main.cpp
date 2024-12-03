#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

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
    int calculatedDefaults;
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
    if (attribute == "defaults") return record.defaults;
    if (attribute == "calculatedDefaults") return record.calculatedDefaults;
    return 0;
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

std::vector<loanRecord> merge(const std::vector<loanRecord>& first, const std::vector<loanRecord>& second, const std::string& attribute, int order) {
    std::vector<loanRecord> merged;
    size_t i = 0, j = 0;

    while (i < first.size() && j < second.size()) {
        bool condition = (order == 1) ?
                         (chosenAttribute(first[i], attribute) <= chosenAttribute(second[j], attribute)) :
                         (chosenAttribute(first[i], attribute) >= chosenAttribute(second[j], attribute));
        if (condition) {
            merged.push_back(first[i]);
            ++i;
        } else {
            merged.push_back(second[j]);
            ++j;
        }
    }

    while (i < first.size()) {
        merged.push_back(first[i]);
        ++i;
    }

    while (j < second.size()) {
        merged.push_back(second[j]);
        ++j;
    }

    return merged;
}

std::vector<loanRecord> mergeSort(const std::vector<loanRecord>& records, const std::string& attribute, int order) {
    if (records.size() <= 1) {
        return records;
    }

    size_t mid = records.size() / 2;
    std::vector<loanRecord> left(records.begin(), records.begin() + mid);
    std::vector<loanRecord> right(records.begin() + mid, records.end());

    left = mergeSort(left, attribute, order);
    right = mergeSort(right, attribute, order);

    return merge(left, right, attribute, order);
}

double measureExecutionTime(std::vector<loanRecord>& records, const std::string& attribute, int order, int algorithm) {
    auto start = std::chrono::high_resolution_clock::now();

    if (algorithm == 1) {
        heapSort(records, attribute, order);
    } else if (algorithm == 2) {
        records = mergeSort(records, attribute, order);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    return elapsed.count();
}

void compareSortingAlgorithms(std::vector<loanRecord>& records, const std::string& attribute, int order) {
    std::vector<loanRecord> heapRecords = records;
    std::vector<loanRecord> mergeRecords = records;

    double heapTime = measureExecutionTime(heapRecords, attribute, order, 1);
    double mergeTime = measureExecutionTime(mergeRecords, attribute, order, 2);

    std::cout << "\nHeap Sort completed in: " << heapTime << " seconds.";
    std::cout << "\nMerge Sort completed in: " << mergeTime << " seconds.\n";

    if (heapTime < mergeTime) {
        std::cout << "Heap Sort was faster by " << (mergeTime - heapTime) << " seconds.\n";
    } else if (mergeTime < heapTime) {
        std::cout << "Merge Sort was faster by " << (heapTime - mergeTime) << " seconds.\n";
    } else {
        std::cout << "Both algorithms performed equally.\n";
    }
}

int main() {
    std::vector<loanRecord> records;
    loadDataset("Loan_default.csv", records);

    if (records.empty()) {
        std::cerr << "Dataset failed to load!" << std::endl;
        return 1;
    }
    int menuChoice;

    while(true) {
        std::cout <<"\n=============================\n";
        std::cout << "Loan Menu Options";
        std::cout <<"\n=============================\n";
        std::cout << "1. Sort Dataset\n";
        std::cout << "2. Comparing Sorting Algorithms\n";
        std::cout << "3. Exit";
        std::cout <<"\n=============================\n";
        std::cout << "Enter your choice: ";
        std::cin >> menuChoice;

        if (menuChoice == 1) {
            while (true) {
                std::string attribute;
                std::cout <<"\n=============================\n";
                std::cout << "Menu Option 1: Sort Dataset\n";
                std::cout << "----------------------------\n";
                std::cout << "Choose the column to sort:";
                std::cout <<"\n=============================\n";
                std::cout << "1. Age\n";
                std::cout << "2. Income\n";
                std::cout << "3. Loan Amount\n";
                std::cout << "4. Credit Score\n";
                std::cout << "5. Months Employed\n";
                std::cout << "6. Number of Credit Lines\n";
                std::cout << "7. Interest Rate\n";
                std::cout << "8. Loan Term\n";
                std::cout << "9. DTI Ratio\n";
                std::cout << "10. Defaults (Original)\n"; // implemented for testing the original default attribute in the dataset.
                std::cout << "11. Defaults (Calculated)\n";
                std::cout <<"=============================\n";
                std::cout << "Enter your choice: ";
                int attributeChoice;
                std::cin >> attributeChoice;

                if (attributeChoice == 1) {
                    attribute = "age";
                } else if (attributeChoice == 2) {
                    attribute = "income";
                } else if (attributeChoice == 3) {
                    attribute = "loanAmount";
                } else if (attributeChoice == 4) {
                    attribute = "creditScore";
                } else if (attributeChoice == 5) {
                    attribute = "monthsEmployed";
                }else if (attributeChoice == 6) {
                    attribute = "numCreditLines";
                }else if (attributeChoice == 7) {
                    attribute = "interestRate";
                }else if (attributeChoice == 8) {
                    attribute = "loanTerm";
                }else if (attributeChoice == 9) {
                    attribute = "dtiRatio";
                } else if (attributeChoice == 10) {
                    attribute = "defaults";
                } else if (attributeChoice == 11) {
                    attribute = "calculatedDefaults";
                } else {
                    std::cout << "Invalid attribute choice.\n";
                    continue;
                }
                // Sorting Order Menu
                int order;
                std::cout <<"\n=============================\n";
                std::cout << "Choose sorting order:\n";
                std::cout << "1. Ascending\n";
                std::cout << "2. Descending\n" ;
                std::cout <<"=============================\n";
                std::cin >> order;

                // Sorting Algorithm Menu
                int algorithm;
                std::cout <<"=============================\n";
                std::cout << "Choose sorting algorithm:\n";
                std::cout << "1. Heap Sort\n";
                std::cout << "2. Merge Sort\n";
                std::cout <<"=============================\n";
                std::cin >> algorithm;

                // assigning name to the user-selected algorithm to display the name in the performance cout
                std::string algo_name;
                if (algorithm == 1) {
                    algo_name = "Heap Sort";
                } else if (algorithm == 2) {
                    algo_name = "Merge Sort";
                } else {
                    std::cout << "Invalid algorithm choice.\n";
                    continue;
                }
                
                // measure the performance of the user-selected algorithm
                double time_measured = measureExecutionTime(records, attribute, order, algorithm);                
                
                // Display Records Menu
                int displayChoice;
                std::cout << "Choose how many records to display:\n";
                std::cout << "1. First 10 Loan Records\n";
                std::cout << "2. Entire dataset of Loan Records\n";
                std::cin >> displayChoice;
                size_t displayCount;
                if (displayChoice == 1) {
                    displayCount = 10;
                }
                if (displayChoice == 2) {
                    displayCount = records.size();
                }
                std::cout << "Displaying " << displayCount << " records:\n";
                for (size_t i = 0; i < displayCount; ++i) {
                    std::cout << "Loan ID: " << records[i].loanID;
                    std::cout << ", " << attribute << ": " << chosenAttribute(records[i], attribute) << "\n";
                }
                
                // Displaying the performance of the user-selected algorithm
                std::cout << "\n" << algo_name <<" completed in " << time_measured << " seconds.\n";

                int sub_menu_choice;
                std::cout << "\n--- Sub-Menu ---\n";
                std::cout << "1. Repeat Sorting\n";
                std::cout << "2. Return to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> sub_menu_choice;
                if (sub_menu_choice == 1) {
                    std::cout << "Reapting Sorting menu...\n";
                    continue;
                }else if (sub_menu_choice == 2) {
                    std::cout << "Returing to Main Menu...\n";
                    break;
                } else {
                    std::cout << "Invalid choice!";
                }

            }
        }
        else if (menuChoice == 2) {
            // Compare Sorting Algorithm Menu
            while (true) {
                std::string attribute;
                int order;
                std::cout <<"\n=========================================\n";
                std::cout << "Menu Option 2: Compare Sorting Algorithms\n";
                std::cout << "-----------------------------------------\n";
                std::cout << "Choose the column to sort:\n";
                std::cout <<"=========================================\n";
                std::cout << "1. Age\n";
                std::cout << "2. Income\n";
                std::cout << "3. Loan Amount\n";
                std::cout << "4. Credit Score\n";
                std::cout << "5. Months Employed\n";
                std::cout << "6. Number of Credit Lines\n";
                std::cout << "7. Interest Rate\n";
                std::cout << "8. Loan Term\n";
                std::cout << "9. DTI Ratio\n";
                std::cout << "10. Defaults (Original)\n"; // implemented for testing purposes, this is the original default attribute in the dataset.
                std::cout << "11. Defaults (Calculated)\n";
                std::cout <<"=============================\n";
                std::cout << "Enter your choice: ";
                int attributeChoice;
                std::cin >> attributeChoice;

                if (attributeChoice == 1) {
                    attribute = "age";
                } else if (attributeChoice == 2) {
                    attribute = "income";
                } else if (attributeChoice == 3) {
                    attribute = "loanAmount";
                } else if (attributeChoice == 4) {
                    attribute = "creditScore";
                } else if (attributeChoice == 5) {
                    attribute = "monthsEmployed";
                }else if (attributeChoice == 6) {
                    attribute = "numCreditLines";
                }else if (attributeChoice == 7) {
                    attribute = "interestRate";
                }else if (attributeChoice == 8) {
                    attribute = "loanTerm";
                }else if (attributeChoice == 9) {
                    attribute = "dtiRatio";
                } else if (attributeChoice == 10) {
                    attribute = "defaults";
                } else if (attributeChoice == 11) {
                    attribute = "calculatedDefaults";
                } else {
                    std::cout << "Invalid attribute choice.\n";
                    continue;
                }
                // Sorting Order Menu
                std::cout <<"\n=============================\n";
                std::cout << "Choose the sorting order:\n";
                std::cout << "1. Ascending\n";
                std::cout << "2. Descending\n" ;
                std::cout <<"=============================\n";
                std::cin >> order;

                // Implement the sorting algorithm performance comparisons here

                //Compare sorting algorithms
                compareSortingAlgorithms(records, attribute, order);

                int displayChoice;
                std::cout << "Choose how many records to display:\n";
                std::cout << "1. Top 10 records\n";
                std::cout << "2. Entire Loan Records Dataset\n";
                std::cin >> displayChoice;

                size_t displayCount;
                if (displayChoice == 1) {
                    displayCount = 10;
                }
                if (displayChoice == 2) {
                    displayCount = records.size();
                }
                std::cout << "Displaying " << displayCount << " records:\n";
                for (size_t i = 0; i < displayCount; ++i) {
                    std::cout << "Loan ID: " << records[i].loanID;
                    std::cout << ", " << attribute << ": " << chosenAttribute(records[i], attribute) << "\n";
                }

                int sub_menu_choice;
                std::cout << "\n--- Sub-Menu ---\n";
                std::cout << "1. Repeat Sorting\n";
                std::cout << "2. Return to Main Menu\n";
                std::cout << "Enter your choice: ";
                std::cin >> sub_menu_choice;
                if (sub_menu_choice == 1) {
                    std::cout << "Reapting Sorting menu...\n";
                    continue;
                }else if (sub_menu_choice == 2) {
                    std::cout << "Returing to Main Menu...\n";
                    break;
                } else {
                    std::cout << "Invalid choice!";
                }
            }
        }
        else if (menuChoice == 3) {
            std::cout << "Existing Program...\n";
            break;
        } else {
            std::cout << "Invalid Menu Choice. Please Try Again! \n";
        }
    }
    return 0;
}
