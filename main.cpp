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
    int calcDefaults;

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
    if (attribute == "calculatedDefaults") return record.calcDefaults;}

void RiskScore(std::vector<loanRecord>& records) {
    // placeholder for implementation

}

void DefaultStatus(std::vector<loanRecord>& records, float defaultThreshold) {
    // placeholder for implementation
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

std::vector<loanRecord> merge(const std::vector<loanRecord>& first, const std::vector<loanRecord>& second, const std::string& attribute, int order) {
    // placeholder for implementation
}

std::vector<loanRecord> mergeSort(const std::vector<loanRecord>& records, const std::string& attribute, int order) {
    // placeholder for implementation
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
                    attribute = "dtiRatio";
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
                int algoChoice;
                std::cout <<"=============================\n";
                std::cout << "Choose sorting algorithm:\n";
                std::cout << "1. Heap Sort\n";
                std::cout << "2. Merge Sort\n";
                std::cout <<"=============================\n";
                std::cin >> algoChoice;

                // Implement sorting algorithm performance here
                std::string algo_name;
                if (algoChoice == 1) {
                    algo_name = "Heap Sort";
                    heapSort(records, attribute, order);
                } else if (algoChoice == 2) {
                    algo_name = "Merge Sort";
                    records = mergeSort(records, attribute, order);
                } else {
                    std::cout << "Invalid algorithm choice.\n";
                    continue;
                }
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
                    std::cout << "Invald choice!";
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
                    attribute = "dtiRatio";
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
                std::cout << "\n Heap Sorted completed in " << "placeholder for heap time variable"  << " seconds\n";
                std::cout << "\n Merge Sorted completed in " << "placeholder for merge time variable"  << " seconds\n";

                // implement performance comparsion if statements and comments here

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
                    std::cout << "Invald choice!";
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