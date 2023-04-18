#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<direct.h>
#include<conio.h>
#include<limits>

using namespace std;


// UTILITIES
// reading file
string read_file(string path) {
	ifstream file(path);
	string output_string;
	if(file) {
		ostringstream ss;
		ss << file.rdbuf();
		output_string = ss.str();
	}
	return output_string;
}


// reading line
string read_line(string path, int line_number){
	fstream file(path);
	file.seekg(std::ios::beg);
    for(int i=0; i < line_number - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    string output;
	file >> output;
	return output;
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
	if(file_0){
		file << "\n";
	}
	file << content;
	file.close();
	return 0;
}


// rounding currencies
float round(float var)
{
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}


// initialize function (for folders and file initializing purpose)
void initialization() {
    _mkdir("data");
	
	string tid_path = "data/current_tid";
	ifstream file_0(tid_path);
	if (!file_0) {
		write_file(tid_path, "10000000");
	}
}

// define datetime class
class Date {
	public:
		int year;
		int month;
		int date;

		Date(){}
		Date(int year, int month, int date){
			this->year = year;
			this->month = month;
			this->date = date;
		}
};


// define user account class
class User {
	private:
		string password;
	public:
		string username;
		int age;
		double balance;

		// user initialization
		User(string username, int age, string password) {
			this->username = username;
			this->age = age;
			this->password = password;
			this->balance = 0;
			string path = "data/U_" + username;
			append_file(path, this->password);
			append_file(path, to_string(this->age));
			append_file(path, to_string(this->balance));
		}
		/*
		// cash deposit
		void cash_deposit(double amount, Date date) {
			this->balance += amount;
			string current_transaction_id = read_file("data/current_tid");
			Transaction transaction = Transaction(current_transaction_id, 1, amount, date);
		}

		// cash withdrawal
		void cash_withdraw(double amount, Date date) {
			this->balance -= amount;
			string current_transaction_id = read_file("data/current_tid");
			Transaction transaction = Transaction(current_transaction_id, 0, amount, date);
		
		}*/
		
};


// define transaction class
class Transaction {
	public:
		string transaction_id;
		int transaction_type;
		float amount;
		Date date;

		Transaction(int transaction_type, float amount, Date date) {
			string temp1 = read_file("data/current_tid");
			int temp2 = stoi(temp1);
			temp2++;
			this->transaction_id = to_string(temp2);
			this->transaction_type = transaction_type;
			this->amount = amount;
			this->date = date;
			string path = "data/T_" + this->transaction_id;
			append_file(path, this->transaction_id);
			append_file(path, to_string(this->transaction_type));
			append_file(path, to_string(this->amount));
			append_file(path, to_string(this->date.year));
			append_file(path, to_string(this->date.month));
			append_file(path, to_string(this->date.date));

			write_file("data/current_tid", this->transaction_id);
		}
		
};


// menu class
class Menu{
	private:
		void operation_menu(){
			return;
		}

		void user_login_menu(){
			char user_input;
			string username;
			string password;
			string input_password;

			system("cls");
			cout << "User Login Page\n" << endl;
			cout << "Username : ";
			cin >> username;
			string user_path = "data/U_" + username;
			ifstream file(user_path);
			if (file) {
				cout << "Password : ";
				cin >> input_password;
				password = read_line(user_path, 1);
				if (input_password == password){
					cout << "login_successfully" << endl;
					cout << "\nPress any key to continue" << endl;
					cout << ">>";
					getch();
					return;
				} else {
					cout << "Password Incorrect!" << endl;
					cout << "\nPress any key to continue (x to exit)" << endl;
					cout << ">>";
					user_input = getch();
					if (user_input == 'x'){
						return;
					}
					user_login_menu();
				}
			} else {
				cout << "User does not exist!" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">>";
				user_input = getch();
				if (user_input == 'x'){
					return;
				}
				user_login_menu();
			}
			return;
		}

		void create_user_menu(){
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

			string user_path = "data/U_" + username;
			ifstream file(user_path);
			if (file) {
				cout << "Username Unavailable" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">>";
				user_input = getch();
				if (user_input == 'x'){
					return;
				}
				create_user_menu();
			}

			cout << "New Password : ";
			cin >> password;
			cout << "Retype Password : ";
			cin >> confirmation;

			if (password == confirmation){
				cout << "Age : ";
				cin >> tmp;

				bool is_number = true;
				for (int i = 0; i < tmp.length(); i++){
					if (!isdigit(tmp[i])){
						is_number = false;
					}
				}

				if (is_number){
					age = stoi(tmp);
					
					User user = User(username, age, password);

					cout << "\nUser Created Successfully!" << endl;
					cout << "Username : " << username << endl;
					cout << "Age : " << age << endl;
					cout << "Balance : RM 0.00" << endl;
					cout << "\nPress any key to continue" << endl;
					cout << ">>";
					getch();
					return;
				} else {
					cout << "\nPlease Enter Valid Number!" << endl;
					cout << "\nPress any key to continue (x to exit)" << endl;
					cout << ">>";
					user_input = getch();
					if (user_input == 'x'){
						return;
					}
					create_user_menu();
				}
			} else {
				cout << "Password does not match" << endl;
				cout << "\nPress any key to continue (x to exit)" << endl;
				cout << ">>";
				user_input = getch();
				if (user_input == 'x'){
					return;
				}
				create_user_menu();
			}
		}

	public:
		Menu(){}

		void display(){
			while(true){
				char user_input;

				system("cls");
				cout << "Welcome to the Piggy Bank!\n" << endl;
				cout << "How can I help you?" << endl;
				cout << "1      - User Login" << endl;
				cout << "2      - Create User" << endl;
				cout << "Others - Quit\n" << endl;
				cout << ">> ";

				user_input = getch();

				switch(user_input){
					case '1':
						user_login_menu();
						break;
					case '2':
						create_user_menu();
						break;
					default:
						cout << "\n\nThank you and have a nice day..." << endl;
						return;
				}
			}
		}
};


// main function
int main() {
	initialization();

	Menu menu = Menu();
	menu.display();
	return 0;
}
