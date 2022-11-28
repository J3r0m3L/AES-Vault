#include <iostream>
#include <string>
#include <vector>
#include "headers/encryption.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;


class Entry {
private:
    int index;
    string organization;
    string email;
    string username;
    string password;
public:
    Entry(int id, string org, string em, string user, string passwd);

    void store();
    void remove();
    // tmp;
    void secret(vector<vector<int>> keys);
    void notSecret(vector<vector<int>> keys);
    void print();


    void copy_email(vector<vector<int>> keys);
    void copy_username(vector<vector<int>> keys);
    void copy_password(vector<vector<int>> keys);

    ~Entry();
};

// Constructor
Entry::Entry(int id, string org, string em, string user, string passwd)
    : index{ id },
    organization{ org },
    email{ em },
    username{ user },
    password{ passwd } {
};

// Methods
void Entry::store() {
    cout << "Placeholder";
}

void Entry::remove() {
    cout << "Placeholder";
}

//tmp
void Entry::secret(vector<vector<int>> keys) {
    email = encrypt(email, keys);
    username = encrypt(username, keys);
    password = encrypt(password, keys);
}
void Entry::notSecret(vector<vector<int>> keys) {
    email = decrypt(email, keys);
    username = decrypt(username, keys);
    password = decrypt(password, keys);
}
void Entry::print() {
    cout << organization << endl;
    cout << email << endl;
    cout << username << endl;
    cout << password << endl;
}






void Entry::copy_email(vector<vector<int>> keys) {
    string plaintext = decrypt(email, keys);
    cout << plaintext << endl;
};
void Entry::copy_username(vector<vector<int>> keys) {
    string plaintext = decrypt(username, keys);
    cout << plaintext << endl;
};
void Entry::copy_password(vector<vector<int>> keys) {
    string plaintext = decrypt(password, keys);
    cout << plaintext << endl;
}




// Deconstructor
Entry::~Entry() {};