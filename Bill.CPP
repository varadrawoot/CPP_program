#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const int N = 5;
const float FIXED_CHARGE = 50.0;
const float RATE1 = 3.50;  // first 100 units
const float RATE2 = 5.50;  // 101 - 300 units
const float RATE3 = 7.50;  // above 300 units

class Consumer
{
protected:
    int consumerNo;
    string consumerName;
    int previousReading;
    int currentReading;
    int units;

public:
    Consumer()
    {
        consumerNo = 0;
        consumerName = "";
        previousReading = 0;
        currentReading = 0;
        units = 0;
    }

    void getData()
    {
        cout << "Enter Consumer No      : ";
        cin >> consumerNo;
        cin.ignore();
        cout << "Enter Consumer Name    : ";
        getline(cin, consumerName);
        cout << "Enter Previous Reading : ";
        cin >> previousReading;
        do
        {
            cout << "Enter Current Reading  : ";
            cin >> currentReading;
            if (currentReading < previousReading)
                cout << "Current reading cannot be less than previous reading. Try again.\n";
        } while (currentReading < previousReading);
        units = currentReading - previousReading;
    }
};

class Bill : public Consumer
{
private:
    float energyCharge;
    float totalAmount;

public:
    Bill()
    {
        energyCharge = 0;
        totalAmount = 0;
    }

    void calculateBill()
    {
        if (units <= 100)
            energyCharge = units * RATE1;
        else if (units <= 300)
            energyCharge = (100 * RATE1) + (units - 100) * RATE2;
        else
            energyCharge = (100 * RATE1) + (200 * RATE2) + (units - 300) * RATE3;
        totalAmount = energyCharge + FIXED_CHARGE;
    }

    void displayBill()
    {
        cout << "\n----------------------------------------\n";
        cout << "          ELECTRICITY BILL\n";
        cout << "----------------------------------------\n";
        cout << "Consumer No      : " << consumerNo << endl;
        cout << "Consumer Name    : " << consumerName << endl;
        cout << "Previous Reading : " << previousReading << endl;
        cout << "Current Reading  : " << currentReading << endl;
        cout << "Units Consumed   : " << units << endl;
        cout << fixed << setprecision(2);
        cout << "Energy Charge    : Rs. " << energyCharge << endl;
        cout << "Fixed Charge     : Rs. " << FIXED_CHARGE << endl;
        cout << "Total Bill       : Rs. " << totalAmount << endl;
        cout << "----------------------------------------\n";
    }

    void writeToFile(ofstream &fout)
    {
        fout << fixed << setprecision(2);
        fout << "----------------------------------------\n";
        fout << "          ELECTRICITY BILL\n";
        fout << "----------------------------------------\n";
        fout << "Consumer No      : " << consumerNo << endl;
        fout << "Consumer Name    : " << consumerName << endl;
        fout << "Previous Reading : " << previousReading << endl;
        fout << "Current Reading  : " << currentReading << endl;
        fout << "Units Consumed   : " << units << endl;
        fout << "Energy Charge    : Rs. " << energyCharge << endl;
        fout << "Fixed Charge     : Rs. " << FIXED_CHARGE << endl;
        fout << "Total Bill       : Rs. " << totalAmount << endl;
        fout << "----------------------------------------\n\n";
    }
};

int main()
{
    Bill b[N];

    cout << "====== ELECTRICITY BILL GENERATOR ======\n";
    cout << "Slab Rates: 0-100 units   @ Rs. 3.50\n";
    cout << "            101-300 units @ Rs. 5.50\n";
    cout << "            Above 300     @ Rs. 7.50\n";
    cout << "Fixed Charge: Rs. 50 per bill\n\n";

    for (int i = 0; i < N; i++)
    {
        cout << "\n--- Consumer " << i + 1 << " ---\n";
        b[i].getData();
        b[i].calculateBill();
    }

    ofstream fout("bills.txt");
    if (!fout)
    {
        cout << "Error: could not open file bills.txt\n";
        return 1;
    }

    cout << "\n\n========== GENERATED BILLS ==========";
    for (int i = 0; i < N; i++)
    {
        b[i].displayBill();
        b[i].writeToFile(fout);
    }

    fout.close();
    cout << "\nAll bills have been saved in the file bills.txt\n";
    return 0;
}