#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<direct.h>

using namespace std;


// FILE OPERATIONS
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


// initialize function (for folders and file initializing purpose)
void initialization() {
    _mkdir("data");
	
	string tid_path = "data/current_tid";
	ifstream file_0(tid_path);
	if (!file_0) {
		write_file(tid_path, "10000000");
	}

	string uid_path = "data/current_uid";
	ifstream file_1(uid_path);
	if (!file_1) {
		write_file(uid_path, "1000");
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
		string user_id;
		string password;
	public:
		string name;
		int age;
		double balance;

		// user initialization
		User(string name, int age, string password) {
			this->name = name;
			this->age = age;
			this->password = password;
			/*
			string user_id = read_file("data/user_id");
			int temp = stoi(user_id);
			temp++;
			user_id = std::to_string(temp);
			this->user_id = user_id;
		}

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
		*/
		}
		
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


// main function
int main() {
	initialization();

	Date date = Date(2020, 04, 01);
	Transaction transaction = Transaction(1, 100, date);
	return 0;
}
