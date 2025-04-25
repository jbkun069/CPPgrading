#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <map>
#include <sstream>
#include <tuple>

using namespace std;

// Function to calculate average
float calculateAverage(const vector<float>& marks) {
    float total = 0;
    for (float mark : marks) {
        total += mark;
    }
    return total / marks.size();
}

// Determine grade based on average
char calculateGrade(float average) {
    const float A_THRESHOLD = 90.0f;
    const float B_THRESHOLD = 80.0f;
    const float C_THRESHOLD = 70.0f;
    const float D_THRESHOLD = 60.0f;

    if (average >= A_THRESHOLD) return 'A';
    else if (average >= B_THRESHOLD) return 'B';
    else if (average >= C_THRESHOLD) return 'C';
    else if (average >= D_THRESHOLD) return 'D';
    else return 'F';
}

// Function to Save Data to File
const string GRADES_FILE = "grades.txt";

void saveToFile(const string& name, const vector<float>& marks, float average, char grade) {
    ofstream outFile(GRADES_FILE, ios::app); // Append mode
    if (!outFile) {
        cerr << "Error opening file " << GRADES_FILE << "!" << endl;
        return;
    }
    if (marks.empty()) {
        cerr << "No marks to save!" << endl;
        return;
    }
    outFile << "Student: " << name << endl;
    outFile << "Marks: ";
    for (float m : marks) outFile << m << " ";
    outFile << "\nAverage: " << average << endl;
    outFile << "Grade: " << grade << "\n\n";
    outFile.close();
}

// Function to View All Records
void viewAllRecords() {
    ifstream inFile("grades.txt");
    if (!inFile) {
        cerr << "Error opening file!" << endl;
        return;
    }
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}

// Function to Calculate Summary Statistics
void calculateStatistics() {
    ifstream inFile("grades.txt");
    if (!inFile) {
        cerr << "Error opening file!" << endl;
        return;
    }

    vector<float> averages;
    map<char, int> gradeCount;
    float highestMark = -1, lowestMark = 101;
    float highestAverage = -1, lowestAverage = 101;

    string line;
    while (getline(inFile, line)) {
        if (line.find("Average: ") == 0) {
            float average = stof(line.substr(9));
            averages.push_back(average);
            if (average > highestAverage) highestAverage = average;
            if (average < lowestAverage) lowestAverage = average;
        } else if (line.find("Grade: ") == 0) {
            char grade = line[7];
            gradeCount[grade]++;
        } else if (line.find("Marks: ") == 0) {
            istringstream iss(line.substr(7));
            float mark;
            while (iss >> mark) {
                if (mark > highestMark) highestMark = mark;
                if (mark < lowestMark) lowestMark = mark;
            }
        }
    }
    inFile.close();

    // Calculate class average
    float classAverage = 0;
    if (!averages.empty()) {
        for (float avg : averages) {
            classAverage += avg;
        }
        classAverage /= averages.size();
    }

    // Display statistics
    cout << "\n----- Summary Statistics -----" << endl;
    cout << "Class Average: " << classAverage << endl;
    cout << "Highest Mark: " << highestMark << endl;
    cout << "Lowest Mark: " << lowestMark << endl;
    cout << "Highest Average: " << highestAverage << endl;
    cout << "Lowest Average: " << lowestAverage << endl;
    cout << "Grade Distribution:" << endl;
    for (const auto& pair : gradeCount) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

// Function to Load All Records into a Vector
vector<tuple<string, vector<float>, float, char>> loadRecords() {
    ifstream inFile("grades.txt");
    if (!inFile) {
        cerr << "Error opening file!" << endl;
        return {};
    }

    vector<tuple<string, vector<float>, float, char>> records;
    string line, name;
    vector<float> marks;
    float average;
    char grade;

    while (getline(inFile, line)) {
        if (line.find("Student: ") == 0) {
            if (!name.empty()) {
                records.emplace_back(name, marks, average, grade);
            }
            name = line.substr(9);
        } else if (line.find("Marks: ") == 0) {
            istringstream iss(line.substr(7));
            marks.clear();
            float mark;
            while (iss >> mark) {
                marks.push_back(mark);
            }
            average = calculateAverage(marks);
            grade = calculateGrade(average);
        }
    }
    if (!name.empty()) {
        records.emplace_back(name, marks, average, grade);
    }

    inFile.close();
    return records;
}

// Function to Sort and Display Records by Name
// Helper function to display a single student record
void displayStudentRecord(const tuple<string, vector<float>, float, char>& record) {
    cout << "Student: " << get<0>(record) << endl;
    cout << "Marks: ";
    for (float mark : get<1>(record)) {
        cout << mark << " ";
    }
    cout << "\nAverage: " << get<2>(record) << endl;
    cout << "Grade: " << get<3>(record) << "\n\n";
}

// Function to Sort and Display Records by Name
void sortAndDisplayByName() {
    auto records = loadRecords();
    sort(records.begin(), records.end(), [](const auto& a, const auto& b) {
        return get<0>(a) < get<0>(b); // Sort by name (first element of tuple)
    });

    cout << "\n----- Students Sorted by Name -----" << endl;
    for (const auto& record : records) {
        displayStudentRecord(record);
    }
}

// Function to Sort and Display Records by Average
void sortAndDisplayByAverage() {
    auto records = loadRecords();
    sort(records.begin(), records.end(), [](const auto& a, const auto& b) {
        return get<2>(a) > get<2>(b); // Sort by average in descending order
    });

    cout << "\n----- Students Sorted by Average -----" << endl;
    for (const auto& record : records) {
        displayStudentRecord(record);
    }
}

// Function to Filter and Display Records by Grade
void filterAndDisplayByGrade(char grade) {
    auto records = loadRecords();

    cout << "\n----- Students with Grade " << grade << " -----" << endl;
    for (const auto& record : records) {
        if (get<3>(record) == grade) {
            displayStudentRecord(record);
        }
    }
}

// Function to Filter and Display Records by Performance
void filterAndDisplayByPerformance(float threshold, bool above) {
    auto records = loadRecords();
    string comparison = above ? "Above" : "Below";

    cout << "\n----- Students " << comparison << " Average " << threshold << " -----" << endl;
    for (const auto& record : records) {
        if ((above && get<2>(record) > threshold) || (!above && get<2>(record) < threshold)) {
            displayStudentRecord(record);
        }
    }
}

// Function to Delete Student Record by Name
bool deleteRecord(const string& name) {
    // Load all records
    auto records = loadRecords();
    bool found = false;
    
    // Find and remove the record with the matching name
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
    
    // Rewrite the file with the remaining records
    ofstream outFile(GRADES_FILE, ios::trunc); // Truncate the file
    if (!outFile) {
        cerr << "Error opening file for writing!" << endl;
        return false;
    }
    
    for (const auto& record : records) {
        const string& studentName = get<0>(record);
        const vector<float>& marks = get<1>(record);
        float average = get<2>(record);
        char grade = get<3>(record);
        
        outFile << "Student: " << studentName << endl;
        outFile << "Marks: ";
        for (float m : marks) outFile << m << " ";
        outFile << "\nAverage: " << average << endl;
        outFile << "Grade: " << grade << "\n\n";
    }
    
    outFile.close();
    return true;
}

// Main Program
int main() {
    int choice;
    do {
        cout << "\nStudent Grade Management System" << endl;
        cout << "1. Add Student" << endl;
        cout << "2. View All Records" << endl;
        cout << "3. Search Student" << endl;
        cout << "4. Delete Student" << endl;  // New option
        cout << "5. Calculate Statistics" << endl;  // Shifted from 4
        cout << "6. Sort by Name" << endl;  // Shifted from 5
        cout << "7. Sort by Average" << endl;  // Shifted from 6
        cout << "8. Filter by Grade" << endl;  // Shifted from 7
        cout << "9. Filter by Performance" << endl;  // Shifted from 8
        cout << "10. Exit" << endl;  // Shifted from 9
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int numSubjects;
            vector<float> marks;

            // Input student name
            cout << "Enter student name: ";
            cin.ignore(); // Clear input buffer
            getline(cin, name);

            // Input number of subjects
            cout << "Enter number of subjects: ";
            cin >> numSubjects;
            cin.ignore(); // Clear input buffer

            // Input marks with validation
            for (int i = 0; i < numSubjects; i++) {
                float mark;
                do {
                    cout << "Enter mark for subject " << i + 1 << " (0-100): ";
                    cin >> mark;
                    if (cin.fail() || mark < 0 || mark > 100) {
                        cout << "Invalid mark! Try again:" << endl;
                        cin.clear(); // Clear the error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    }
                } while (cin.fail() || mark < 0 || mark > 100);
                marks.push_back(mark);
            }

            // Calculate average and grade
            float average = calculateAverage(marks);
            char grade = calculateGrade(average);

            // Display results
            cout << "\n----- Results -----" << endl;
            cout << "Student: " << name << endl;
            cout << "Average: " << average << endl;
            cout << "Grade: " << grade << endl;

            // Save to file
            saveToFile(name, marks, average, grade);

        } else if (choice == 2) {
            viewAllRecords();
        } else if (choice == 3) {
            string searchName;
            cout << "Enter student name to search: ";
            cin.ignore(); // Clear input buffer
            getline(cin, searchName);
            // Implement search logic here...
        } else if (choice == 4) {
            string deleteName;
            cout << "Enter student name to delete: ";
            cin.ignore(); // Clear input buffer
            getline(cin, deleteName);
            
            if (deleteRecord(deleteName)) {
                cout << "Student record deleted successfully!" << endl;
            } else {
                cout << "Student not found or error deleting record." << endl;
            }
        } else if (choice == 5) {
            calculateStatistics();
        } else if (choice == 6) {
            sortAndDisplayByName();
        } else if (choice == 7) {
            sortAndDisplayByAverage();
        } else if (choice == 8) {
            char grade;
            cout << "Enter grade to filter by (A/B/C/D/F): ";
            cin >> grade;
            filterAndDisplayByGrade(grade);
        } else if (choice == 9) {
            float threshold;
            bool above;
            cout << "Enter average threshold: ";
            cin >> threshold;
            cout << "Filter students (1) Above or (2) Below the threshold? ";
            int option;
            cin >> option;
            above = (option == 1);
            filterAndDisplayByPerformance(threshold, above);
        } else if (choice != 10) {
            cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 10);

    cout << "Exiting..." << endl;
    return 0;
}



