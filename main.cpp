#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <map>
#include <sstream>
#include <tuple>
#include <filesystem>
#include <cctype>

using namespace std;

// Configuration struct to hold settings
struct Config {
    static const string GRADES_FILE;
    static const double MAX_MARK;
    static const double MIN_MARK;
    static const double INVALID_AVERAGE;
    static const double A_THRESHOLD;
    static const double B_THRESHOLD;
    static const double C_THRESHOLD;
    static const double D_THRESHOLD;
};

const string Config::GRADES_FILE = "grades.txt";
const double Config::MAX_MARK = 100.0;
const double Config::MIN_MARK = 0.0;
const double Config::INVALID_AVERAGE = -1.0;
const double Config::A_THRESHOLD = 90.0;
const double Config::B_THRESHOLD = 80.0;
const double Config::C_THRESHOLD = 70.0;
const double Config::D_THRESHOLD = 60.0;

// Function to calculate average of marks
double calculateAverage(const vector<double>& marks) {
    if (marks.empty()) return Config::INVALID_AVERAGE;
    double total = 0;
    for (double mark : marks) {
        total += mark;
    }
    return total / marks.size();
}

// Determine grade based on average
char calculateGrade(double average) {
    if (average >= Config::A_THRESHOLD) return 'A';
    else if (average >= Config::B_THRESHOLD) return 'B';
    else if (average >= Config::C_THRESHOLD) return 'C';
    else if (average >= Config::D_THRESHOLD) return 'D';
    else return 'F';
}

// Check if student name already exists
bool studentExists(const string& name, const string& filename) {
    if (!filesystem::exists(filename)) return false;
    ifstream inFile(filename);
    if (!inFile) throw runtime_error("Failed to open file: " + filename);
    
    string line;
    while (getline(inFile, line)) {
        if (line.find("Student: ") == 0 && line.substr(9) == name) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

// Sanitize student name (remove leading/trailing spaces)
string sanitizeName(const string& name) {
    string sanitized = name;
    sanitized.erase(sanitized.begin(), find_if(sanitized.begin(), sanitized.end(), []( Popper: isspace));
    sanitized.erase(find_if(sanitized.rbegin(), sanitized.rend(), [](char c) { return !isspace(c); }).base(), sanitized.end());
    return sanitized;
}

// Save student data to file
void saveToFile(const string& name, const vector<double>& marks, double average, char grade, const string& filename) {
    if (marks.empty()) {
        cout << "No marks to save!" << endl;
        return;
    }
    ofstream outFile(filename, ios::app);
    if (!outFile) {
        throw runtime_error("Failed to open file for writing: " + filename);
    }
    outFile << "Student: " << name << endl;
    outFile << "Marks: ";
    for (double m : marks) outFile << m << " ";
    outFile << "\nAverage: " << average << endl;
    outFile << "Grade: " << grade << "\n\n";
}

// Read all lines from file
vector<string> readFileLines(const string& filename) {
    vector<string> lines;
    if (!filesystem::exists(filename)) {
        return lines;
    }
    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Failed to open file: " + filename);
    }
    string line;
    while (getline(inFile, line)) {
        lines.push_back(line);
    }
    return lines;
}

// View all records
void viewAllRecords(const string& filename) {
    auto lines = readFileLines(filename);
    if (lines.empty()) {
        cout << "No records found!" << endl;
        return;
    }
    for (const auto& line : lines) {
        cout << line << endl;
    }
}

// Calculate summary statistics
void calculateStatistics(const string& filename) {
    if (!filesystem::exists(filename)) {
        cout << "No records found!" << endl;
        return;
    }
    vector<double> averages;
    map<char, int> gradeCount;
    double highestMark = Config::MIN_MARK;
    double lowestMark = Config::MAX_MARK;
    double highestAverage = Config::INVALID_AVERAGE;
    double lowestAverage = Config::MAX_MARK;

    auto lines = readFileLines(filename);
    for (const auto& line : lines) {
        if (line.find("Average: ") == 0) {
            double average = stod(line.substr(9));
            averages.push_back(average);
            if (average > highestAverage) highestAverage = average;
            if (average < lowestAverage) lowestAverage = average;
        } else if (line.find("Grade: ") == 0) {
            char grade = line[7];
            gradeCount[grade]++;
        } else if (line.find("Marks: ") == 0) {
            istringstream markStream(line.substr(7));
            double mark;
            while (markStream >> mark) {
                if (mark > highestMark) highestMark = mark;
                if (mark < lowestMark) lowestMark = mark;
            }
        }
    }

    double classAverage = 0;
    if (!averages.empty()) {
        for (double avg : averages) {
            classAverage += avg;
        }
        classAverage /= averages.size();
    } else {
        cout << "No averages available!" << endl;
        return;
    }

    cout << "\n----- Summary Statistics -----" << endl;
    cout << "Class Average: " << classAverage << endl;
    cout << "Highest Mark: " << (highestMark == Config::MIN_MARK ? "N/A" : to_string(highestMark)) << endl;
    cout << "Lowest Mark: " << (lowestMark == Config::MAX_MARK ? "N/A" : to_string(lowestMark)) << endl;
    cout << "Highest Average: " << (highestAverage == Config::INVALID_AVERAGE ? "N/A" : to_string(highestAverage)) << endl;
    cout << "Lowest Average: " << (lowestAverage == Config::MAX_MARK ? "N/A" : to_string(lowestAverage)) << endl;
    cout << "Grade Distribution:" << endl;
    for (const auto& pair : gradeCount) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

// Load all records into a vector of tuples
vector<tuple<string, vector<double>, double, char>> loadRecords(const string& filename) {
    vector<tuple<string, vector<double>, double, char>> records;
    if (!filesystem::exists(filename)) {
        return records;
    }

    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Failed to open file: " + filename);
    }

    string line, name;
    vector<double> marks;
    double average;
    char grade;

    while (getline(inFile, line)) {
        if (line.find("Student: ") == 0) {
            if (!name.empty()) {
                records.emplace_back(name, marks, average, grade);
            }
            name = line.substr(9);
        } else if (line.find("Marks: ") == 0) {
            istringstream markStream(line.substr(7));
            marks.clear();
            double mark;
            while (markStream >> mark) {
                marks.push_back(mark);
            }
            average = calculateAverage(marks);
            grade = calculateGrade(average);
        }
    }
    if (!name.empty()) {
        records.emplace_back(name, marks, average, grade);
    }

    return records;
}

// Display a single student record
void displayStudentRecord(const tuple<string, vector<double>, double, char>& record) {
    cout << "Student: " << get<0>(record) << endl;
    cout << "Marks: ";
    for (double mark : get<1>(record)) {
        cout << mark << " ";
    }
    cout << "\nAverage: " << get<2>(record) << endl;
    cout << "Grade: " << get<3>(record) << "\n\n";
}

// Sort and display records by name
void sortAndDisplayByName(const string& filename) {
    auto records = loadRecords(filename);
    if (records.empty()) {
        cout << "\nNo student records found!\n";
        return;
    }
    sort(records.begin(), records.end(), [](const auto& a, const auto& b) {
        return get<0>(a) < get<0>(b);
    });

    cout << "\n----- Students Sorted by Name -----" << endl;
    for (const auto& record : records) {
        displayStudentRecord(record);
    }
}

// Sort and display records by average
void sortAndDisplayByAverage(const string& filename) {
    auto records = loadRecords(filename);
    if (records.empty()) {
        cout << "\nNo student records found!\n";
        return;
    }
    sort(records.begin(), records.end(), [](const auto& a, const auto& b) {
        return get<2>(a) > get<2>(b);
    });

    cout << "\n----- Students Sorted by Average -----" << endl;
    for (const auto& record : records) {
        displayStudentRecord(record);
    }
}

// Filter and display records by grade
void filterAndDisplayByGrade(char grade, const string& filename) {
    grade = toupper(grade);
    if (grade != 'A' && grade != 'B' && grade != 'C' && grade != 'D' && grade != 'F') {
        cout << "Invalid grade! Use A, B, C, D, or F." << endl;
        return;
    }

    auto records = loadRecords(filename);
    cout << "\n----- Students with Grade " << grade << " -----" << endl;
    bool found = false;
    for (const auto& record : records) {
        if (get<3>(record) == grade) {
            displayStudentRecord(record);
            found = true;
        }
    }
    if (!found) {
        cout << "No students found with grade " << grade << endl;
    }
}

// Filter and display records by performance
void filterAndDisplayByPerformance(double threshold, bool above, const string& filename) {
    auto records = loadRecords(filename);
    if (records.empty()) {
        cout << "\nNo student records found!\n";
        return;
    }
    string comparison = above ? "Above" : "Below";
    cout << "\n----- Students " << comparison << " Average " << threshold << " -----" << endl;
    bool found = false;
    for (const auto& record : records) {
        if ((above && get<2>(record) > threshold) || (!above && get<2>(record) < threshold)) {
            displayStudentRecord(record);
            found = true;
        }
    }
    if (!found) {
        cout << "No students found " << comparison << " " << threshold << endl;
    }
}

// Delete student record by name
bool deleteRecord(const string& name, const string& filename) {
    auto records = loadRecords(filename);
    bool found = false;
    
    for (auto it = records.begin(); it != records.end(); ) {
        if (get<0>(*it) == name) {
            it = records.erase(it);
            found = true;
        } else {
            ++it;
        }
    }
    
    if (!found) {
        return false;
    }
    
    ofstream outFile(filename, ios::trunc);
    if (!outFile) {
        throw runtime_error("Failed to open file for writing: " + filename);
    }
    
    for (const auto& record : records) {
        const string& studentName = get<0>(record);
        const vector<double>& marks = get<1>(record);
        double average = get<2>(record);
        char grade = get<3>(record);
        
        outFile << "Student: " << studentName << endl;
        outFile << "Marks: ";
        for (double m : marks) outFile << m << " ";
        outFile << "\nAverage: " << average << endl;
        outFile << "Grade: " << grade << "\n\n";
    }
    
    return true;
}

// Add a new student
void addStudent(const string& filename) {
    string name;
    int numSubjects;
    vector<double> marks;

    cout << "Enter student name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);
    name = sanitizeName(name);
    
    if (name.empty()) {
        cout << "Name cannot be empty!" << endl;
        return;
    }
    
    if (studentExists(name, filename)) {
        cout << "Student already exists!" << endl;
        return;
    }

    cout << "Enter number of subjects: ";
    while (!(cin >> numSubjects) || numSubjects <= 0) {
        cout << "Invalid number! Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    marks.reserve(numSubjects);
    for (int i = 0; i < numSubjects; i++) {
        double mark;
        cout << "Enter mark for subject " << i + 1 << " (0-100): ";
        while (!(cin >> mark) || mark < Config::MIN_MARK || mark > Config::MAX_MARK) {
            cout << "Invalid mark! Enter a number between 0 and 100: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        marks.push_back(mark);
    }

    double average = calculateAverage(marks);
    char grade = calculateGrade(average);

    cout << "\n----- Results -----" << endl;
    cout << "Student: " << name << endl;
    cout << "Average: " << average << endl;
    cout << "Grade: " << grade << endl;

    saveToFile(name, marks, average, grade, filename);
}

// Main menu
void displayMenu() {
    cout << "\nStudent Grade Management System" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. View All Records" << endl;
    cout << "3. Search Student" << endl;
    cout << "4. Delete Student" << endl;
    cout << "5. Calculate Statistics" << endl;
    cout << "6. Sort by Name" << endl;
    cout << "7. Sort by Average" << endl;
    cout << "8. Filter by Grade" << endl;
    cout << "9. Filter by Performance" << endl;
    cout << "10. Exit" << endl;
    cout << "Enter your choice: ";
}

// Main program
int main() {
    try {
        int choice;
        do {
            displayMenu();
            while (!(cin >> choice) || choice < 1 || choice > 10) {
                cout << "Invalid input! Enter a number between 1 and 10: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            if (choice == 1) {
                addStudent(Config::GRADES_FILE);
            } else if (choice == 2) {
                viewAllRecords(Config::GRADES_FILE);
            } else if (choice == 3) {
                string searchName;
                cout << "Enter student name to search: ";
                cin.ignore();
                getline(cin, searchName);
                searchName = sanitizeName(searchName);
                auto records = loadRecords(Config::GRADES_FILE);
                bool found = false;
                for (const auto& record : records) {
                    if (get<0>(record) == searchName) {
                        displayStudentRecord(record);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Student not found!" << endl;
                }
            } else if (choice == 4) {
                string deleteName;
                cout << "Enter student name to delete: ";
                cin.ignore();
                getline(cin, deleteName);
                deleteName = sanitizeName(deleteName);
                if (deleteRecord(deleteName, Config::GRADES_FILE)) {
                    cout << "Student record deleted successfully!" << endl;
                } else {
                    cout << "Student not found or error deleting record." << endl;
                }
            } else if (choice == 5) {
                calculateStatistics(Config::GRADES_FILE);
            } else if (choice == 6) {
                sortAndDisplayByName(Config::GRADES_FILE);
            } else if (choice == 7) {
                sortAndDisplayByAverage(Config::GRADES_FILE);
            } else if (choice == 8) {
                char grade;
                cout << "Enter grade to filter by (A/B/C/D/F): ";
                cin >> grade;
                filterAndDisplayByGrade(grade, Config::GRADES_FILE);
            } else if (choice == 9) {
                double threshold;
                int option;
                cout << "Enter average threshold: ";
                while (!(cin >> threshold) || threshold < Config::MIN_MARK || threshold > Config::MAX_MARK) {
                    cout << "Invalid threshold! Enter a number between 0 and 100: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cout << "Filter students (1) Above or (2) Below the threshold? ";
                while (!(cin >> option) || (option != 1 && option != 2)) {
                    cout << "Enter 1 for Above or 2 for Below: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                filterAndDisplayByPerformance(threshold, option == 1, Config::GRADES_FILE);
            }
        } while (choice != 10);

        cout << "Exiting..." << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}