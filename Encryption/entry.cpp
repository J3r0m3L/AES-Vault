#include <string>
#include <vector>
#include "encryption.h"

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
    
    ~Entry();
};

// Constructor
Entry::Entry(string org, string em, string user, string passwd, vector<vector<int>> keys)
    :   organization{org}, 
        email{em}, 
        username{user}, 
        password{passwd} {
}

// Deconstructor
Entry::~Entry() {};