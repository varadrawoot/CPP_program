#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int rollNo;
    float marks[5];
    float percentage;
    char grade;

public:
    // Function to accept student data
    void acceptData() {
        cout << "\nEnter Student Name       : ";
        cin.ignore();                  // clear buffer
        getline(cin, name);

        cout << "Enter Roll Number        : ";
        cin >> rollNo;

        cout << "Enter marks of 5 subjects (out of 100):\n";
        for (int i = 0; i < 5; i++) {
            cout << "  Subject " << (i + 1) << " : ";
            cin >> marks[i];

            // simple validation
            while (marks[i] < 0 || marks[i] > 100) {
                cout << "  Invalid marks! Enter again (0-100): ";
                cin >> marks[i];
            }
        }
        calculatePercentage();
        assignGrade();
    }

    // Calculate percentage
    void calculatePercentage() {
        float total = 0;
        for (int i = 0; i < 5; i++) {
            total += marks[i];
        }
        percentage = total / 5.0;
    }

    // Assign grade based on percentage
    void assignGrade() {
        if (percentage >= 90)
            grade = 'A';
        else if (percentage >= 80)
            grade = 'B';
        else if (percentage >= 70)
            grade = 'C';
        else if (percentage >= 60)
            grade = 'D';
        else if (percentage >= 40)
            grade = 'E';
        else
            grade = 'F';
    }

    // Display student details
    void display() {
        cout << left << setw(20) << name
             << setw(10) << rollNo
             << setw(10) << fixed << setprecision(2) << percentage
             << setw(8) << grade << endl;
    }

    // Write student record into file
    void writeToFile(ofstream &fout) {
        fout << left << setw(20) << name
             << setw(10) << rollNo
             << setw(10) << fixed << setprecision(2) << percentage
             << setw(8) << grade << endl;
    }
};

int main() {
    const int SIZE = 4;          // 4 students
    Student s[SIZE];

    cout << "=============================================\n";
    cout << "       STUDENT GRADING SYSTEM (OOP)\n";
    cout << "=============================================\n";

    // Accept data for 4 students
    for (int i = 0; i < SIZE; i++) {
        cout << "\n----- Enter details of Student " << (i + 1) << " -----";
        s[i].acceptData();
    }

    // Display all records on screen
    cout << "\n\n==================== RESULT ====================\n";
    cout << left << setw(20) << "Name"
         << setw(10) << "Roll No"
         << setw(10) << "Percentage"
         << setw(8) << "Grade" << endl;
    cout << "------------------------------------------------\n";

    for (int i = 0; i < SIZE; i++) {
        s[i].display();
    }

    // Write records into a file
    ofstream fout("StudentRecords.txt");
    if (!fout) {
        cout << "\nError: Unable to create file!\n";
        return 1;
    }

    fout << "==================== STUDENT RECORDS ====================\n";
    fout << left << setw(20) << "Name"
         << setw(10) << "Roll No"
         << setw(10) << "Percentage"
         << setw(8) << "Grade" << endl;
    fout << "--------------------------------------------------------\n";

    for (int i = 0; i < SIZE; i++) {
        s[i].writeToFile(fout);
    }

    fout.close();
    cout << "\n\nRecords successfully written to file: StudentRecords.txt\n";
    cout << "=====================================================\n";

    return 0;
}