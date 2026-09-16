/*
    ===========================================================================
    Program Name : Bank Database Management System
    File Name    : Bank_database.CPP
    Database File: Bank_database.db (Saved in Visual Table Format)

    OOP Concepts Used:
    1. Hierarchical Inheritance
    2. Protected Data Members  
    3. Constructors            
    4. Function Overriding     
    5. Encapsulation           
    6. MySQL Commands          
    7. Database File           
    ===========================================================================
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;

// Structure to hold in-memory account data
struct AccountRecord
{
    int accountNo;
    string name;
    double balance;
    string type;
    string extra;
};

// Helper function to trim spaces
string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Helper to read numbers safely (automatically strips '%', commas, etc.)
double readDoubleInput(const string& prompt)
{
    cout << prompt;
    string input;
    cin >> input;
    string clean = "";
    for (size_t i = 0; i < input.length(); i++)
    {
        char c = input[i];
        if (isdigit(c) || c == '.' || c == '-') clean += c;
    }
    if (clean.empty()) return 0.0;
    try { return stod(clean); }
    catch (...) { return 0.0; }
}

int readIntInput(const string& prompt)
{
    cout << prompt;
    string input;
    cin >> input;
    string clean = "";
    for (size_t i = 0; i < input.length(); i++)
    {
        char c = input[i];
        if (isdigit(c)) clean += c;
    }
    if (clean.empty()) return 0;
    try { return stoi(clean); }
    catch (...) { return 0; }
}

// ==================== BASE CLASS ====================
class BankAccount
{
protected:
    int accountNo;          // Protected member accessible in derived classes
    string name;
    double balance;
    string accountType;

public:
    BankAccount() // Default constructor
    {
        accountNo = 0;
        name = "";
        balance = 0.0;
        accountType = "General";
    }

    BankAccount(int acc, string n, double b, string t) // Parameterized constructor
    {
        accountNo = acc;
        name = n;
        balance = b;
        accountType = t;
    }

    virtual ~BankAccount() {} // Destructor

    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance = balance + amount;
            cout << "\n Amount deposited successfully!";
            cout << "\n Current Balance: Rs. " << balance << endl;
        }
        else
        {
            cout << "\n Invalid deposit amount!\n";
        }
    }

    // Virtual function for Overriding
    virtual bool withdraw(double amount)
    {
        if (amount > 0 && balance >= amount)
        {
            balance = balance - amount;
            cout << "\n Amount withdrawn successfully!";
            cout << "\n Current Balance: Rs. " << balance << endl;
            return true;
        }
        else
        {
            cout << "\n Insufficient balance!\n";
            return false;
        }
    }

    // Virtual display function
    virtual void display()
    {
        cout << "\n+------------+----------------+------------+----------+";
        cout << "\n| ACC NO     | HOLDER NAME    | BALANCE(Rs)| TYPE     |";
        cout << "\n+------------+----------------+------------+----------+";
        cout << "\n| " << left << setw(10) << accountNo 
             << " | " << setw(14) << name 
             << " | " << setw(10) << fixed << setprecision(2) << balance 
             << " | " << setw(8) << accountType << " |";
        cout << "\n+------------+----------------+------------+----------+";
    }

    // Encapsulation: Getters
    int getAccountNo() { return accountNo; }
    string getName() { return name; }
    double getBalance() { return balance; }
    string getAccountType() { return accountType; }
};


// ==================== DERIVED CLASS 1 (Savings) ====================
class SavingsAccount : public BankAccount
{
private:
    double interestRate;
    double minBalance;

public:
    SavingsAccount(int acc, string n, double b, double rate, double minBal)
        : BankAccount(acc, n, b, "Savings") // Base constructor call
    {
        interestRate = rate;
        minBalance = minBal;
    }

    // Function Overriding: withdraw with minimum balance rule
    bool withdraw(double amount) override
    {
        if (amount <= 0)
        {
            cout << "\n Invalid withdrawal amount!\n";
            return false;
        }

        if (balance - amount >= minBalance) // Accessing protected member 'balance' directly
        {
            balance = balance - amount;
            cout << "\n Amount withdrawn from Savings Account successfully!";
            cout << "\n Current Balance: Rs. " << balance << endl;
            return true;
        }
        else
        {
            cout << "\n Denied! Minimum balance of Rs. " << minBalance << " must be maintained.\n";
            return false;
        }
    }

    // Function Overriding: display
    void display() override
    {
        BankAccount::display(); // Call base display
        cout << "\n [Savings Details] Rate: " << interestRate << "% | Min Balance: Rs. " << minBalance << "\n";
    }
};


// ==================== DERIVED CLASS 2 (Current) ====================
class CurrentAccount : public BankAccount
{
private:
    double overdraftLimit;

public:
    CurrentAccount(int acc, string n, double b, double overdraft)
        : BankAccount(acc, n, b, "Current") // Base constructor call
    {
        overdraftLimit = overdraft;
    }

    // Function Overriding: withdraw with overdraft limit
    bool withdraw(double amount) override
    {
        if (amount <= 0)
        {
            cout << "\n Invalid withdrawal amount!\n";
            return false;
        }

        if (balance + overdraftLimit >= amount) // Accessing protected member 'balance' directly
        {
            balance = balance - amount;
            cout << "\n Amount withdrawn from Current Account successfully!";
            if (balance < 0)
            {
                cout << "\n [Alert] Overdraft used: Rs. " << (-balance) << " / " << overdraftLimit;
            }
            cout << "\n Current Balance: Rs. " << balance << endl;
            return true;
        }
        else
        {
            cout << "\n Denied! Overdraft limit of Rs. " << overdraftLimit << " exceeded.\n";
            return false;
        }
    }

    // Function Overriding: display
    void display() override
    {
        BankAccount::display(); // Call base display
        cout << "\n [Current Details] Overdraft Limit: Rs. " << overdraftLimit << "\n";
    }
};


// ==================== DATABASE & TABLE FILE HANDLING ====================

// Read all records from Bank_database.db table format
vector<AccountRecord> readDatabase()
{
    vector<AccountRecord> records;
    ifstream file("Bank_database.db");
    if (!file.is_open()) return records;

    string line;
    while (getline(file, line))
    {
        if (line.empty() || line[0] != '|') continue;
        if (line.find("ACCOUNT NO") != string::npos) continue; // Skip header

        stringstream ss(line);
        string dummy, idStr, name, balStr, type, extra;
        getline(ss, dummy, '|');
        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, balStr, '|');
        getline(ss, type, '|');
        getline(ss, extra, '|');

        idStr = trim(idStr);
        name = trim(name);
        balStr = trim(balStr);
        type = trim(type);
        extra = trim(extra);

        if (!idStr.empty() && idStr != "ACCOUNT NO")
        {
            try
            {
                AccountRecord r;
                r.accountNo = stoi(idStr);
                r.name = name;
                r.balance = stod(balStr);
                r.type = type;
                r.extra = extra;
                records.push_back(r);
            }
            catch (...)
            {
                // Skip invalid lines safely
            }
        }
    }
    file.close();
    return records;
}

// Save all records into Bank_database.db in Table format
void saveDatabase(const vector<AccountRecord>& records)
{
    ofstream file("Bank_database.db");
    file << "+------------+----------------+------------+----------+----------------------+\n";
    file << "| ACCOUNT NO | HOLDER NAME    | BALANCE(Rs)| TYPE     | EXTRA DETAILS        |\n";
    file << "+------------+----------------+------------+----------+----------------------+\n";

    for (size_t i = 0; i < records.size(); i++)
    {
        file << "| " << left << setw(10) << records[i].accountNo
             << " | " << setw(14) << records[i].name
             << " | " << setw(10) << fixed << setprecision(2) << records[i].balance
             << " | " << setw(8) << records[i].type
             << " | " << setw(20) << records[i].extra << " |\n";
    }

    file << "+------------+----------------+------------+----------+----------------------+\n";
    file.close();
}

void initializeDatabase()
{
    cout << "\n+-------------------------------------------------------------+";
    cout << "\n|                 DATABASE INITIALIZATION                     |";
    cout << "\n+-------------------------------------------------------------+";
    cout << "\n| [MySQL] CREATE DATABASE IF NOT EXISTS Bank_database;        |";
    cout << "\n| [MySQL] USE Bank_database;                                  |";
    cout << "\n| [MySQL] CREATE TABLE IF NOT EXISTS accounts (               |";
    cout << "\n|           account_no INT PRIMARY KEY,                       |";
    cout << "\n|           name VARCHAR(50),                                 |";
    cout << "\n|           balance DOUBLE,                                   |";
    cout << "\n|           account_type VARCHAR(20),                         |";
    cout << "\n|           extra_details VARCHAR(50)                         |";
    cout << "\n|         );                                                  |";
    cout << "\n+-------------------------------------------------------------+\n";

    ifstream check("Bank_database.db");
    if (!check.is_open())
    {
        vector<AccountRecord> emptyList;
        saveDatabase(emptyList);
    }
    else
    {
        check.close();
    }
}

bool checkAccountExists(int accNo)
{
    vector<AccountRecord> records = readDatabase();
    for (size_t i = 0; i < records.size(); i++)
    {
        if (records[i].accountNo == accNo) return true;
    }
    return false;
}

void addSavingsAccount()
{
    cout << "\n====== OPEN NEW SAVINGS ACCOUNT ======\n";
    int accNo = readIntInput("Enter Account Number: ");

    if (accNo <= 0)
    {
        cout << "\n[Error] Invalid Account Number!\n";
        return;
    }

    if (checkAccountExists(accNo))
    {
        cout << "\n[Error] Account " << accNo << " already exists in Database!\n";
        return;
    }

    cin.ignore();
    cout << "Enter Holder Name: ";
    string name;
    getline(cin, name);
    if (name.empty()) name = "Customer";

    double balance = readDoubleInput("Enter Initial Balance (Rs.): ");
    double rate = readDoubleInput("Enter Interest Rate (%): ");
    double minBal = readDoubleInput("Enter Minimum Balance (Rs.): ");

    SavingsAccount s(accNo, name, balance, rate, minBal);

    // MySQL Command Display
    cout << "\n[MySQL Query]: INSERT INTO accounts VALUES (" << accNo << ", '" << name 
         << "', " << balance << ", 'Savings', 'Rate:" << rate << "%,MinBal:" << minBal << "');\n";

    // Write to Bank_database.db table file
    vector<AccountRecord> records = readDatabase();
    AccountRecord newRec;
    newRec.accountNo = accNo;
    newRec.name = name;
    newRec.balance = balance;
    newRec.type = "Savings";
    newRec.extra = "Rate:" + to_string((int)rate) + "%,MinBal:" + to_string((int)minBal);
    records.push_back(newRec);

    saveDatabase(records);
    cout << " Account saved into 'Bank_database.db' in table format successfully!\n";
}

void addCurrentAccount()
{
    cout << "\n====== OPEN NEW CURRENT ACCOUNT ======\n";
    int accNo = readIntInput("Enter Account Number: ");

    if (accNo <= 0)
    {
        cout << "\n[Error] Invalid Account Number!\n";
        return;
    }

    if (checkAccountExists(accNo))
    {
        cout << "\n[Error] Account " << accNo << " already exists in Database!\n";
        return;
    }

    cin.ignore();
    cout << "Enter Holder Name: ";
    string name;
    getline(cin, name);
    if (name.empty()) name = "Customer";

    double balance = readDoubleInput("Enter Initial Balance (Rs.): ");
    double overdraft = readDoubleInput("Enter Overdraft Limit (Rs.): ");

    CurrentAccount c(accNo, name, balance, overdraft);

    // MySQL Command Display
    cout << "\n[MySQL Query]: INSERT INTO accounts VALUES (" << accNo << ", '" << name 
         << "', " << balance << ", 'Current', 'Overdraft:" << overdraft << "');\n";

    // Write to Bank_database.db table file
    vector<AccountRecord> records = readDatabase();
    AccountRecord newRec;
    newRec.accountNo = accNo;
    newRec.name = name;
    newRec.balance = balance;
    newRec.type = "Current";
    newRec.extra = "Overdraft:" + to_string((int)overdraft);
    records.push_back(newRec);

    saveDatabase(records);
    cout << " Account saved into 'Bank_database.db' in table format successfully!\n";
}

// Display table format like MySQL CLI
void displayAllFromDB()
{
    cout << "\n[MySQL Query]: SELECT * FROM accounts;\n\n";

    ifstream file("Bank_database.db");
    if (!file.is_open()) { cout << "\n Database file not found.\n"; return; }

    string line;
    int count = 0;
    while (getline(file, line))
    {
        cout << line << endl;
        if (!line.empty() && line[0] == '|' && line.find("ACCOUNT NO") == string::npos)
        {
            count++;
        }
    }
    file.close();

    cout << " Total: " << count << " row(s) in database table\n";
}

// Search and display single row table
void searchAccountInDB()
{
    int searchId = readIntInput("\nEnter Account Number to Search: ");

    cout << "\n[MySQL Query]: SELECT * FROM accounts WHERE account_no = " << searchId << ";\n";

    vector<AccountRecord> records = readDatabase();
    bool found = false;

    for (size_t i = 0; i < records.size(); i++)
    {
        if (records[i].accountNo == searchId)
        {
            cout << "\n+------------+----------------+------------+----------+----------------------+";
            cout << "\n| ACCOUNT NO | HOLDER NAME    | BALANCE(Rs)| TYPE     | EXTRA DETAILS        |";
            cout << "\n+------------+----------------+------------+----------+----------------------+";
            cout << "\n| " << left << setw(10) << records[i].accountNo 
                 << " | " << setw(14) << records[i].name 
                 << " | " << setw(10) << fixed << setprecision(2) << records[i].balance 
                 << " | " << setw(8) << records[i].type 
                 << " | " << setw(20) << records[i].extra << " |";
            cout << "\n+------------+----------------+------------+----------+----------------------+\n";
            found = true;
            break;
        }
    }

    if (!found) cout << "\n [MySQL] 0 rows in set. Account " << searchId << " not found in database.\n";
}

void updateTransaction(bool isDeposit)
{
    if (isDeposit) cout << "\n====== DEPOSIT MONEY ======\n";
    else cout << "\n====== WITHDRAW MONEY ======\n";

    int targetAcc = readIntInput("Enter Account Number: ");
    double amount = readDoubleInput("Enter Amount (Rs.): ");

    vector<AccountRecord> records = readDatabase();
    int foundIndex = -1;

    for (size_t i = 0; i < records.size(); i++)
    {
        if (records[i].accountNo == targetAcc)
        {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1)
    {
        cout << "\n Account " << targetAcc << " not found in database!\n";
        return;
    }

    bool success = false;
    double curBal = records[foundIndex].balance;

    if (isDeposit)
    {
        BankAccount temp(records[foundIndex].accountNo, records[foundIndex].name, curBal, records[foundIndex].type);
        temp.deposit(amount);
        records[foundIndex].balance = temp.getBalance();
        success = true;
    }
    else
    {
        if (records[foundIndex].type == "Savings")
        {
            SavingsAccount s(records[foundIndex].accountNo, records[foundIndex].name, curBal, 4.0, 1000.0);
            success = s.withdraw(amount);
            records[foundIndex].balance = s.getBalance();
        }
        else if (records[foundIndex].type == "Current")
        {
            CurrentAccount c(records[foundIndex].accountNo, records[foundIndex].name, curBal, 10000.0);
            success = c.withdraw(amount);
            records[foundIndex].balance = c.getBalance();
        }
        else
        {
            BankAccount temp(records[foundIndex].accountNo, records[foundIndex].name, curBal, records[foundIndex].type);
            success = temp.withdraw(amount);
            records[foundIndex].balance = temp.getBalance();
        }
    }

    if (success)
    {
        cout << "\n[MySQL Query]: UPDATE accounts SET balance = " << records[foundIndex].balance 
             << " WHERE account_no = " << targetAcc << ";\n";

        saveDatabase(records);
        cout << " Database file 'Bank_database.db' updated successfully in table format!\n";
    }
}

void deleteAccountFromDB()
{
    cout << "\n====== DELETE ACCOUNT ======\n";
    int targetAcc = readIntInput("Enter Account Number to Delete: ");

    vector<AccountRecord> records = readDatabase();
    vector<AccountRecord> updatedRecords;
    bool found = false;

    for (size_t i = 0; i < records.size(); i++)
    {
        if (records[i].accountNo == targetAcc)
        {
            found = true;
        }
        else
        {
            updatedRecords.push_back(records[i]);
        }
    }

    if (found)
    {
        cout << "\n[MySQL Query]: DELETE FROM accounts WHERE account_no = " << targetAcc << ";\n";
        saveDatabase(updatedRecords);
        cout << " Account " << targetAcc << " deleted successfully from 'Bank_database.db'!\n";
    }
    else
    {
        cout << "\n Account " << targetAcc << " not found!\n";
    }
}

// Display raw database file
void showRawDatabaseFile()
{
    cout << "\n================ RAW CONTENT OF 'Bank_database.db' ================\n";
    ifstream file("Bank_database.db");
    if (!file.is_open())
    {
        cout << " File does not exist.\n";
        return;
    }

    string line;
    while (getline(file, line))
    {
        cout << line << endl;
    }
    file.close();
    cout << "===================================================================\n";
}


// ==================== MAIN FUNCTION ====================
int main()
{
    initializeDatabase();
    int choice = 0;

    do
    {
        cout << "\n\n==========================================";
        cout << "\n         BANK DATABASE SYSTEM";
        cout << "\n==========================================";
        cout << "\n 1. Open Savings Account  (INSERT)";
        cout << "\n 2. Open Current Account  (INSERT)";
        cout << "\n 3. Deposit Money         (UPDATE)";
        cout << "\n 4. Withdraw Money        (UPDATE)";
        cout << "\n 5. Search Account by ID  (SELECT WHERE)";
        cout << "\n 6. Display All Accounts  (SELECT *)";
        cout << "\n 7. Delete Account        (DELETE)";
        cout << "\n 8. View 'Bank_database.db' File";
        cout << "\n 9. Exit";
        cout << "\n==========================================";
        choice = readIntInput("\n Enter your choice (1-9): ");

        switch (choice)
        {
            case 1: addSavingsAccount(); break;
            case 2: addCurrentAccount(); break;
            case 3: updateTransaction(true); break;  // Deposit
            case 4: updateTransaction(false); break; // Withdraw
            case 5: searchAccountInDB(); break;
            case 6: displayAllFromDB(); break;
            case 7: deleteAccountFromDB(); break;
            case 8: showRawDatabaseFile(); break;
            case 9: cout << "\n Thank you for using Bank Database System!\n"; break;
            default: cout << "\n Invalid Choice! Please enter choice between 1 and 9.\n";
        }

    } while (choice != 9);

    return 0;
}
