#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using std::cout;
using std::endl;
using std::vector;
using std::string;

// RCON table
const int rcon[10] {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

// hex to decimal conversion table
const std::map<char, int> hex2dec  {{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
                                    {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
                                    {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, 
                                    {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}};

// column mix matrix
const int mixmatrix[4][4][2]   {{{1, 0}, {1, 1}, {0, 1}, {0, 1}},
                                {{0, 1}, {1, 0}, {1, 1}, {0, 1}},
                                {{0, 1}, {0, 1}, {1, 0}, {1, 1}},
                                {{1, 1}, {0, 1}, {0, 1}, {1, 0}}};
                                
// column reverse mix matrix
const int revmixmatrix[4][4][4]{{{1, 1, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 1}, {1, 0, 0, 1}},
                                {{1, 0, 0, 1}, {1, 1, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 1}},
                                {{1, 1, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 0}, {1, 0, 1, 1}},
                                {{1, 0, 1, 1}, {1, 1, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 0}}};

// forward-sbox
const int sbox[16][16] {{0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76}, 
                        {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
                        {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
                        {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
                        {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
                        {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
                        {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
                        {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
                        {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
                        {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
                        {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
                        {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
                        {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
                        {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
                        {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
                        {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}};

// inverse-sbox
const int invsbox[16][16] {{0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
                        {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb},
                        {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e},
                        {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25},
                        {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92},
                        {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84},
                        {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06},
                        {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b},
                        {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73},
                        {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e},
                        {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b},
                        {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4},
                        {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f},
                        {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef},
                        {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61},
                        {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d}};

// ASCII conversion chart
const std::map<int, char> printable_chars  {{0x20, ' '}, {0x21, '!'}, {0x22, '"'}, {0x23, '#'}, {0x24, '$'}, {0x25, '%'},
                                            {0x26, '&'}, {0x27, '\''}, {0x28, '('}, {0x29, ')'}, {0x2a, '*'}, {0x2b, '+'},
                                            {0x2c, ','}, {0x2d, '-'}, {0x2e, '.'}, {0x2f, '/'}, {0x30, '0'}, {0x31, '1'},
                                            {0x32, '2'}, {0x33, '3'}, {0x34, '4'}, {0x35, '5'}, {0x36, '6'}, {0x37, '7'},
                                            {0x38, '8'}, {0x39, '9'}, {0x3a, ':'}, {0x3b, ';'}, {0x3c, '<'}, {0x3d, '='},
                                            {0x3e, '>'}, {0x3f, '?'}, {0x40, '@'}, {0x41, 'A'}, {0x42, 'B'}, {0x43, 'C'},
                                            {0x44, 'D'}, {0x45, 'E'}, {0x46, 'F'}, {0x47, 'G'}, {0x48, 'H'}, {0x49, 'I'},
                                            {0x4a, 'J'}, {0x4b, 'K'}, {0x4c, 'L'}, {0x4d, 'M'}, {0x4e, 'N'}, {0x4f, 'O'},
                                            {0x50, 'P'}, {0x51, 'Q'}, {0x52, 'R'}, {0x53, 'S'}, {0x54, 'T'}, {0x55, 'U'},
                                            {0x56, 'V'}, {0x57, 'W'}, {0x58, 'X'}, {0x59, 'Y'}, {0x5a, 'Z'}, {0x5b, '['},
                                            {0x5c, '\\'}, {0x5d, ']'}, {0x5e, '^'}, {0x5f, '_'}, {0x60, '`'}, {0x61, 'a'},
                                            {0x62, 'b'}, {0x63, 'c'}, {0x64, 'd'}, {0x65, 'e'}, {0x66, 'f'}, {0x67, 'g'},
                                            {0x68, 'h'}, {0x69, 'i'}, {0x6a, 'j'}, {0x6b, 'k'}, {0x6c, 'l'}, {0x6d, 'm'},
                                            {0x6e, 'n'}, {0x6f, 'o'}, {0x70, 'p'}, {0x71, 'q'}, {0x72, 'r'}, {0x73, 's'},
                                            {0x74, 't'}, {0x75, 'u'}, {0x76, 'v'}, {0x77, 'w'}, {0x78, 'x'}, {0x79, 'y'},
                                            {0x7a, 'z'}, {0x7b, '{'}, {0x7c, '|'}, {0x7d, '}'}, {0x7e, '~'}};


// returns a hexadecimal vector
vector<int> str2hex(const string &str) {
    vector<int> hex (0);
    for (auto &ch: str)
        hex.push_back(printable_chars.at(ch));
    return hex;
}

// returns all generated keys (rotate + s-box + xor)
vector<vector<int>> key_gen(const vector<int> &key) {
    
    // all generated keys
    vector<vector<int>> keys(11, key);
    
    // initialize column vectors
    vector<int> col1 (4);
    vector<int> col2 (4);
    vector<int> col3 (4);
    vector<int> col4 (4);
    vector<int> tmp (4);
    
    for (int i = 0; i < 10; i++) {
    
        // split into columns
        col1 = vector<int>(keys.at(i).begin(), keys.at(i).begin() + 4);
        col2 = vector<int>(keys.at(i).begin() + 4, keys.at(i).begin() + 8);
        col3 = vector<int>(keys.at(i).begin() + 8, keys.at(i).begin() + 12);
        col4 = vector<int>(keys.at(i).begin() + 12, keys.at(i).end());
        
        // rotate
        tmp = vector<int>(col4.begin() + 1, col4.end());
        tmp.push_back(col4.at(0));
        
        // s-box substitution
        for (int j = 0; j < 4; j++) {
            std::stringstream ss;
            ss << std::hex << tmp.at(j);
            string hex_num(ss.str());
            if (hex_num.length() == 1)
                hex_num.insert(0, "0");
            tmp.at(j) = sbox[hex2dec.at(hex_num.at(0))][hex2dec.at(hex_num.at(1))];
        }

        // xor operation
        bool toggle {true};
        for (int j= 0; j < 4; j++) {
            if (toggle == true) {
                col1.at(j) = col1.at(j) ^ tmp.at(j) ^ rcon[i];
                toggle = false;
            } else {
                col1.at(j) = col1.at(j) ^ tmp.at(j);
            }
            col2.at(j) = col2.at(j) ^ col1.at(j);
            col3.at(j) = col3.at(j) ^ col2.at(j);
            col4.at(j) = col4.at(j) ^ col3.at(j);
            //cout << std::hex << col1.at(j) << " ";
            //cout << std::hex << col2.at(j) << " ";
            //cout << std::hex << col3.at(j) << " ";
            //cout << std::hex << col4.at(j) << endl;
        }
        //cout << endl;
        
        // add to keys
        for (int j = 0; j < 4; j++) {
            keys.at(i + 1).at(j) = col1.at(j);
            keys.at(i + 1).at(j + 4) = col2.at(j);
            keys.at(i + 1).at(j + 8) = col3.at(j);
            keys.at(i + 1).at(j + 12) = col4.at(j);
        }
    }
    
    return keys;
}

// returns cipher text
vector<int> cipher_encrypt(const vector<int> &hex_input, const vector<vector<int>> &keys) {
    
    vector<int> cipher (16);
    
    // initialize column vectors, row vectors, and tmp variables
    vector<int> row2 (4);
    vector<int> row3 (4);
    vector<int> row4 (4);
    vector<int> col1 (4);
    vector<int> col2 (4);
    vector<int> col3 (4);
    vector<int> col4 (4);
    int tmp1 {};
    int tmp2 {};
    vector<int> tmp3 (4);
    
    // --initial round--
    for (int j = 0; j < 16; j++)
        cipher.at(j) = hex_input.at(j) ^ keys.at(0).at(j);
    
    // --main rounds--
    for (int i = 1; i < 10; i++) {
        
        // s-box substition
        for (int j = 0; j < 16; j++) {
            std::stringstream ss;
            ss << std::hex << cipher.at(j);
            string hex_num(ss.str());
            if (hex_num.length() == 1)
                hex_num.insert(0, "0");
            cipher.at(j) = sbox[hex2dec.at(hex_num.at(0))][hex2dec.at(hex_num.at(1))];
        }
        
        // shift rows
        row2 = {cipher.at(5), cipher.at(9), cipher.at(13), cipher.at(1)};
        row3 = {cipher.at(10), cipher.at(14), cipher.at(2), cipher.at(6)};
        row4 = {cipher.at(15), cipher.at(3), cipher.at(7), cipher.at(11)};
    
        for (int j = 0; j < 4; j++) {
            cipher.at(1 + 4 * j) = row2.at(j);
            cipher.at(2 + 4 * j) = row3.at(j);
            cipher.at(3 + 4 * j) = row4.at(j);
        }
        
        // mix columns
        col1 = vector<int>(cipher.begin(), cipher.begin() + 4);
        col2 = vector<int>(cipher.begin() + 4, cipher.begin() + 8);
        col3 = vector<int>(cipher.begin() + 8, cipher.begin() + 12);
        col4 = vector<int>(cipher.begin() + 12, cipher.end());
        
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                tmp1 = (mixmatrix[j][k][0] == 1) ? col1.at(k) << 1 : 0;
                tmp2 = (mixmatrix[j][k][1] == 1) ? col1.at(k) : 0;
                tmp3.at(k) = tmp1 ^ tmp2;
            }
            cipher.at(j) = tmp3.at(0) ^ tmp3.at(1) ^ tmp3.at(2) ^ tmp3.at(3);
            
            for (int k = 0; k < 4; k++) {
                tmp1 = (mixmatrix[j][k][0] == 1) ? col2.at(k) << 1 : 0;
                tmp2 = (mixmatrix[j][k][1] == 1) ? col2.at(k) : 0;
                tmp3.at(k) = tmp1 ^ tmp2;
            }
            cipher.at(j + 4) = tmp3.at(0) ^ tmp3.at(1) ^ tmp3.at(2) ^ tmp3.at(3);
            
            for (int k = 0; k < 4; k++) {
                tmp1 = (mixmatrix[j][k][0] == 1) ? col3.at(k) << 1 : 0;
                tmp2 = (mixmatrix[j][k][1] == 1) ? col3.at(k) : 0;
                tmp3.at(k) = tmp1 ^ tmp2;
            }
            cipher.at(j + 8) = tmp3.at(0) ^ tmp3.at(1) ^ tmp3.at(2) ^ tmp3.at(3);
            
            for (int k = 0; k < 4; k++) {
                tmp1 = (mixmatrix[j][k][0] == 1) ? col4.at(k) << 1 : 0;
                tmp2 = (mixmatrix[j][k][1] == 1) ? col4.at(k) : 0;
                tmp3.at(k) = tmp1 ^ tmp2;
            }
            cipher.at(j + 12) = tmp3.at(0) ^ tmp3.at(1) ^ tmp3.at(2) ^ tmp3.at(3);
        }
        for (int j = 0; j < 16; j++)
            cipher.at(j) = (cipher.at(j) > 255) ? cipher.at(j) ^ 283 : cipher.at(j);
        
        // xor
        for (int j = 0; j < 16; j++) {
            cipher.at(j) = cipher.at(j) ^ keys.at(i).at(j);
        }
    }
    
    // --last round--
    
    // s-box substition
    for (int j = 0; j < 16; j++) {
        std::stringstream ss;
        ss << std::hex << cipher.at(j);
        string hex_num(ss.str());
        if (hex_num.length() == 1)
            hex_num.insert(0, "0");
        cipher.at(j) = sbox[hex2dec.at(hex_num.at(0))][hex2dec.at(hex_num.at(1))];
    }
    
    // shift rows
    row2 = {cipher.at(5), cipher.at(9), cipher.at(13), cipher.at(1)};
    row3 = {cipher.at(10), cipher.at(14), cipher.at(2), cipher.at(6)};
    row4 = {cipher.at(15), cipher.at(3), cipher.at(7), cipher.at(11)};

    for (int j = 0; j < 4; j++) {
        cipher.at(1 + 4 * j) = row2.at(j);
        cipher.at(2 + 4 * j) = row3.at(j);
        cipher.at(3 + 4 * j) = row4.at(j);
    }
    
    // xor
    for (int j = 0; j < 16; j++)
        cipher.at(j) = cipher.at(j) ^ keys.at(10).at(j);

    return cipher;
}

// returns decrypted hex text
vector<int> cipher_decrypt(const vector<int> &cipher_input, const vector<vector<int>> &keys) {
    
    vector<int> hex_output (16);
    
    // --initial round-- (xor)
    for (int j = 0; j < 16; j++) {
        hex_output.at(j) = cipher_input.at(j) ^ keys.at(10).at(j);
    }
    
    // --main rounds--
    for (int i = 9; i > -1; i--) {
        
        // s-box substition
        for (int j = 0; j < 16; j++) {
            std::stringstream ss;
            ss << std::hex << hex_output.at(j);
            string hex_num(ss.str());
            if (hex_num.length() == 1)
                hex_num.insert(0, "0");
            hex_output.at(j) = invsbox[hex2dec.at(hex_num.at(0))][hex2dec.at(hex_num.at(1))];
        }
        
        // shift rows
        vector<int> row2 {hex_output.at(13), hex_output.at(1), hex_output.at(5), hex_output.at(9)};
        vector<int> row3 {hex_output.at(10), hex_output.at(14), hex_output.at(2), hex_output.at(6)};
        vector<int> row4 {hex_output.at(7), hex_output.at(11), hex_output.at(15), hex_output.at(3)};

        for (int j = 0; j < 4; j++) {
            hex_output.at(1 + 4 * j) = row2.at(j);
            hex_output.at(2 + 4 * j) = row3.at(j);
            hex_output.at(3 + 4 * j) = row4.at(j);
        }
        
        // mix columns
        vector<int> col1 {vector<int>(hex_output.begin(), hex_output.begin() + 4)};
        vector<int> col2 {vector<int>(hex_output.begin() + 4, hex_output.begin() + 8)};
        vector<int> col3 {vector<int>(hex_output.begin() + 8, hex_output.begin() + 12)};
        vector<int> col4 {vector<int>(hex_output.begin() + 12, hex_output.end())};
        
        int tmp1 {};
        int tmp2 {};
        int tmp3 {};
        int tmp4 {};
        vector<int> tmp5 (4);
        
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                tmp1 = (revmixmatrix[j][k][0] == 1) ? col1.at(k) << 3 : 0;
                tmp2 = (revmixmatrix[j][k][1] == 1) ? col1.at(k) << 2 : 0;
                tmp3 = (revmixmatrix[j][k][2] == 1) ? col1.at(k) << 1 : 0;
                tmp4 = (revmixmatrix[j][k][3] == 1) ? col1.at(k): 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hex_output.at(j) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);
            
            for (int k = 0; k < 4; k++) {
                tmp1 = (revmixmatrix[j][k][0] == 1) ? col2.at(k) << 3 : 0;
                tmp2 = (revmixmatrix[j][k][1] == 1) ? col2.at(k) << 2 : 0;
                tmp3 = (revmixmatrix[j][k][2] == 1) ? col2.at(k) << 1 : 0;
                tmp4 = (revmixmatrix[j][k][3] == 1) ? col2.at(k) : 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hex_output.at(j + 4) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);
            
            for (int k = 0; k < 4; k++) {
                tmp1 = (revmixmatrix[j][k][0] == 1) ? col3.at(k) << 3 : 0;
                tmp2 = (revmixmatrix[j][k][1] == 1) ? col3.at(k) << 2 : 0;
                tmp3 = (revmixmatrix[j][k][2] == 1) ? col3.at(k) << 1 : 0;
                tmp4 = (revmixmatrix[j][k][3] == 1) ? col3.at(k) : 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hex_output.at(j + 8) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);
            
            for (int k = 0; k < 4; k++) {
                tmp1 = (revmixmatrix[j][k][0] == 1) ? col4.at(k) << 3 : 0;
                tmp2 = (revmixmatrix[j][k][1] == 1) ? col4.at(k) << 2 : 0;
                tmp3 = (revmixmatrix[j][k][2] == 1) ? col4.at(k) << 1 : 0;
                tmp4 = (revmixmatrix[j][k][3] == 1) ? col4.at(k) : 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hex_output.at(j + 12) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);
        }
        for (int j = 0; j < 16; j++) {
            hex_output.at(j) = (hex_output.at(j) > 255) ? hex_output.at(j) ^ 283 : hex_output.at(j);
        }
        
        // xor
        for (int j = 0; j < 16; j++) {
            hex_output.at(j) = hex_output.at(j) ^ keys.at(i).at(j);
        }
    }
    
    return hex_output;
}

int main() {
    // key
    string key = "TEAMSCORPIAN1234";

    // convert string key to hexidecimal --great--
    vector<int> hex_key = str2hex(key);
    
    // key generation --great--
    vector<vector<int>> keys = key_gen(hex_key);
    
    // input
    string input = "Two One Nine Two";
    
    // convert string input to hexidecimal --great--
    vector<int> hex_input = str2hex(input);

    // AES Encryption --great--
    vector<int> cipher_input = cipher_encrypt(hex_input, keys);
    
    // print out cipher
    for (int i = 0; i < 16; i++) {
        cout << std::hex << cipher_input.at(i) << " ";
    }
    
    // AES Decryption --need to check--
    vector<int> hex_output = cipher_decrypt(cipher_input, keys);
    
    // print out decryption
    cout << endl;
    for (int i = 0; i < 16; i++) {
        cout << hex_output.at(i) << " ";
    }

    return 0;
}