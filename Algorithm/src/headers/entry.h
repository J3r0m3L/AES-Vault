#pragma once
#define entry_H
#include <string>
#include <vector>

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
    // temp just to test out the AES algo
    void secret(vector<vector<int>> keys);
    void notSecret(vector<vector<int>> keys);
    void print();


    void copy_email(vector<vector<int>> keys);
    void copy_username(vector<vector<int>> keys);
    void copy_password(vector<vector<int>> keys);

    ~Entry();
};
