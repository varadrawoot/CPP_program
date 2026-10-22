/*
    Student Grading System (OOP Microproject in C++)
    Concepts: Inheritance, Encapsulation & File I/O
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Base Class: Encapsulation
class Student {
protected:
    int rollNo;
    string name;
public:
    void getDetails(int i) {
        cout << "\n--- Student " << i << " Details ---\n";
        cout << "Enter Roll Number: ";
        cin >> rollNo;
        cin.ignore();
        cout << "Enter Student Name: ";
        getline(cin, name);
    }
};

// Derived Class: Single Inheritance
class Result : public Student {
private:
    float marks[5], total, percentage;
    char grade;

    void calculate() {
        total = 0;
        for (int i = 0; i < 5; i++) total += marks[i];
        percentage = total / 5.0f;
        if (percentage >= 90) grade = 'A';
        else if (percentage >= 75) grade = 'B';
        else if (percentage >= 60) grade = 'C';
        else if (percentage >= 40) grade = 'D';
        else grade = 'F';
    }

public:
    void getData(int i) {
        getDetails(i);
        cout << "Enter Marks for 5 Subjects (out of 100):\n";
        for (int j = 0; j < 5; j++) {
            cout << "  Subject " << (j + 1) << ": ";
            cin >> marks[j];
        }
        calculate();
    }

    void display() const {
        cout << "\nRoll No: " << rollNo << " | Name: " << name
             << " | Total: " << total << "/500 | Percentage: " << percentage
             << "% | Grade: " << grade;
    }

    void writeToFile(ofstream &outFile) const {
        outFile << rollNo << "\t" << name << "\t" << total << "\t"
                << percentage << "%\t" << grade << endl;
    }
};

int main() {
    const int N = 4; // 4 Students
    Result s[N];

    cout << "===== STUDENT GRADING SYSTEM =====\n";
    for (int i = 0; i < N; i++) s[i].getData(i + 1);

    cout << "\n\n========== STUDENT RESULTS ==========";
    for (int i = 0; i < N; i++) s[i].display();
    cout << "\n=====================================\n";

    // Write records into file
    ofstream file("student_records.txt");
    if (!file) {
        cout << "Error opening file!\n";
        return 1;
    }
    file << "RollNo\tName\tTotal\tPercentage\tGrade\n";
    file << "--------------------------------------------------\n";
    for (int i = 0; i < N; i++) s[i].writeToFile(file);
    file.close();

    cout << "\n[✓] Records saved to 'student_records.txt' successfully!\n";
    return 0;
}
