#include <iostream>
#include "entry.h"
#include "encryption.h"



int main() {
    // Key
    string key = "ExpectResistance";
    vector<vector<int>> keys = key_gen(text2hexa(key));
    
    // Generate Entry
    Entry google {"Amazon", "JeffreyBezos@gmail.com", "Jeffrey", "Password123", keys};
    google.copy_email(keys);
    google.copy_username(keys);
    google.copy_password(keys);
    
    return 0;
}