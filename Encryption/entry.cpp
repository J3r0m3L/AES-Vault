#include <iostream>
#include <string>
#include <vector>
#include "encryption.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;


class Entry {
private:
    string organization;
    string email;
    string username;
    string password;
public:
    Entry(string org, string em, string user, string passwd, vector<vector<int>> keys);
    
    // later change to copy to clipboard
    void copy_email(vector<vector<int>> keys);
    void copy_username(vector<vector<int>> keys);
    void copy_password(vector<vector<int>> keys);
    
    ~Entry();
};

// Constructor
Entry::Entry(string org, string em, string user, string passwd, vector<vector<int>> keys)
    :   organization{org}, email{encrypt(em, keys)}, username{encrypt(user, keys)}, password{encrypt(passwd, keys)} {
};

// Methods
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