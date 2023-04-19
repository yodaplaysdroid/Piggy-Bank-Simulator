#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <conio.h>
#include <limits>
#include <vector>
#include <cstdio>

using namespace std;

class Transaction;
class User;
class Date;

string read_file(string path);
string read_line(string path, int line_number);
int write_line(string path, int line_number, string input);
int write_file(string path, string content);
int append_file(string path, string content);
double round(double var);
double to_rm(string user_input);


// UTILITIES
// reading file
string read_file(string path) {
	ifstream file(path);
	string output_string;
	if (file) {
		ostringstream ss;
		ss << file.rdbuf();
		output_string = ss.str();
	}
	file.close();
	return output_string;
}


// reading line
string read_line(string path, int line_number) {
	fstream file(path);
	file.seekg(std::ios::beg);
    for (int i=0; i < line_number - 1; ++i) {
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    string output;
	file >> output;
	file.close();
	return output;
}


// writing line
int write_line(string path, int line_number, string input) {
    ifstream file(path);

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    lines[line_number - 1] = input;

    file.close();

    ofstream outfile(path);

    for (const auto& l : lines) {
        outfile << l << endl;
    }

    outfile.close();
    return 0;
}


// writing file
int write_file(string path, string content) {
	ofstream file(path);

	file << content;
	file.close();
	return 0;
}


// appending file
int append_file(string path, string content) {
	ifstream file_0(path);
	ofstream file;

	file.open(path.c_str(), std::ios_base::app);
	if (file_0) {
		file << "\n";
	}
	file << content;
	file_0.close();
	file.close();
	return 0;
}


// rounding currencies
double round(double var) {
    double value = (long)(var * 100 + .5);
    return (double)value / 100;
}


// checking whether input currency is valid
double to_rm(string user_input) {
	bool is_number = true;
	int dot_counter = 0;
	for (int i = 0; i < user_input.length(); i++) {
		if (!isdigit(user_input[i])) {
			if (user_input[i] == '.') {
				dot_counter++;
			} else {
				is_number = false;
			}
		}
	}
	if ((dot_counter <= 1) && (is_number)) {
		return round(stod(user_input));
	} else {
		return -1;
	}
}


// is number
bool is_number(string var) {
	for (int i = 0; i < var.length(); i++) {
		if (!isdigit(var[i])) {
			return false;
		}
	}
	return true;
}


// define datetime class
class Date {
	public:
		int year;
		int month;
		int day;

		Date() {}
		Date(int year, int month, int day) {
			this->year = year;
			this->month = month;
			this->day = day;
		}

		bool set_date(string input_year, string input_month, string input_day) {
			int year, month, day;
			if (is_number(input_year) && is_number(input_month) && is_number(input_day)) {
				year = stoi(input_year);
				month = stoi(input_month);
				day = stoi(input_day);
				
				if ((month > 12) || (month < 1)) {
					return false;
				}
				if ((month == 4 ) || (month == 6 ) || (month == 9 ) || (month == 11 )) {
					if ((day <= 30) && (day >= 1)) {
						this->year = year;
						this->month = month;
						this->day = day;
						return true;
					}
				} else if (month == 2) {
					if ((year % 4) == 0) {
						if ((day <= 29) && (day >= 1)) {
							this->year = year;
							this->month = month;
							this->day = day;
							return true;
						}
					} else {
						if ((day <= 28) && (day >= 1)) {
							this->year = year;
							this->month = month;
							this->day = day;
							return true;
						}
					}
				} else {
					if ((day <= 31) && (day >= 1)) {
						this->year = year;
						this->month = month;
						this->day = day;
						return true;
					}
				}
			}
			return false;
		}

		void datestamp() {
			cout << "(";
			cout << year;
			cout << "/";
			cout << month;
			cout << "/";
			cout << day;
			cout << ")";
		}
};


// define transaction class
class Transaction {
	public:
		string transaction_type;
		double amount;
		Date date;
		string detail;

		Transaction() {}
		Transaction(string transaction_type, double amount, Date date) {
			this->transaction_type = transaction_type;
			this->amount = amount;
			this->date = date;
			string date_s = to_string(date.year) + "/" + to_string(date.month) + "/" + to_string(date.day);
			this->detail = "\nDate : " + date_s + "\nTransaction Type : " + this->transaction_type + "\nAmount : " + to_string(this->amount) + "\n";
		}
};


// define user account class
class User {
	public:
		string username;
		int age;
		string password;
		double balance;
		string path;
		string transaction_path;

		User() {}
		// user initialization
		User(string username, int age, string password) {
			this->username = username;
			this->age = age;
			this->password = password;
			this->balance = 0;
			this->path = "data/U_" + username;
			this->transaction_path = "data/T_" + username;

			append_file(this->path, this->password);
			append_file(this->path, to_string(this->age));
			append_file(this->path, to_string(this->balance));
			write_file(this->transaction_path, "Transactions\n");
		}

		double cash_deposit(double amount, Date date) {
			this->balance += amount;
			write_line(this->path, 3, to_string(this->balance));
			Transaction t = Transaction("deposit", amount, date);
			cout << t.detail;
			append_file(transaction_path, t.detail);
			return this->balance;
		}

		double cash_withdraw(double amount, Date date) {
			this->balance -= amount;
			write_line(this->path, 3, to_string(this->balance));
			Transaction t = Transaction("withdraw", amount, date);
			append_file(transaction_path, t.detail);
			return this->balance;
		}

		void show_transactions() {
			cout << read_file(this->transaction_path);
		}
};


// menu class
class Menu {
	private:
		User current_user;
		Date date;
		
		bool get_date() {
			string year, month, day;
			char user_input;

			while (true) {
				cout << "\nEnter Date\nYear  : ";
				cin >> year;
				cout << "Month : ";
				cin >> month;
				cout << "Day   : ";
				cin >> day;
				if (!date.set_date(year, month, day)) {
					cout << "\nPlease enter valid date!" << endl;
					cout << "\nPress any key to continue (x to exit)" << endl;
					cout << ">> ";
					user_input = getch();
					cout << "\n";
					if (user_input == 'x') {
						return false;
					}
				} else {
					return true;
				}
			}
		}
		void change_username() {
			char user_input;
			string new_username;
			cout << "\n\nNew Username : ";
			cin >> new_username;

			string new_user_path = "data/U_" + new_username;
			ifstream file(new_user_path);
			if (file) {
				file.close();
				cout << "Username Unavailable" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				change_username();
			} else {
				file.close();
				User new_user = User(new_username, current_user.age, current_user.password);
				new_user.balance = current_user.balance;
				write_file(new_user.transaction_path, read_file(current_user.transaction_path));

				string path = current_user.path;
				std::remove(path.c_str());
				path = current_user.transaction_path;
				std::remove(path.c_str());

				current_user = new_user;

				cout << "\nUsername Changed Successfully!" << endl;
				cout << "\nPress any key to continue..." << endl;
				cout << ">> ";
				getch();
				return;
			}
		}

		void edit_age() {
			string temp;
			int new_age;
			char user_input;

			cout << "\n\nNew Age : ";
			cin >> temp;

			if (is_number(temp)) {
				new_age = stoi(temp);
				write_line(current_user.path, 2, temp);
				current_user.age = new_age;

				cout << "\nAge Changed Successfully!" << endl;
				cout << "\nPress any key to continue..." << endl;
				cout << ">> ";
				getch();
				return;
			} else {
				cout << "\nPlease Enter Valid Age!" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				edit_age();
			}
		}

		void change_password() {
			string old_password;
			string new_password;
			string new_password_confirmation;
			char user_input;

			cout << "\n\nOld Password : ";
			cin >> old_password;
			if (old_password == current_user.password) {
				cout << "New Password : ";
				cin >> new_password;
				cout << "Retype New Password : ";
				cin >> new_password_confirmation;
				
				if (new_password == new_password_confirmation) {
					current_user.password = new_password;
					write_line(current_user.path, 1, new_password);
					
					cout << "\nPassword Changed Successfully!" << endl;
					cout << "\nPress any key to continue..." << endl;
					cout << ">> ";
					getch();
					return;
				} else {
					cout << "\nPassword Not Matching!" << endl;
					cout << "\nPress any key to continue (x to exit)" << endl;
					cout << ">> ";
					user_input = getch();
					if (user_input == 'x') {
						return;
					}
					change_password();
				}
			} else {
				cout << "\nPassword Incorrect!" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				change_password();
			}
		}

		void edit_account() {
			char user_input;

			while (true) {
				system("cls");
				cout << "Edit Account Menu\n" << endl;
				cout << "1 - Edit Username" << endl;
				cout << "2 - Edit Age" << endl;
				cout << "3 - Change Password" << endl;
				cout << "x - Return\n" << endl;
				cout << ">> ";
				user_input = getch();

				switch (user_input) {
					case '1':
						change_username();
						return;
					case '2':
						edit_age();
						return;
					case '3':
						change_password();
						return;
					case 'x':
						return;
					default:
						break;
				}
			}
		}

		void cash_deposit_menu() {
			char user_input;
			string temp;
			double amount;

			system("cls");
			cout << "Cash Deposit Menu\n" << endl;
			cout << "Enter Amount : ";
			cin >> temp;
			amount = to_rm(temp);
			if (amount == -1) {
				cout << "Please enter valid amount" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				cash_deposit_menu();
			} else {
				current_user.cash_deposit(amount, date);
				printf("\nDeposited  : RM %.2f", amount);
				printf("\nNew Amount : RM %.2f\n\n", current_user.balance);
				cout << "\nPress any key to continue..." << endl;
				cout << ">> ";
				getch();
			}
			return;
		}

		void cash_withdrawal_menu() {
			char user_input;
			string temp;
			double amount, new_amount;

			system("cls");
			cout << "Cash Withdrawal Menu\n" << endl;
			cout << "Enter Amount : ";
			cin >> temp;
			amount = to_rm(temp);
			if (amount == -1) {
				cout << "Please enter valid amount" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				cash_withdrawal_menu();
			} else {
				new_amount = current_user.balance - amount;
				if (new_amount < 0) {
					cout << "Insufficient Balance!" << endl;
					cout << "\nPress any key to continue (x to exit)" << endl;
					cout << ">> ";
					user_input = getch();
					if (user_input == 'x') {
						return;
					}
					cash_withdrawal_menu();
				} else {
					current_user.cash_withdraw(amount, date);
					printf("\nWithdrawed : RM %.2f", amount);
					printf("\nNew Amount : RM %.2f\n\n", new_amount);
					cout << "\nPress any key to continue..." << endl;
					cout << ">> ";
					getch();
				}
			}
			return;
		}

		void account_display() {
			char user_input;

			while (true) {
				system("cls");
				cout << "My Account\n" << endl;
				cout << "Username : " << current_user.username << endl;
				cout << "Age      : " << current_user.age << endl;
				printf("Balance  : RM %.2f\n\n", current_user.balance);
				cout << "1 - Edit Account Credentials" << endl;
				cout << "2 - Transaction History" << endl;
				cout << "x - Return\n" << endl;
				cout << ">> ";
				user_input = getch();

				switch (user_input) {
					case '1':
						edit_account();
						break;
					case '2':
						system("cls");
						current_user.show_transactions();
						cout << "\n\nPress any key to continue..." << endl;
						cout << ">> ";
						getch();
						break;
					case 'x':
						return;
					default:
						break;
				}
			}
		}

		void operation_menu() {
			char user_input;
			string delete_confirmation;
			string password;

			while (true) {
				system("cls");
				cout << "Operation Menu" << endl;
				date.datestamp();
				cout << "\n\n1 - Cash Deposit" << endl;
				cout << "2 - Cash Withdrawal" << endl;
				cout << "3 - My Account" << endl;
				cout << "4 - Delete Account" << endl;
				cout << "x - Logout\n" << endl;
				cout << ">> ";
				user_input = getch();

				switch (user_input) {
					case '1':
						cash_deposit_menu();
						break;
					case '2':
						cash_withdrawal_menu();
						break;
					case '3':
						account_display();
						break;
					case '4':
						cout << "\n\nEnter Password : ";
						cin >> password;
						if (password != current_user.password) {
							cout << "\nPassword Incorrect" << endl;
							cout << "Operation Aborted" << endl;
							cout << "\nPress any key to continue..." << endl;
							cout << ">> ";
							getch();
							date = Date();
							current_user = User();
							return;
						}
						cout << "\n\nWarning : Account Delete Confirmation" << endl;
						cout << "Type 'confirm'" << endl;
						cout << ">> ";
						cin >> delete_confirmation;
						if (delete_confirmation == "confirm") {
							std::remove(current_user.path.c_str());
							std::remove(current_user.transaction_path.c_str());

							cout << "\nRemoved Successfully!" << endl;
							cout << "\nPress any key to continue..." << endl;
							cout << ">> ";
							getch();
						} else {
							cout << "Operation Aborted" << endl;
							cout << "\nPress any key to continue..." << endl;
							cout << ">> ";
							getch();
						}

						date = Date();
						current_user = User();
						return;
					case 'x':
						date = Date();
						current_user = User();
						return;
					default:
						break;
				}
			}
		}

		void user_login_menu() {
			char user_input;
			string username;
			string password;
			string input_password;
			string year, month, day;

			system("cls");
			cout << "User Login Page\n" << endl;
			cout << "Username : ";
			cin >> username;
			if (username == "x") {
				return;
			}
			string user_path = "data/U_" + username;
			ifstream file(user_path);
			if (file) {
				file.close();
				cout << "Password : ";
				cin >> input_password;
				password = read_line(user_path, 1);
				if (input_password == password) {
					if (get_date()) {
						current_user.username = username;
						current_user.password = password;
						current_user.age = stoi(read_line(user_path, 2));
						current_user.balance = stod(read_line(user_path, 3));
						current_user.path = "data/U_" + username;
						current_user.transaction_path = "data/T_" + username;
						operation_menu();
					}
				} else {
					cout << "Password Incorrect!" << endl;
					cout << "\nPress any key to continue (x to exit)" << endl;
					cout << ">> ";
					user_input = getch();
					if (user_input == 'x') {
						return;
					}
					user_login_menu();
				}
			} else {
				file.close();
				cout << "User does not exist!" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				user_login_menu();
			}
			return;
		}

		void create_user_menu() {
			char user_input;
			string username;
			string password;
			string confirmation;
			int age;
			string tmp;

			system("cls");
			cout << "Create User Page\n" << endl;

			cout << "New Username (Username must be unique) : ";
			cin >> username;
			if (username == "x") {
				return;
			}

			string user_path = "data/U_" + username;
			ifstream file(user_path);
			if (file) {
				cout << "Username Unavailable" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				create_user_menu();
			}

			cout << "New Password : ";
			cin >> password;
			cout << "Retype Password : ";
			cin >> confirmation;

			if (password == confirmation) {
				cout << "Age : ";
				cin >> tmp;

				if (is_number(tmp)) {
					age = stoi(tmp);
					
					User user = User(username, age, password);

					cout << "\nUser Created Successfully!" << endl;
					cout << "Username : " << username << endl;
					cout << "Age : " << age << endl;
					cout << "Balance : RM 0.00" << endl;
					cout << "\nPress any key to continue..." << endl;
					cout << ">> ";
					getch();
					return;
				} else {
					cout << "\nPlease Enter Valid Age!" << endl;
					cout << "\nPress any key to continue (x to exit)" << endl;
					cout << ">> ";
					user_input = getch();
					if (user_input == 'x') {
						return;
					}
					create_user_menu();
				}
			} else {
				cout << "Password does not match" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">> ";
				user_input = getch();
				if (user_input == 'x') {
					return;
				}
				create_user_menu();
			}
		}

	public:
		Menu() {}

		void display() {
			while (true) {
				char user_input;

				system("cls");
				cout << "Welcome to the Piggy Bank!\n" << endl;
				cout << "How can I help you?" << endl;
				cout << "1 - User Login" << endl;
				cout << "2 - Create User" << endl;
				cout << "x - Quit\n" << endl;
				cout << ">> ";

				user_input = getch();

				switch (user_input) {
					case '1':
						user_login_menu();
						break;
					case '2':
						create_user_menu();
						break;
					case 'x':
						cout << "\n\nThank you and have a nice day...\n" << endl;
						return;
					default:
						break;
				}
			}
		}
};


// main function
int main() {
	_mkdir("data");

	Menu menu = Menu();
	menu.display();
	return 0;
}
