#include <string>
#include <iostream>

using namespace std;


// Transaction
// for single transaction (deposit or withdraw action)
class Transaction {

	public:
        int id;
		float amount;
        string type;
        string day;

        Transaction() {}
        Transaction(float amount, string type, string day) {
            this->amount = amount;
            this->type = type;
            this->day = day;
        }
};

// Record
// for keeping track of a user's transactions
class Record {

    private:
    	int index;
        Transaction transaction[10];

    public:
        Record() {
            this->index = 0;
        }
        
        void add_transaction(Transaction transaction) {
            this->transaction[this->index] = transaction;
            this->transaction[this->index].id = this->index + 1;
            this->index++;
        }

        void show_transactions() {
            for (int i = 0; i < index; i++) {
                if (this->transaction[i].id != -1){
                    cout << "Transaction : " << this->transaction[i].id << endl;
                    cout << "Amount : " << this->transaction[i].amount << endl;
                    cout << "Action : " << this->transaction[i].type << endl;
                    cout << "Day : " << this->transaction[i].day << endl;
                    cout << "\n";
                }
            }
        }

        float delete_transaction(int id) {
            float amount;
            if (this->transaction[id-1].type == "deposit")  {
                amount = 0 - this->transaction[id-1].amount;
            } else {
                amount = this->transaction[id-1].amount;
            }
            this->transaction[id-1].id = -1;
            return amount;
        }

        void avg_per_day() {
            string day[10];
            for (int i = 0; i < index; i++) {
                if (this->transaction[i].id != -1){
                    day[i] = this->transaction[i].day;
                }
            }
            for (int i = 0; i < index; i++) {
                for (int j = i+1; j < index; j++) {
                    if (day[i] == day[j]) {
                        day[j] = "-1";
                    }
                }
            }
            cout << "Deposit" << endl;
            for (int i = 0; i < index; i++) {
                if (day[i] != "-1") {
                    float sum = 0;
                    int days = 0;
                    for (int j = 0; j < index; j++) {
                        if ((this->transaction[j].id != -1) && (this->transaction[j].day == day[i]) && (this->transaction[j].type == "deposit")) {
                            sum += this->transaction[j].amount;
                            days++;
                        }
                    }
                    float avg = sum / days;
                    cout << "day " << day[i] << " sum : " << sum << endl;
                    cout << "day " << day[i] << " avg : " << avg << endl;
                }
            }
            cout << "Withdrawal" << endl;
            for (int i = 0; i < index; i++) {
                if (day[i] != "-1") {
                    float sum = 0;
                    int days = 0;
                    for (int j = 0; j < index; j++) {
                        if ((this->transaction[j].id != -1) && (this->transaction[j].day == day[i]) && (this->transaction[j].type == "withdraw")) {
                            sum += this->transaction[j].amount;
                            days++;
                        }
                    }
                    float avg = sum / days;
                    cout << "day " << day[i] << " sum : " << sum << endl;
                    cout << "day " << day[i] << " avg : " << avg << endl;
                }
            }
        }
};

// User class
class User {

    public:
        string name;
        string age;
        float balance;
        Record cash_book;

        User() {}

        User(string name, string age) {
            this->name = name;
            this->age = age;
            this->balance = 0;
            this->cash_book = Record();
        }

        void cash_deposit(float amount, string day) {
            this->balance += amount;
            this->cash_book.add_transaction(Transaction(amount, "deposit", day));
        }

        void cash_withdrawal(float amount, string day) {
            this->balance -= amount;
            this->cash_book.add_transaction(Transaction(amount, "withdraw", day));
        }

        void show_history() {
            this->cash_book.show_transactions();
        }

        void show_history(bool show_name) {
            cout << this->name << endl;
            this->cash_book.show_transactions();
        }

        void delete_transaction(int id) {
            this->balance += this->cash_book.delete_transaction(id);
        }
};

// create 5 preset users
User users[5];

// superuser for administration
class Superuser : public User {

    private:
        string password;
    
    public:
        Superuser() {}
        Superuser(string name, string age, string password) {
            this->name = name;
            this->age = age;
            this->balance = 0;
            this->cash_book = Record();
            this->password = password;
        }

        bool view_all_transactions(string password) {
            if (this->password == password) {
                for (int i = 0; i <= 4; i++) {
                    cout << users[i].name << endl;
                    users[i].show_history(true);
                }
                return true;
            } else {
                cout << "password incorrect" << endl;
                return false;
            }
        }
};

// create a default superuser
Superuser su;

// display ui
class Session {
    
    public:

        string t;
        string input;
        User user;

        Session() {}

        void user_login() {
            string name;
            string age;

            cout << "user login" << endl;
            cout << "name : ";
            cin >> name;

            cout << "age : ";
            cin >> age;

            this->user = User(name, age);

            while (this->input != "x") {
                display();
            }
        }

    private:
        void display() {
            cout << "1 : deposit cash" << endl;
            cout << "2 : withdraw cash" << endl;
            cout << "3 : view balance" << endl;
            cout << "4 : delete transaction" << endl;
            cout << "5 : transaction history" << endl;
            cout << "6 : view all" << endl;
            cout << "x : exit" << endl;
            cin >> this->input;

            if (this->input == "1") {
                deposit_cash_menu();
            } else if (this->input == "2") {
                withdraw_cash_menu();
            } else if (this->input == "3") {
                view_balance();
            } else if (this->input == "4") {
                delete_trans_menu();
            } else if (this->input == "5") {
                show_history();
            } else if (this->input == "6") {
                view_all_menu();
            }
        }

        void view_balance() {
            cout << "Balance : " << endl;
            cout << this->user.balance << endl;
            return;
        }

        void show_history() {
            cout << "History : " << endl;
            this->user.cash_book.show_transactions();
            this->user.cash_book.avg_per_day();
        }

        void deposit_cash_menu() {
            string amount, day;
            cout << "deposit amount : ";
            cin >> amount;
            cout << "day no. (eg. 0, 1, 2, 3, 4 ...) : ";
            cin >> day;
            this->user.cash_deposit(stof(amount), day);
            return;
        }

        void withdraw_cash_menu() {
            string amount, day;
            cout << "withdraw amount : ";
            cin >> amount;
            cout << "day (eg. 0, 1, 2, 3, 4 ...) : ";
            cin >> day;
            this->user.cash_withdrawal(stof(amount), day);
            return;
        }

        void delete_trans_menu() {
            string id;
            cout << "enter transaction id : ";
            cin >> id;
            this->user.delete_transaction(stoi(id));
            return;
        }

        void view_all_menu() {
            string name, password;
            cout << "su name : ";
            cin >> name;
            cout << "su password : ";
            cin >> password;

            if (su.name == name) {
                if (su.view_all_transactions(password)) {
                    cout << this->user.name << endl;
                    this->user.cash_book.show_transactions();
                }
            } else {
                cout << "name incorrect" << endl;
            }
        }
};

void create_preset_data() {
    users[0] = User("Wei Yin", "21");
    users[1] = User("Oscar", "22");
    users[2] = User("Lee", "23");
    users[3] = User("Muthu", "19");
    users[4] = User("Nur", "20");
    su = Superuser("admin", "99", "admin");

    for (int i = 0; i <= 4; i++) {
        users[i].cash_deposit(1000, "0");
        users[i].cash_deposit(2000, "0");
        users[i].cash_deposit(1500, "1");
        users[i].cash_withdrawal(800, "1");
        users[i].cash_withdrawal(600, "1");
    }
}

int main() {

    create_preset_data();
    Session session = Session();
    session.user_login();
    return 0;
}