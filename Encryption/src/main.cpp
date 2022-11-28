#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "headers/entry.h"
#include "headers/encryption.h"

using std::cout;
using std::endl;

vector<Entry> getItems() {
    // read csv, convert into entries, and add to a vector
    std::ifstream inputFile;
    inputFile.open("./src/database.csv"); //"/Users/JRome/Desktop/AES-Vault/Encryption/src/database.csv"

    vector<Entry> entryList{};
    string line = "";
    int i {0};

    while (getline(inputFile, line)) {
        string organization;
        string email;
        string username;
        string password;

        std::stringstream inputString(line);
        getline(inputString, organization, ',');
        getline(inputString, email, ',');
        getline(inputString, username, ',');
        getline(inputString, password, ',');

        Entry item{ i, organization, email, username, password };
        entryList.push_back(item);
        i++;
    }
    
    return entryList;
}



int main() {

    // key
    string key = "ExpectResistance";
    vector<vector<int>> keys = key_gen(text2hexa(key));


    // test entry
    Entry google{ 1, "Amazon", "JeffreyBezos@gmail.com", "Jeffrey", "Password123"};

    google.secret(keys);
    google.print();

    google.notSecret(keys);
    google.print();



    //google.copy_email(keys);
    //google.copy_username(keys);
    //google.copy_password(keys);

    // add entry to csv and vector


    // delete entries to csv and vector


    return 0;
}