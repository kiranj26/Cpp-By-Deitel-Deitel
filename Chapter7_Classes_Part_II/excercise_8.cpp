/*
Exercise 8: SavingsAccount class

This program implements a SavingsAccount class that models a bank savings account.
- The class has a static data member `annualInterestRate` shared by all accounts.
- Each SavingsAccount object stores its own `savingsBalance`.
- The constructor initializes the balance (non-negative).
- `calculateMonthlyInterest` computes monthly interest as (savingsBalance * annualInterestRate) / 12 and adds it to the balance.
- `modifyInterestRate` is a static function to set the annual interest rate (non-negative).
- `main` demonstrates the class by creating two savers, setting the interest rate to 3%, calculating monthly interest, displaying balances,
    then changing the rate to 4%, recalculating interest, and displaying updated balances.
*/

#include <iostream>
using namespace std;

// Create a SavingsAccount Class
class SavingsAccount {
        // create a static data member to contains the annual interest rate
        static double annualInterestRate;

        private:
                // indincating amount saver currently have in their savings account
                double savingsBalance;  

        public:
                // constructor to initialize savingsBalance
                SavingsAccount(double initialBalance) {
                        if (initialBalance >= 0) {
                                savingsBalance = initialBalance;
                        } else {
                                savingsBalance = 0;
                                cout << "Initial balance invalid. Setting to 0." << endl;
                        }
                }   

                ~SavingsAccount() {}

                double getSavingsBalance() const {
                        return savingsBalance;
                }

                // function to calculate monthly interst rate
                void calculateMonthlyInterest() {
                        double monthlyInterest = (savingsBalance * annualInterestRate) / 12;
                        savingsBalance += monthlyInterest;
                }

                // function static to set the annual interest rate
                static void modifyInterestRate(double newRate) {
                        if (newRate >= 0) {
                                annualInterestRate = newRate;
                        } else {
                                cout << "Interest rate invalid. Setting to 0." << endl;
                                annualInterestRate = 0;
                        }
                }

                static double getAnnualInterestRate() {
                        return annualInterestRate;
                }

};

double SavingsAccount::annualInterestRate = 0; // initialize static member

int main(){
        // drive program to test the SavingsAccount class
        SavingsAccount saver1(2000.0);
        SavingsAccount saver2(3000.0);  
        // set annual interest rate to 3%
        SavingsAccount::modifyInterestRate(0.03);

        saver1.calculateMonthlyInterest();
        saver2.calculateMonthlyInterest();

        cout << "Saver 1 balance: " << saver1.getSavingsBalance() << endl;
        cout << "Saver 2 balance: " << saver2.getSavingsBalance() << endl;

        // set annual interest rate to 4%
        SavingsAccount::modifyInterestRate(0.04);
        saver1.calculateMonthlyInterest();
        saver2.calculateMonthlyInterest();

        cout << "After modifying interest rate to 4%:" << endl;
        cout << "Saver 1 balance: " << saver1.getSavingsBalance() << endl;
        cout << "Saver 2 balance: " << saver2.getSavingsBalance() << endl;

        return 0;
}