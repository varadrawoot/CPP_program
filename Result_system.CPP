/*
            Student Result system
            This will show the result of two students and will also show the total marks and and percentage of students.

            Key components (What we will use in this program):
            1. Single Inheritance.
            2. Constructor and Destructor.
            3. Scope Resolution Operator :: (Data hiding).
            4. Function overriding.
*/
            #include <iostream>
            #include <string>
            using namespace std;


            // ---- Base class ----
            class student
            {
                protected: // Access is restricted but can be inheritable
                int rollno;
                string name;

                public:
                student(int r, string n) // parameterized constructor
                {
                    rollno = r;
                    name = n;
                    cout << "\n Student constructor called for " << name <<endl;
                }
                    // Now we will display base class
                    void display()
                    {
                        cout<<"\n ---- Student Details ----\n";
                        cout<<"Rollno is:" <<rollno<<endl;
                        cout<<"Name is:"<<name<<endl;
                    }

                    //Now will create a destructor for base class so it will be called when the object of derived class is destroyed
                    ~student()
                    {
                        cout<<"\n Student destructor called for " << name <<endl;
                    }

            };
            // ---- Derived class ----
            class result : public student
            {
                private:
                int marks[3]; // Array to store marks of 3 subjects
                int total;
                float percentage;
                char grade;

                char calculateGrade()
                {
                    if (percentage >= 90) return 'A';
                    if (percentage >= 80) return 'B';
                    if (percentage >= 70) return 'C';
                    if (percentage >= 60) return 'F';
                    return 'F';
                }

                public:
                result(int r, string n, int m[3]) : student(r, n) // Parameterized constructor of derived class
                {
                    total =0;
                    for(int i = 0; i<3; i++)
                    {
                        marks[i] = m[i];
                        total += marks[i];
                    }
                    percentage = total/3.0;
                    grade = calculateGrade();
                    cout << "\n Result constructor called for :" << name <<endl;
                }

                // Overriding the display function of base class
                void display()
                {
                    student::display();
                    cout<<"\n ---- Result Details ----\n"<<endl;
                    cout<< " Marks of 3 subject are"<<endl;
                    for(int i = 0; i<3; i++)
                    {
                        cout<<"Subject "<<i+1<<": "<<marks[i]<<endl;
                    }
                    cout<<"Total Marks: "<<total<<"/300"<<endl;
                    cout<<"Percentage: "<<percentage<<"%"<<endl;
                    cout<<"Grade: "<<grade<<endl;
                }

                // Destructor of derived class
                ~result()
                {
                    cout<<"\n Result destructor called for " << name <<endl;
                }
            };
            // -------- Main function --------
            int main()
            {
                int marks1[3] = {90, 85, 99};
                result R1(1, "Varad Rawoot" , marks1);
                R1.display();

                 cout << "\n----------------------------\n";
 
                 int marks2[3] = {90, 75, 80};
                 result  s2(102, "Krishna Gurav", marks2);
                 s2.display();
 
                return 0;
            }