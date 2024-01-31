#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

using namespace std;

//variable declaration;
struct ExpenseEntry {
    double amount;
    string details;
    int month;
};

struct User {
    string username;
    string password;
    double loanAmount;  
};

const int MAX_CATEGORIES=4,MAX_EXPENSES_PER_CATEGORY = 100,NUM_MONTHS = 12;
void writeUsers(const string& filename, const User users[], int numUsers) ;
const string userfilepath="C:\\CProgram\\users.txt";
const string expenseFilePath = "C:\\CProgram\\expenses\\";

void Category(const string expenseCategories[]){
	cout << "                                    CATEGORIES " << endl;
	cout << "                        +-------------------------------+"<< endl;
	for (int i = 0; i < MAX_CATEGORIES; ++i) {
        cout <<"\t\t\t|\t"<< i + 1 << ". " << expenseCategories[i] <<"\t\t|"<< endl;
    }
	cout << "                        +-------------------------------+"<< endl;
}
void Header(){
	cout << "              -------------------------------------------------------------"<<endl<<endl;
    cout << "                                       FINSTRAT"<<endl<<endl;
    cout << "              -------------------------------------------------------------"<<endl;
}

void ClearScreen() {
	cin.ignore();
    cout << "                           Press any key to continue...";
    // Platform-specific clear screen command
    getchar();
    system ("CLS");
}

// Function to save user expenses to a file
void saveExpenses(const string& username, const ExpenseEntry expenses[MAX_CATEGORIES][MAX_EXPENSES_PER_CATEGORY], const int expensesCount[MAX_CATEGORIES]) {
    ofstream expenseFile((expenseFilePath + username + "_expenses.txt").c_str(), ios::trunc);   // Open the file in truncate mode
    if (expenseFile.is_open()) {
        for (int i = 0; i < MAX_CATEGORIES; ++i) {
            for (int j = 0; j < expensesCount[i]; ++j) {
                expenseFile << i << " " << expenses[i][j].amount << " " << expenses[i][j].details << " " << expenses[i][j].month << endl;
            }
        }
        expenseFile.close();
    }
}

// Function to load user expenses from a file
void loadUserExpenses(const string& username, ExpenseEntry expenses[MAX_CATEGORIES][MAX_EXPENSES_PER_CATEGORY], int expensesCount[MAX_CATEGORIES]) {
    ifstream expenseFile((expenseFilePath + username + "_expenses.txt").c_str());
    if (expenseFile.is_open()) {
        while (true) {
            int category, month;
            double amount;
            string details;
            expenseFile >> category >> amount >> details >> month;
            if (expenseFile.eof()) {
                break;
            }
            ExpenseEntry newExpense;
            newExpense.amount = amount;
            newExpense.details = details;
            newExpense.month = month;
            expenses[category][expensesCount[category]++] = newExpense;
        }
        expenseFile.close();
    }
}

// Function to register a new user
void registerUser(User users[], int& numUsers, const string& username, const string& password) {
    for (int i = 0; i < numUsers; ++i) {
        if (users[i].username == username) {
            cout << "            !!   Username already exists. Please enter a new Username   !!" << endl;
            return; // Registration failed
        }
    }
    User newUser;
    newUser.username = username;
    newUser.password = password;
    newUser.loanAmount = 0.0; 
    users[numUsers] = newUser;
    numUsers++;
    writeUsers(userfilepath, users, numUsers);
    cout << "                                Registration successful!" << endl;
}

// Function to read user information from a text file
void readUser(const string& filename, User users[], int& numUsers) {
    ifstream userFile(filename.c_str());
    numUsers = 0;
    if (userFile.is_open()) {
        while (userFile >> users[numUsers].username >> users[numUsers].password >> users[numUsers].loanAmount) {
            numUsers++;
        }
        userFile.close();
    }
}

// Function to write user information to a text file
void writeUsers(const string& filename, const User users[], int numUsers) {
    ofstream userFile(filename.c_str());
    if (userFile.is_open()) {
        for (int i = 0; i < numUsers; ++i) {
            userFile << users[i].username << " " << users[i].password << " " << users[i].loanAmount << endl;
        }
        userFile.close();
    }
}

// Function to find user index by username
int findUserIndex(const User users[], int numUsers, const string& username) {
    for (int i = 0; i < numUsers; ++i) {
        if (users[i].username == username) {
            return i;
        }
    }
    return -1;  // User not found
}

// Function to add an expense
void addExpense(User& user, ExpenseEntry expenses[MAX_CATEGORIES][MAX_EXPENSES_PER_CATEGORY],
        int expensesCount[MAX_CATEGORIES], double monthlyBudget[], const string expenseCategories[]){
    double expense;
    int category, month;
	Category(expenseCategories);
    cout<<"                        Choose a category: ";
    cin >> category;
    if (category >= 1 && category <= MAX_CATEGORIES) {
        category--; // Adjust to 0-based index
        cout << "                        Month (1-12): ";
        cin >> month;
        if (month < 1 || month > 12) {
            cout << "                     !!   Invalid month. Please try again.   !!" << endl;
            return;
        }
        cout << "                        Amount (" << expenseCategories[category] << "): RM";
        cin >> expense;
        if (monthlyBudget[month - 1] - expense < 0) {
            cout << "                             !!   Budget Exceed   !!" << endl;
            return;
        }
        // Add the new expense
        expenses[category][expensesCount[category]].month = month;
        cout << "                        Details(e.g. description): ";
        cin.ignore();
        getline(cin, expenses[category][expensesCount[category]].details);
        expenses[category][expensesCount[category]].amount = expense;
        expensesCount[category]++;
        monthlyBudget[month - 1] -= expense;
        cout << "                        Expense added successfully."<<endl;
		cout << "                        Remaining budget: RM" << monthlyBudget[month - 1];
    } else {
        cout << "                       !!   Invalid category choice.   !!" << endl;
    }
}

// Function to update expenses
void updateExpense(User& user, ExpenseEntry expenses[MAX_CATEGORIES][MAX_EXPENSES_PER_CATEGORY],
        int expensesCount[MAX_CATEGORIES], double monthlyBudget[], const string expenseCategories[]) {
    Category(expenseCategories);
    cout<<"                          Choose a category to update: ";
    int updateChoice;
    cin >> updateChoice;
    if (updateChoice >= 1 && updateChoice <= MAX_CATEGORIES) {
        int categoryIndex = updateChoice - 1; // Adjust to 0-based index
        string category = expenseCategories[categoryIndex];
        if (expensesCount[categoryIndex] > 0) {
        	cout << "              -------------------------------------------------------------"<<endl;
            cout << "                             Expenses for '" << category <<"'" <<endl;
            cout <<"\t\tNo\tMonth\tAmount(RM)\tDetails" <<endl;
            for (int j = 0; j < expensesCount[categoryIndex]; ++j) {
                cout << "\t\t" << j + 1 << ".\t" << expenses[categoryIndex][j].month
                     << "\t" << expenses[categoryIndex][j].amount
                     << "\t\t" << expenses[categoryIndex][j].details << endl;
            }
            cout << "              -------------------------------------------------------------"<<endl;
            cout << "                        No. of the expense to update: ";
            int updateEntry;
            cin >> updateEntry;
            if (updateEntry >= 1 && updateEntry <= expensesCount[categoryIndex]) {
            	cout << "                        Month (1-12): ";
                int newMonth;
                cin >> newMonth;
                if (newMonth < 1 || newMonth > 12) {
                    cout << "                     !!   Invalid month. Please try again.   !!" << endl;
                    return;
				}
                cout << "                        Amount(" << category << "): RM";
                double newAmount;
                cin >> newAmount;
				if (monthlyBudget[newMonth - 1] - newAmount < 0) {
		            cout << "                             !!   Budget Exceed   !!" << endl;
		            return;
		        }
                cout << "                        Details(e.g. description): ";
                cin.ignore();
                getline(cin, expenses[categoryIndex][updateEntry - 1].details);
                // Adjust the budget based on the selected expense's month
                monthlyBudget[newMonth - 1] += expenses[categoryIndex][updateEntry - 1].amount-newAmount;
                expenses[categoryIndex][updateEntry - 1].amount = newAmount;
                expenses[categoryIndex][updateEntry - 1].month = newMonth;
                monthlyBudget[newMonth - 1] -= newAmount;
                cout << "                        Expense updated successfully."<<endl;
				cout << "                        Remaining budget: RM" << monthlyBudget[newMonth - 1];
            } else {
                cout << "                       !!   Invalid entry number.  !!" << endl;
            }
        } else {
            cout << endl <<"                         No expenses found for '" << category << "'." << endl<<endl;
        }
    } else {
        cout << "                       !!   Invalid category choice.   !!" << endl;
    }
}

// Function to delete expenses
void deleteExpense(User& user, ExpenseEntry expenses[MAX_CATEGORIES][MAX_EXPENSES_PER_CATEGORY],
        int expensesCount[MAX_CATEGORIES], double monthlyBudget[], const string expenseCategories[]) {
    Category(expenseCategories);
    cout<<"                          Choose a category to update: ";
    int category;
    cin >> category;
    if (category >= 1 && category <= MAX_CATEGORIES) {
        category--; // Adjust to 0-based index
        if (expensesCount[category] > 0) {
        	cout << "              -------------------------------------------------------------"<<endl;
            cout << "                             Expenses for '" << expenseCategories[category] << "'."<< endl;
            cout <<"\t\tNo\tMonth\tAmount(RM)\tDetails" <<endl;
            for (int j = 0; j < expensesCount[category]; ++j) {
                cout << "\t\t" << j + 1 << ".\t" << expenses[category][j].month
                     << "\t" << expenses[category][j].amount
                     << "\t\t" << expenses[category][j].details << endl;
            }
            cout << "              -------------------------------------------------------------"<<endl;
            cout << "                        No. of the expense to delete: ";
            int deleteEntry;
            cin >> deleteEntry;
            if (deleteEntry >= 1 && deleteEntry <= expensesCount[category]) {
                // Adjust the budget based on the selected expense's month
                monthlyBudget[expenses[category][deleteEntry - 1].month - 1] += expenses[category][deleteEntry - 1].amount;

                // Deduct the expense from the monthly budget
                for (int k = deleteEntry - 1; k < expensesCount[category] - 1; ++k) {
                    expenses[category][k] = expenses[category][k + 1];
                }
                expensesCount[category]--;
                cout << "                        Expense deleted successfully." << endl;
				cout << "                        Remaining budget: RM" << monthlyBudget[expenses[category][deleteEntry - 1].month - 1] << endl;
            } else {
                cout << "                       !!   Invalid entry number.  !!" << endl;
            }
        } else {
            cout << endl <<"                         No expenses found for '" << expenseCategories[category] << "'." << endl<<endl;
        }
    } else {
        cout << "                       !!   Invalid category choice.   !!" << endl;
    }
}

//Function to print expenses
void printExpenses(User& user, const ExpenseEntry expenses[MAX_CATEGORIES][MAX_EXPENSES_PER_CATEGORY], const int expensesCount[MAX_CATEGORIES], double monthlyBudget[],double initialBudget) {
    bool hasExpenses = false;
    // Iterate over months
    for (int month = 1; month <= 12; ++month) {
        // Check if there are expenses for the current month
        bool hasExpensesForMonth = false;
        double totalExpensesForMonth = 0.0;
        // Display month only if there are expenses
        for (int i = 0; i < MAX_CATEGORIES; ++i) {
            for (int j = 0; j < expensesCount[i]; ++j) {
                if (expenses[i][j].month == month) {
                    hasExpensesForMonth = true;
                    totalExpensesForMonth += expenses[i][j].amount;
                    break;
                }
            }
        }
        if (hasExpensesForMonth) {
            cout << "\nExpense Report for Month " << month << ":\n";
            cout << setw(20) << left << "Category" << setw(10) << right << "Expense (RM)" << setw(20) << "Details" << endl;
            cout << "-----------------------------------------------------" << endl;
            // Deduct budget for the specific month
            monthlyBudget[month - 1] = initialBudget - totalExpensesForMonth - user.loanAmount;
            for (int i = 0; i < MAX_CATEGORIES; ++i) {
                for (int j = 0; j < expensesCount[i]; ++j) {
                    if (expenses[i][j].month == month) {
                        hasExpenses = true;
                        cout << setw(20) << left << i << setw(10) << right << fixed << setprecision(2)
                             << expenses[i][j].amount << setw(20) << expenses[i][j].details << endl;
                    }
                }
            }
            cout << "-----------------------------------------------------" << endl;
			// Deduct loan amount when viewing expenses
            cout << "Loan amount deducted: RM" << user.loanAmount << endl;
            // Display remaining budget for the month
            cout << "Remaining budget for Month " << month << ": RM" << monthlyBudget[month - 1] << endl;
            cout << "-----------------------------------------------------" << endl;
        }
    }
    if (!hasExpenses) {
        cout << "\nNo expenses recorded." << endl;
    }
}

// Function to enter a loan for a user
void enterLoan(User& user) {
    double loanAmount;
    cout << "                                Loan amount: RM";
    cin >> loanAmount;
    user.loanAmount = loanAmount;  // Update the loan amount with the user input
    // Update the user information in the file
    writeUsers(userfilepath, &user, 1);
    cout << "                                Loan added successfully. " << endl;
	cout << "                                Total loan amount: RM" << user.loanAmount << endl;
}

int main() {
	const string expenseCategories[MAX_CATEGORIES] = {"Groceries", "Leisure", "Utilities", "Others"};
    User users[100]; // Assuming a maximum of 100 users
    ExpenseEntry expenses[MAX_CATEGORIES][MAX_EXPENSES_PER_CATEGORY];
    int expensesCount[MAX_CATEGORIES] = {0};
    double budget,monthlyBudget[NUM_MONTHS];
    int monthChoice,userIndex,numUsers=0;
    readUser(userfilepath, users, numUsers);
	
    while (true) {
    	//Enter choice to log in
    	Header();
        cout << "                             Welcome to your expense tracker!" <<endl<<endl;
        cout << "                                        MENU " << endl;
        cout << "                        +---------------------------------+"<<endl;
        cout << "                        |            1. Log In            |" << endl;
        cout << "                        |       2. Register a New User    |" << endl;
        cout << "                        |             3. Quit             |"<< endl;
    	cout << "                        +---------------------------------+"<<endl;
        cout << "                        Enter your choice: ";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:{ // Log In
				string username, password;
				cout << "              -------------------------------------------------------------"<< endl;
				cout <<"                                       LOG IN"<<endl;
                cout << "                                  Username: ";
                cin >> username;
                cout << "                                  Password: ";
                cin >> password;
				//Find User
                int userIndex = findUserIndex(users, numUsers, username);
                if (userIndex != -1 && users[userIndex].password == password) {
                    cout << "                                Login successful!" << endl;
                    ClearScreen();
                    loadUserExpenses(users[userIndex].username, expenses, expensesCount);
                    // Option to enter budget after successful login
                    cout << "                               Monthly budget: RM";
                    cin >> budget;
                    for (int i = 0; i < NUM_MONTHS; ++i) {
					    monthlyBudget[i] = budget;
					}
					bool cont=false;
                    while (!cont) {
                    	//Choose an action
                    	Header();
                        int action;
                        cout << "                                        MENU " << endl;
        				cout << "                        +----------------------------------+"<< endl;
                        cout << "                        |          1. Add Expense          |" << endl;
                        cout << "                        |          2. Update Expense       |" << endl;
                        cout << "                        |          3. Delete Expense       |" << endl;
                        cout << "                        |          4. View Expenses        |" << endl;
                        cout << "                        |          5. Enter Loan           |" << endl;
                        cout << "                        |          6. Done                 |" << endl;
                        cout << "                        +----------------------------------+"<< endl;
                        cout << "                                Enter your choice: ";
                        cin >> action;
                        cout <<"              -------------------------------------------------------------"<<endl;
                        switch (action) {
		                    case 1: // Add Expense
	                            addExpense(users[userIndex], expenses, expensesCount, monthlyBudget, expenseCategories);
	                            ClearScreen();
	                            break;
	                        case 2: // Update Expense
	                            updateExpense(users[userIndex], expenses, expensesCount, monthlyBudget, expenseCategories);
	                            ClearScreen();
	                            break;
	                        case 3: // Delete Expense
	                            deleteExpense(users[userIndex], expenses, expensesCount, monthlyBudget, expenseCategories);
	                            ClearScreen();
	                            break;
                            case 4: // View Expenses
                            	system ("CLS");
                                printExpenses(users[userIndex], expenses, expensesCount, monthlyBudget,budget);
                                ClearScreen();
                                break;
                            case 5:  // Enter Loan
                                enterLoan(users[userIndex]);
                                ClearScreen();
                                break;
                            case 6:  // Done
                            	saveExpenses(users[userIndex].username, expenses, expensesCount);
                            	users[userIndex] = User();

							    // Reset expenses data
							    for (int i = 0; i < MAX_CATEGORIES; ++i) {
							        expensesCount[i] = 0;
							        for (int j = 0; j < MAX_EXPENSES_PER_CATEGORY; ++j) {
							            expenses[i][j] = ExpenseEntry();
							        }
							    }
							    // Reset monthly budget data
							    for (int i = 0; i < NUM_MONTHS; ++i) {
							        monthlyBudget[i] = 0;
							    }
							    // Reset budget
							    budget = 0;
                            	ClearScreen();
                            	cont=true;
                            	break;
                            default:
                                cout << "                     !!   Invalid choice. Please try again.   !!" << endl;
                                ClearScreen();
                                break;
	                        }
	                    }
	                } else {
	                    cout << "                     !!   Login failed. Please try again.   !!" << endl;
	                    ClearScreen();
	                }
	            }
	            break;
            case 2: {// Register a New User
				string username, password;
				cout << "              -------------------------------------------------------------"<<endl;
				cout <<"                                         REGISTER"<<endl;
                cout << "                                  Username: ";
                cin >> username;
                cout << "                                  Password: ";
                cin >> password;
                registerUser(users, numUsers, username, password);
                ClearScreen();
            }
                break;
            case 3: // Quit
                goto exit_loop;
            default:
                cout << "                     !!   Invalid choice. Please try again.   !!" << endl;
                ClearScreen();
                break;
        }
    }
exit_loop:
    writeUsers("users.txt", users, numUsers);  // Save user information to file before exiting
    cout << endl <<"\t\t\t\t\tGoodbye!" << endl << endl;
    return 0;
}
