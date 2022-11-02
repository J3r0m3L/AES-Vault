#include <iostream>
#include "entry.h"
#include "encryption.h"


using std::cout;

int temp() {
    // Key
    string key = "ExpectResistance";
    vector<vector<int>> keys = key_gen(str2hex(key));
    
    // Generate Entry
    Entry google {"Amazon", "JeffreyBezos@gmail.com", "Jeffrey", "Password123", keys};
    
    return 0;
}