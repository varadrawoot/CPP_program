/*
    ===========================================================================
    Program Name : Airways Database Management System (Beginner Friendly)
    File Name    : Airways_Database.CPP
    Database File: Airways_Database.db

    OOP Concepts Used:
    1. Constructor       
    2. Encapsulation     
    3. Inheritance      
    4. Overriding       
    5. MySQL Queries of CRUD operation   
    ===========================================================================
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ==========================================
// 1. BASE CLASS (Encapsulation & Constructor)
// ==========================================
class Flight
{
protected:
    int ticketNo;
    string passengerName;
    string source;
    string destination;
    double fare;

public:
    // Constructor to initialize values
    Flight(int t, string name, string src, string dest, double f)
    {
        ticketNo = t;
        passengerName = name;
        source = src;
        destination = dest;
        fare = f;
    }

    // Encapsulation: Getters
    int getTicketNo() const { return ticketNo; }
    string getName() const { return passengerName; }
    string getSource() const { return source; }
    string getDestination() const { return destination; }
    double getFare() const { return fare; }

    // Virtual function for Overriding
    virtual void display() const
    {
        cout << "\n----------------------------------------";
        cout << "\n Ticket No      : " << ticketNo;
        cout << "\n Passenger Name : " << passengerName;
        cout << "\n Route          : " << source << " -> " << destination;
        cout << "\n Total Fare     : Rs. " << fare;
        cout << "\n----------------------------------------\n";
    }
};

// ==========================================
// 2. DERIVED CLASS (Inheritance & Overriding)
// ==========================================
class InternationalFlight : public Flight
{
private:
    string passportNo;

public:
    // Constructor calling Base Constructor
    InternationalFlight(int t, string name, string src, string dest, double f, string pass)
        : Flight(t, name, src, dest, f)
    {
        passportNo = pass;
    }

    // Function Overriding: Custom display for International Flights
    void display() const override
    {
        Flight::display(); // Call base class display
        cout << " Flight Type    : International";
        cout << "\n Passport No    : " << passportNo;
        cout << "\n----------------------------------------\n";
    }
};

// ==========================================
// 3. CRUD FUNCTIONS (MySQL Query Simulations)
// ==========================================

// 1. INSERT (Create)
void bookFlight()
{
    int tNo;
    string name, src, dest;
    double fare;

    cout << "\n--- Book Flight (INSERT) ---\n";
    cout << "Enter Ticket No: ";
    cin >> tNo;
    cout << "Enter Passenger Name: ";
    cin >> name;
    cout << "Enter Source City: ";
    cin >> src;
    cout << "Enter Destination City: ";
    cin >> dest;
    cout << "Enter Fare (Rs): ";
    cin >> fare;

    // OOP Constructor Call
    Flight f(tNo, name, src, dest, fare);

    // MySQL Command
    cout << "\n[MySQL Query]: INSERT INTO flights VALUES (" 
         << tNo << ", '" << name << "', '" << src << "', '" << dest << "', " << fare << ");\n";

    // Save to Database File (Airways_Database.db)
    ofstream file("Airways_Database.db", ios::app);
    file << tNo << " " << name << " " << src << " " << dest << " " << fare << endl;
    file.close();

    cout << " Ticket booked and saved into 'Airways_Database.db' successfully!\n";
    f.display();
}

// 2. SELECT * (Read All)
void displayAllFlights()
{
    cout << "\n[MySQL Query]: SELECT * FROM flights;\n\n";

    ifstream file("Airways_Database.db");
    if (!file.is_open())
    {
        cout << " Database file not found!\n";
        return;
    }

    int t;
    string name, src, dest;
    double fare;
    int count = 0;

    cout << "==============================================================\n";
    cout << "TICKET NO\tNAME\t\tSOURCE\t\tDESTINATION\tFARE\n";
    cout << "==============================================================\n";

    while (file >> t >> name >> src >> dest >> fare)
    {
        cout << t << "\t\t" << name << "\t" << src << "\t\t" << dest << "\t\tRs." << fare << endl;
        count++;
    }
    file.close();

    cout << "==============================================================\n";
    cout << "Total: " << count << " record(s) found.\n";
}

// 3. SELECT WHERE (Search)
void searchFlight()
{
    int searchTicket;
    cout << "\nEnter Ticket No to Search: ";
    cin >> searchTicket;

    cout << "\n[MySQL Query]: SELECT * FROM flights WHERE ticket_no = " << searchTicket << ";\n";

    ifstream file("Airways_Database.db");
    int t;
    string name, src, dest;
    double fare;
    bool found = false;

    while (file >> t >> name >> src >> dest >> fare)
    {
        if (t == searchTicket)
        {
            Flight f(t, name, src, dest, fare);
            f.display();
            found = true;
            break;
        }
    }
    file.close();

    if (!found)
    {
        cout << " [MySQL] 0 rows in set. Ticket No " << searchTicket << " not found!\n";
    }
}

// 4. UPDATE
void updateFlight()
{
    int updateTicket;
    cout << "\nEnter Ticket No to Update: ";
    cin >> updateTicket;

    ifstream file("Airways_Database.db");
    ofstream temp("temp.db");
    int t;
    string name, src, dest;
    double fare;
    bool found = false;

    while (file >> t >> name >> src >> dest >> fare)
    {
        if (t == updateTicket)
        {
            found = true;
            string newDest;
            double newFare;

            cout << "Enter New Destination: ";
            cin >> newDest;
            cout << "Enter New Fare (Rs): ";
            cin >> newFare;

            // MySQL Command
            cout << "\n[MySQL Query]: UPDATE flights SET destination = '" << newDest 
                 << "', fare = " << newFare << " WHERE ticket_no = " << updateTicket << ";\n";

            temp << t << " " << name << " " << src << " " << newDest << " " << newFare << endl;
        }
        else
        {
            temp << t << " " << name << " " << src << " " << dest << " " << fare << endl;
        }
    }

    file.close();
    temp.close();

    remove("Airways_Database.db");
    rename("temp.db", "Airways_Database.db");

    if (found)
        cout << " Ticket updated successfully in 'Airways_Database.db'!\n";
    else
        cout << " Ticket No " << updateTicket << " not found!\n";
}

// 5. DELETE
void deleteFlight()
{
    int deleteTicket;
    cout << "\nEnter Ticket No to Cancel: ";
    cin >> deleteTicket;

    ifstream file("Airways_Database.db");
    ofstream temp("temp.db");
    int t;
    string name, src, dest;
    double fare;
    bool found = false;

    while (file >> t >> name >> src >> dest >> fare)
    {
        if (t == deleteTicket)
        {
            found = true;
            // MySQL Command
            cout << "\n[MySQL Query]: DELETE FROM flights WHERE ticket_no = " << deleteTicket << ";\n";
        }
        else
        {
            temp << t << " " << name << " " << src << " " << dest << " " << fare << endl;
        }
    }

    file.close();
    temp.close();

    remove("Airways_Database.db");
    rename("temp.db", "Airways_Database.db");

    if (found)
        cout << " Ticket cancelled and removed from 'Airways_Database.db' successfully!\n";
    else
        cout << " Ticket No " << deleteTicket << " not found!\n";
}

// ==========================================
// 4. MAIN MENU
// ==========================================
int main()
{
    int choice = 0;

    do
    {
        cout << "\n========================================";
        cout << "\n   AIRWAYS DATABASE MANAGEMENT SYSTEM   ";
        cout << "\n========================================";
        cout << "\n 1. Book Flight Ticket       (INSERT)";
        cout << "\n 2. Display All Flights      (SELECT *)";
        cout << "\n 3. Search Flight by Ticket  (SELECT WHERE)";
        cout << "\n 4. Update Flight Ticket     (UPDATE)";
        cout << "\n 5. Cancel Flight Ticket     (DELETE)";
        cout << "\n 6. Exit";
        cout << "\n========================================";
        cout << "\n Enter your choice (1-6): ";
        cin >> choice;

        switch (choice)
        {
            case 1: bookFlight(); break;
            case 2: displayAllFlights(); break;
            case 3: searchFlight(); break;
            case 4: updateFlight(); break;
            case 5: deleteFlight(); break;
            case 6: cout << "\n Thank you for using Airways Database System!\n"; break;
            default: cout << "\n Invalid Choice! Please choose 1 to 6.\n";
        }

    } while (choice != 6);

    return 0;
}
