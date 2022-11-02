#ifndef encryption_H
#define encryption_H
#include <string>
#include <vector>

using std::string;
using std::vector;

// converts string to a hexadecimal vector
vector<int> str2hex(const string &str);

// generates 11 round keys
vector<vector<int>> key_gen(const vector<int> &key);

// combines the all other functions to fully encrypt text
string encrypt(const string &input, const vector<vector<int>> &keys);

// combines the all other functions to fully decrypt text
string decrypt(const string &input, const vector<vector<int>> &keys);
#endif