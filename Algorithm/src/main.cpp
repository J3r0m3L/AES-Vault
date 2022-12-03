#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "headers/entry.h"
#include "headers/encryption.h"
#include "headers/hash.h"
#include "headers/store.h";

using std::cout;
using std::endl;

int main() {

    // key
    string key = "ExpectResistance";
    vector<vector<int>> keys = key_gen(text2hexa(key));


    // test entry
    Entry google{ 1, "Amazon", "JeffreyBezos@gmail.com", "Jeffrey", "Password123"};
    Entry meta{ 1, "Meta", "Mark@gmail.com", "Mark", "Zuckerberg" };

    google.secret(keys);
    google.print();
    cout << endl;

    google.notSecret(keys);
    google.print();
    cout << endl;
    /*
    createDB();
    createTables();
    //google.store();
    //meta.store();
    vector<vector<string>> storedData = selectData();
    for (int i = 0; i < storedData.at(0).size(); i++) {
        for (int j = 0; j < storedData.size(); j++) {
            cout << storedData.at(j).at(i) << " ";
        }
        cout << endl;
    }
    //insertUser("Lammmmmee");
    //cout << selectUser(1) << endl;
    cout << "wowowowowwowowo" << endl;
    //deleteAll();
    deleteData(14);
    /*
    storedData = selectData();
    for (int i = 0; i < storedData.at(0).size(); i++) {
        for (int j = 0; j < storedData.size(); j++) {
            cout << storedData.at(j).at(i) << " ";
        }
        cout << endl;
    }
    */








    return 0;
}