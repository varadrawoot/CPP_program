#include<iostream>
#include<string>
using namespace std;
class BankAccount
{
    private:
        int accountno;
        string name;
        double balance;

    public:

    BankAccount(int acc, string n, double b)// We use constructor
    {
        accountno=acc;
        name=n;
        balance=b;
    }

    // Display bank account details.
    void display() const
    {
        cout << "\n------Account Details------\n";
        cout << "Account No: " << accountno << '\n';
        cout << "Name: " << name << '\n';
        cout << "Balance: " << balance << '\n';
    }

    // Deposit Money
    void deposit(double amount)
    {
        if (amount > 0)
        {
            balance += amount;
            cout << "\n Amount deposited successfully! \n";
        }
        else
        {
            cout << "\n Invalid deposit amount! \n";
        }
    }

    // Deposit Money
    void withdraw(double amount)//parameter added
    {
        if (amount <= balance)
        {
            balance = balance - amount;
            cout <<"\n Amount withdrawn sucessfully! \n";
        }
        else
        {
            cout <<"\n Insufficient balance! \n";
        }
    }

    // Display the current balance.
    void checkBalance() const
    {
        cout << "\nCurrent Balance: " << balance << '\n';
    }
};
int main()
{
    int accountno;
    string name;
    double balance;


    cout<<"\n====== Bank Account Management ======";

    cout<<"\n Enter your account no:";
    cin>>accountno;
    cin.ignore();

    cout<<"Enter Name:";
    getline(cin,name);

    cout<<"Enter initail balance:";
    cin>>balance;

    // calling a constructor here
    BankAccount account(accountno, name, balance);

    int choice;
    double amount;

    do
    {
        cout<<"====Menue====\n";
        cout<<"1) Display Account\n";
        cout<<"2) Deposit money\n";
        cout<<"3) Withdraw Money\n";
        cout<<"4) Check Balance\n";
        cout<<"5) Exit\n";

        cout<<"Enter your choiice:";
        cin>>choice;

        switch (choice)
        {
            case 1:
                account.display();
                break;
            
            case 2:
                cout<<"Enter amount to deposit:";
                cin>>amount;
                account.deposit(amount);
                break;

            case 3:
                cout<<"Enter amount to withdraw:";
                cin>>amount;
                account.withdraw(amount);
                break;

            case 4:
                account.checkBalance();
                break;
                
            case 5:
                cout<<"\n Thankyou for using Bank Account System \n";
                break;

            default:
                cout<<"\n Invalid choice! \n";
        }
    }
     while (choice != 5);
    return 0;
}