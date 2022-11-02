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

// encrypts hexadecimal vector into hexadecimal cipher vector
vector<int> cipher_encrypt(const vector<int> &hex_input, const vector<vector<int>> &keys);

// decrypts hexadecimal cipher vector into hexadecimal vector
vector<int> cipher_decrypt(const vector<int> &cipher_input, const vector<vector<int>> &keys);

// returns a plaintext string
string translate(const vector<int> &hex_input);

// returns a hex string
string str2hex(const vector<int> &hex_input);

#endif