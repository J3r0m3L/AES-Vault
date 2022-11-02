#include <iostream>
#include <fstream>
#include "entry.h"
#include "encryption.h"



int main() {
    // Read Database and Convert Values into Entries in a Vector
    //fstream fin;
    //fin.open("database.csv", ios::in);
    
    // Key
    string key = "ExpectResistance";
    vector<vector<int>> keys = key_gen(text2hexa(key));
    
    // Generate Entry
    Entry google {1, "Amazon", "JeffreyBezos@gmail.com", "Jeffrey", "Password123", keys, false};
    google.copy_email(keys);
    google.copy_username(keys);
    google.copy_password(keys);
    
    // Add Entry to Text File and to Entries Vector
    
    
    
    // Delete Entry from Text File and Entries Vector
    
    return 0;
}