
/*
    This is the program of Expense Tracker
    so that we can track our expenses and set a spending limit.

    Concepts used:
    
    1) Constructor
    2) Encapsulation
    3) Header files like  iomanip
*/

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class ExpenseTracker
{
private:
    int id[100];
    string category[100];
    double amount[100];
    string date[100];
    string note[100];

    int count;
    double spendingLimit;
    double totalExpense;

public:

    ExpenseTracker()
    {
        count = 0;
        spendingLimit = 0;
        totalExpense = 0;
    }

    void setLimit()
    {
        cout << "\nEnter Spending Limit: Rs. ";
        cin >> spendingLimit;

        cout << "Spending limit set successfully!\n";
    }

    void addExpense()
    {
        if (count >= 100)
        {
            cout << "\nExpense limit reached!\n";
            return;
        }

        cout << "\n========== ADD EXPENSE ==========\n";

        cout << "Enter Expense ID: ";
        cin >> id[count];

        cin.ignore();

        cout << "Enter Category: ";
        getline(cin, category[count]);

        cout << "Enter Amount: Rs. ";
        cin >> amount[count];

        cin.ignore();

        cout << "Enter Date: ";
        getline(cin, date[count]);

        cout << "Enter Note: ";
        getline(cin, note[count]);

        totalExpense = totalExpense + amount[count];

        count++;

        cout << "\nExpense added successfully!\n";

        checkLimit();
    }

    void displayAll()
    {
        if (count == 0)
        {
            cout << "\nNo expenses found.\n";
            return;
        }

        cout << "\n================ ALL EXPENSES ================\n";

        cout << left
             << setw(8) << "ID"
             << setw(18) << "Category"
             << setw(12) << "Amount"
             << setw(15) << "Date"
             << "Note" << endl;

        cout << "------------------------------------------------------------\n";

        for (int i = 0; i < count; i++)
        {
            cout << left
                 << setw(8) << id[i]
                 << setw(18) << category[i]
                 << setw(12) << amount[i]
                 << setw(15) << date[i]
                 << note[i] << endl;
        }

        cout << "============================================================\n";
    }

    void searchExpense()
    {
        int searchId;

        cout << "\nEnter Expense ID to Search: ";
        cin >> searchId;

        for (int i = 0; i < count; i++)
        {
            if (id[i] == searchId)
            {
                cout << "\nExpense Found!\n";

                cout << "ID       : " << id[i] << endl;
                cout << "Category : " << category[i] << endl;
                cout << "Amount   : Rs. " << amount[i] << endl;
                cout << "Date     : " << date[i] << endl;
                cout << "Note     : " << note[i] << endl;

                return;
            }
        }

        cout << "\nExpense not found.\n";
    }

    void calculateTotal()
    {
        cout << fixed << setprecision(2);

        cout << "\n========== TOTAL EXPENSE ==========\n";

        cout << "Total Expense  : Rs. "
             << totalExpense << endl;

        cout << "Spending Limit : Rs. "
             << spendingLimit << endl;

        if (totalExpense < spendingLimit)
        {
            cout << "Remaining      : Rs. "
                 << spendingLimit - totalExpense << endl;
        }
        else
        {
            cout << "\nSTOP SPENDING!\n";
            cout << "You have reached your spending limit.\n";
        }
    }

    void deleteExpense()
    {
        int searchId;
        int position = -1;

        cout << "\nEnter Expense ID to Delete: ";
        cin >> searchId;

        for (int i = 0; i < count; i++)
        {
            if (id[i] == searchId)
            {
                position = i;
                break;
            }
        }

        if (position == -1)
        {
            cout << "\nExpense ID not found.\n";
            return;
        }

        totalExpense = totalExpense - amount[position];

        // Shift remaining expenses one position left
        for (int i = position; i < count - 1; i++)
        {
            id[i] = id[i + 1];
            category[i] = category[i + 1];
            amount[i] = amount[i + 1];
            date[i] = date[i + 1];
            note[i] = note[i + 1];
        }

        count--;

        cout << "\nExpense deleted successfully!\n";
    }

    void checkLimit()
    {
        if (spendingLimit > 0 &&
            totalExpense >= spendingLimit)
        {
            cout << "\n************************************\n";
            cout << "             WARNING!\n";
            cout << "************************************\n";

            cout << "Total Expense  : Rs. "
                 << totalExpense << endl;

            cout << "Spending Limit : Rs. "
                 << spendingLimit << endl;

            cout << "\nSTOP SPENDING!\n";
            cout << "You have reached your spending limit.\n";

            cout << "************************************\n";
        }
    }

    void menu()
    {
        int choice;

        do
        {
            cout << "\n\n====================================\n";
            cout << "          EXPENSE TRACKER\n";
            cout << "====================================\n";

            cout << "\n1. Add Expense";
            cout << "\n2. Display All Expenses";
            cout << "\n3. Search Expense";
            cout << "\n4. Calculate Total Expense";
            cout << "\n5. Delete Expense";
            cout << "\n6. Set Spending Limit";
            cout << "\n7. Exit";

            cout << "\n\nEnter your choice: ";
            cin >> choice;

            switch (choice)
            {
                case 1:
                    addExpense();
                    break;

                case 2:
                    displayAll();
                    break;

                case 3:
                    searchExpense();
                    break;

                case 4:
                    calculateTotal();
                    break;

                case 5:
                    deleteExpense();
                    break;

                case 6:
                    setLimit();
                    break;

                case 7:
                    cout << "\nThank you for using Expense Tracker!\n";
                    break;

                default:
                    cout << "\nInvalid choice! Try again.\n";
            }

        } while (choice != 7);
    }
};


int main()
{
    ExpenseTracker tracker;

    cout << "====================================\n";
    cout << "          EXPENSE TRACKER\n";
    cout << "====================================\n";

    tracker.setLimit();

    tracker.menu();

    return 0;
}