#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//Function to calculate average
float calculateAverage(const vector<float>& marks) {
    float total = 0;
    for (float mark : marks) {
        total += mark;
    }
    return total / marks.size();
}

//Determine grade based on average
char calculateGrade(float average) {
    if (average >= 90) return 'A';
    else if (average >= 80) return 'B';
    else if (average >= 70) return 'C';
    else if (average >= 60) return 'D';
    else return 'F';
}

//Function to Save Data to File
void saveToFile(const string& name, const vector<float>& marks, float average, char grade) {
    ofstream outFile("grades.txt", ios::app); // Append mode
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        return;
    }
    outFile << "Student: " << name << endl;
    outFile << "Marks: ";
    for (float m : marks) outFile << m << " ";
    outFile << "\nAverage: " << average << endl;
    outFile << "Grade: " << grade << "\n\n";
    outFile.close();
}

//Main Program
int main() {
    char anotherStudent = 'y';

    while (tolower(anotherStudent) == 'y') {
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

        // Input marks with validation
        for (int i = 0; i < numSubjects; i++) {
            float mark;
            do {
                cout << "Enter mark for subject " << i+1 << " (0-100): ";
                cin >> mark;
                if (mark < 0 || mark > 100) {
                    cout << "Invalid mark! Try again." << endl;
                }
            } while (mark < 0 || mark > 100);
            marks.push_back(mark);
        }

        // Calculate average and grade
        float average = calculateAverage(marks);
        char grade = calculateGrade(average);

        // Display results
        cout << "\n------ Results ------" << endl;
        cout << "Student: " << name << endl;
        cout << "Average: " << average << endl;
        cout << "Grade: " << grade << endl;

        // Save to file
        saveToFile(name, marks, average, grade);

        // Ask for another student
        cout << "\nAdd another student? (y/n): ";
        cin >> anotherStudent;
    }

    cout << "Data saved to grades.txt. Exiting..." << endl;
    return 0;
}