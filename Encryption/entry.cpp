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
    int index;
    string organization;
    string email;
    string username;
    string password;
public:
    Entry(int id, string org, string em, string user, string passwd, vector<vector<int>> keys, bool encrypted);
    
    void store();
    void remove();
    void copy_email(vector<vector<int>> keys);
    void copy_username(vector<vector<int>> keys);
    void copy_password(vector<vector<int>> keys);
    
    ~Entry();
};

// Constructor
Entry::Entry(int id, string org, string em, string user, string passwd, vector<vector<int>> keys, bool encrypted)
    :   index {id},
        organization{org}, 
        email{(encrypted) ? em : encrypt(em, keys)},
        username{(encrypted) ? em : encrypt(user, keys)}, 
        password{(encrypted) ? em : encrypt(passwd, keys)} {
};

// Methods
void Entry::store() {
    cout << "Placeholder";
}

void Entry::remove() {
    cout << "Placeholder";
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