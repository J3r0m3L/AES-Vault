#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "headers/entry.h"
#include "headers/encryption.h"

using std::cout;
using std::endl;

int main() {

    // key
    string key = "ExpectResistance";
    vector<vector<int>> keys = key_gen(text2hexa(key));


    // test entry
    Entry google{ 1, "Amazon", "JeffreyBezos@gmail.com", "Jeffrey", "Password123"};

    google.secret(keys);
    google.print();

    cout << endl;
    google.notSecret(keys);
    google.print();


    return 0;
}