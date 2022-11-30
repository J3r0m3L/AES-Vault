#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::copy;

// rcon table
const int rcon[10]{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36 };

// hexadecimal to decimal conversion table
const std::map<char, int> hex2dec{ {'0', 0},  {'1', 1},  {'2', 2},  {'3', 3},
                                   {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7},
                                   {'8', 8},  {'9', 9},  {'a', 10}, {'b', 11},
                                   {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15} };

// matrix to forward mix columns
const int forwardMixer[4][4][2]{ {{1, 0}, {1, 1}, {0, 1}, {0, 1}},
                                 {{0, 1}, {1, 0}, {1, 1}, {0, 1}},
                                 {{0, 1}, {0, 1}, {1, 0}, {1, 1}},
                                 {{1, 1}, {0, 1}, {0, 1}, {1, 0}} };

// matrix to inverse mix columns
const int reverseMixer[4][4][4]{ {{1, 1, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 1}, {1, 0, 0, 1}},
                                 {{1, 0, 0, 1}, {1, 1, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 1}},
                                 {{1, 1, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 0}, {1, 0, 1, 1}},
                                 {{1, 0, 1, 1}, {1, 1, 0, 1}, {1, 0, 0, 1}, {1, 1, 1, 0}} };

// forward sbox
const int forwardSbox[16][16]{ {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
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
                               {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16} };

// inverse sbox
const int inverseSbox[16][16]{ {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb},
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
                               {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d} };

// ASCII conversion table
const std::map<int, char> printableChars{ {32, ' '}, {33, '!'}, {34, '"'}, {35, '#'}, {36, '$'}, {37, '%'},
                                          {38, '&'}, {39, '\''}, {40, '('}, {41, ')'}, {42, '*'}, {43, '+'},
                                          {44, ','}, {45, '-'}, {46, '.'}, {47, '/'}, {48, '0'}, {49, '1'},
                                          {50, '2'}, {51, '3'}, {52, '4'}, {53, '5'}, {54, '6'}, {55, '7'},
                                          {56, '8'}, {57, '9'}, {58, ':'}, {59, ';'}, {60, '<'}, {61, '='},
                                          {62, '>'}, {63, '?'}, {64, '@'}, {65, 'A'}, {66, 'B'}, {67, 'C'},
                                          {68, 'D'}, {69, 'E'}, {70, 'F'}, {71, 'G'}, {72, 'H'}, {73, 'I'},
                                          {74, 'J'}, {75, 'K'}, {76, 'L'}, {77, 'M'}, {78, 'N'}, {79, 'O'},
                                          {80, 'P'}, {81, 'Q'}, {82, 'R'}, {83, 'S'}, {84, 'T'}, {85, 'U'},
                                          {86, 'V'}, {87, 'W'}, {88, 'X'}, {89, 'Y'}, {90, 'Z'}, {91, '['},
                                          {92, '\\'}, {93, ']'}, {94, '^'}, {95, '_'}, {96, '`'}, {97, 'a'},
                                          {98, 'b'}, {99, 'c'}, {100, 'd'}, {101, 'e'}, {102, 'f'}, {103, 'g'},
                                          {104, 'h'}, {105, 'i'}, {106, 'j'}, {107, 'k'}, {108, 'l'}, {109, 'm'},
                                          {110, 'n'}, {111, 'o'}, {112, 'p'}, {113, 'q'}, {114, 'r'}, {115, 's'},
                                          {116, 't'}, {117, 'u'}, {118, 'v'}, {119, 'w'}, {120, 'x'}, {121, 'y'},
                                          {122, 'z'}, {123, '{'}, {124, '|'}, {125, '}'}, {126, '~'} };

// converts plain text into a hexadecimal vector
vector<int> text2hexa(const string& text) {
    vector<int> hexa(0);
    for (auto& c : text)
        hexa.push_back(printableChars.at(c));
    return hexa;
}

// converts hexadecimal text into a hexadecimal vector
vector<int> hexa2hexa(const string& text) {
    vector<int> hexa(int(text.length()) / 2);
    string text_slice{};
    for (int i = 0; i < int(text.length()); i += 2) {
        std::stringstream ss;
        text_slice = text.substr(i, 2);
        ss << text_slice;
        ss >> std::hex >> hexa.at(i / 2);
    }
    return hexa;
}

// converts a hexadecimal vector into hexadecimal text
string hexa2hexa(const vector<int>& hexa) {
    string text{};
    string hexa_slice{};
    for (int i = 0; i < int(hexa.size()); i++) {
        std::stringstream ss;
        ss << std::hex << hexa.at(i);
        hexa_slice = ss.str();
        if (hexa_slice.length() == 1)
            hexa_slice.insert(0, "0");
        text.append(hexa_slice);
    }
    return text;
}

// converts a hexadecimal vector into plain text
string hexa2text(const vector<int>& hexa) {
    string text{};
    for (int i = 0; i < int(hexa.size()); i++)
        text.push_back(printableChars.at(hexa.at(i)));
    return text;
}

// adds padding to a hexadecimal vector
vector<int> pad(const vector<int>& hexa) {
    vector<int> padded_hexa{ hexa };
    int difference = (int(hexa.size()) % 16 != 0) ? 16 - (int(hexa.size()) % 16) : 0;
    vector<int> padding(difference, 32);
    padded_hexa.insert(padded_hexa.end(), padding.begin(), padding.end());
    return padded_hexa;
}

// generates 11 round keys from key vector
vector<vector<int>> key_gen(const vector<int>& key) {

    vector<vector<int>> keys(11, key);
    vector<int> tmp(4);
    string hexa_slice{};

    for (int i = 0; i < 10; i++) {

        // rotate
        tmp = vector<int>(keys.at(i).begin() + 13, keys.at(i).end());
        tmp.push_back(keys.at(i).at(12));

        // s-box substitution
        for (int j = 0; j < 4; j++) {
            std::stringstream ss;
            ss << std::hex << tmp.at(j);
            hexa_slice = ss.str();
            if (hexa_slice.length() == 1)
                hexa_slice.insert(0, "0");
            tmp.at(j) = forwardSbox[hex2dec.at(hexa_slice.at(0))][hex2dec.at(hexa_slice.at(1))];
        }

        // xor operation
        bool toggle{ true };
        for (int j = 0; j < 4; j++) {
            if (toggle == true) {
                keys.at(i + 1).at(j) = keys.at(i).at(j) ^ tmp.at(j) ^ rcon[i];
                toggle = false;
            }
            else {
                keys.at(i + 1).at(j) = keys.at(i).at(j) ^ tmp.at(j);
            }
            keys.at(i + 1).at(j + 4) = keys.at(i).at(j + 4) ^ keys.at(i + 1).at(j);
            keys.at(i + 1).at(j + 8) = keys.at(i).at(j + 8) ^ keys.at(i + 1).at(j + 4);
            keys.at(i + 1).at(j + 12) = keys.at(i).at(j + 12) ^ keys.at(i + 1).at(j + 8);
        }
    }

    return keys;
}

// converts hexadecimal vector into aes encrypted hexadecimal vector
vector<int> aes_encrypt(const vector<int>& hexa, const vector<vector<int>>& keys) {

    vector<int> cipher(16);

    // initialize column vectors, row vectors, and tmp variables
    string hexa_num{};
    vector<int> row2(4);
    vector<int> row3(4);
    vector<int> row4(4);
    vector<int> col1(4);
    vector<int> col2(4);
    vector<int> col3(4);
    vector<int> col4(4);
    int tmp1{};
    int tmp2{};
    vector<int> tmp3(4);

    // --initial round--
    for (int j = 0; j < 16; j++)
        cipher.at(j) = hexa.at(j) ^ keys.at(0).at(j);

    // --main rounds--
    for (int i = 1; i < 10; i++) {

        // s-box substition
        for (int j = 0; j < 16; j++) {
            std::stringstream ss;
            ss << std::hex << cipher.at(j);
            hexa_num = ss.str();
            if (hexa_num.length() == 1)
                hexa_num.insert(0, "0");
            cipher.at(j) = forwardSbox[hex2dec.at(hexa_num.at(0))][hex2dec.at(hexa_num.at(1))];
        }

        // shift rows
        row2 = { cipher.at(5), cipher.at(9), cipher.at(13), cipher.at(1) };
        row3 = { cipher.at(10), cipher.at(14), cipher.at(2), cipher.at(6) };
        row4 = { cipher.at(15), cipher.at(3), cipher.at(7), cipher.at(11) };
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
                tmp1 = (forwardMixer[j][k][0] == 1) ? col1.at(k) << 1 : 0;
                tmp2 = (forwardMixer[j][k][1] == 1) ? col1.at(k) : 0;
                tmp3.at(k) = tmp1 ^ tmp2;
            }
            cipher.at(j) = tmp3.at(0) ^ tmp3.at(1) ^ tmp3.at(2) ^ tmp3.at(3);

            for (int k = 0; k < 4; k++) {
                tmp1 = (forwardMixer[j][k][0] == 1) ? col2.at(k) << 1 : 0;
                tmp2 = (forwardMixer[j][k][1] == 1) ? col2.at(k) : 0;
                tmp3.at(k) = tmp1 ^ tmp2;
            }
            cipher.at(j + 4) = tmp3.at(0) ^ tmp3.at(1) ^ tmp3.at(2) ^ tmp3.at(3);

            for (int k = 0; k < 4; k++) {
                tmp1 = (forwardMixer[j][k][0] == 1) ? col3.at(k) << 1 : 0;
                tmp2 = (forwardMixer[j][k][1] == 1) ? col3.at(k) : 0;
                tmp3.at(k) = tmp1 ^ tmp2;
            }
            cipher.at(j + 8) = tmp3.at(0) ^ tmp3.at(1) ^ tmp3.at(2) ^ tmp3.at(3);

            for (int k = 0; k < 4; k++) {
                tmp1 = (forwardMixer[j][k][0] == 1) ? col4.at(k) << 1 : 0;
                tmp2 = (forwardMixer[j][k][1] == 1) ? col4.at(k) : 0;
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
        hexa_num = ss.str();
        if (hexa_num.length() == 1)
            hexa_num.insert(0, "0");
        cipher.at(j) = forwardSbox[hex2dec.at(hexa_num.at(0))][hex2dec.at(hexa_num.at(1))];
    }

    // shift rows
    row2 = { cipher.at(5), cipher.at(9), cipher.at(13), cipher.at(1) };
    row3 = { cipher.at(10), cipher.at(14), cipher.at(2), cipher.at(6) };
    row4 = { cipher.at(15), cipher.at(3), cipher.at(7), cipher.at(11) };
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

// converts aes encrypted hexadecimal vector into hexadecimal vector
vector<int> aes_decrypt(const vector<int>& cipher, const vector<vector<int>>& keys) {

    vector<int> hexa(16);

    // initialize column vectors, row vectors, and tmp variables
    string hex_num{};
    vector<int> row2(4);
    vector<int> row3(4);
    vector<int> row4(4);
    vector<int> col1(4);
    vector<int> col2(4);
    vector<int> col3(4);
    vector<int> col4(4);
    int tmp1{};
    int tmp2{};
    int tmp3{};
    int tmp4{};
    vector<int> tmp5(4);
    int incrementer{};

    // --initial round--

    // xor
    for (int j = 0; j < 16; j++)
        hexa.at(j) = cipher.at(j) ^ keys.at(10).at(j);

    // shift rows
    row2 = { hexa.at(13), hexa.at(1), hexa.at(5), hexa.at(9) };
    row3 = { hexa.at(10), hexa.at(14), hexa.at(2), hexa.at(6) };
    row4 = { hexa.at(7), hexa.at(11), hexa.at(15), hexa.at(3) };
    for (int j = 0; j < 4; j++) {
        hexa.at(1 + 4 * j) = row2.at(j);
        hexa.at(2 + 4 * j) = row3.at(j);
        hexa.at(3 + 4 * j) = row4.at(j);
    }

    // inverse s-box substitution
    for (int j = 0; j < 16; j++) {
        std::stringstream ss;
        ss << std::hex << hexa.at(j);
        hex_num = ss.str();
        if (hex_num.length() == 1)
            hex_num.insert(0, "0");
        hexa.at(j) = inverseSbox[hex2dec.at(hex_num.at(0))][hex2dec.at(hex_num.at(1))];
    }

    // --main rounds--
    for (int i = 9; i > 0; i--) {

        // xor
        for (int j = 0; j < 16; j++)
            hexa.at(j) = hexa.at(j) ^ keys.at(i).at(j);

        // mix columns
        col1 = vector<int>(hexa.begin(), hexa.begin() + 4);
        col2 = vector<int>(hexa.begin() + 4, hexa.begin() + 8);
        col3 = vector<int>(hexa.begin() + 8, hexa.begin() + 12);
        col4 = vector<int>(hexa.begin() + 12, hexa.end());

        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                tmp1 = (reverseMixer[j][k][3] == 1) ? col1.at(k) : 0;
                incrementer = col1.at(k) << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp2 = (reverseMixer[j][k][2] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp3 = (reverseMixer[j][k][1] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp4 = (reverseMixer[j][k][0] == 1) ? incrementer : 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hexa.at(j) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);

            for (int k = 0; k < 4; k++) {
                tmp1 = (reverseMixer[j][k][3] == 1) ? col2.at(k) : 0;
                incrementer = col2.at(k) << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp2 = (reverseMixer[j][k][2] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp3 = (reverseMixer[j][k][1] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp4 = (reverseMixer[j][k][0] == 1) ? incrementer : 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hexa.at(j + 4) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);

            for (int k = 0; k < 4; k++) {
                tmp1 = (reverseMixer[j][k][3] == 1) ? col3.at(k) : 0;
                incrementer = col3.at(k) << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp2 = (reverseMixer[j][k][2] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp3 = (reverseMixer[j][k][1] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp4 = (reverseMixer[j][k][0] == 1) ? incrementer : 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hexa.at(j + 8) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);

            for (int k = 0; k < 4; k++) {
                tmp1 = (reverseMixer[j][k][3] == 1) ? col4.at(k) : 0;
                incrementer = col4.at(k) << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp2 = (reverseMixer[j][k][2] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp3 = (reverseMixer[j][k][1] == 1) ? incrementer : 0;
                incrementer = incrementer << 1;
                incrementer = (incrementer > 255) ? incrementer ^ 283 : incrementer;
                tmp4 = (reverseMixer[j][k][0] == 1) ? incrementer : 0;
                tmp5.at(k) = tmp1 ^ tmp2 ^ tmp3 ^ tmp4;
            }
            hexa.at(j + 12) = tmp5.at(0) ^ tmp5.at(1) ^ tmp5.at(2) ^ tmp5.at(3);
        }

        // shift rows
        row2 = { hexa.at(13), hexa.at(1), hexa.at(5), hexa.at(9) };
        row3 = { hexa.at(10), hexa.at(14), hexa.at(2), hexa.at(6) };
        row4 = { hexa.at(7), hexa.at(11), hexa.at(15), hexa.at(3) };
        for (int j = 0; j < 4; j++) {
            hexa.at(1 + 4 * j) = row2.at(j);
            hexa.at(2 + 4 * j) = row3.at(j);
            hexa.at(3 + 4 * j) = row4.at(j);
        }

        // inverse s-box substitution
        for (int j = 0; j < 16; j++) {
            std::stringstream ss;
            ss << std::hex << hexa.at(j);
            hex_num = ss.str();
            if (hex_num.length() == 1)
                hex_num.insert(0, "0");
            hexa.at(j) = inverseSbox[hex2dec.at(hex_num.at(0))][hex2dec.at(hex_num.at(1))];
        }
    }

    // last round
    for (int j = 0; j < 16; j++)
        hexa.at(j) = hexa.at(j) ^ keys.at(0).at(j);

    return hexa;
}

// combines the all other functions to fully encrypt text
string encrypt(const string& input, const vector<vector<int>>& keys) {
    //convert input string to hexadecimal
    vector<int> hexa = text2hexa(input);

    // pad vector input if it isn't already divisible by 16
    hexa = pad(hexa);


    // AES Encryption
    vector<int> hexa_slice(16);
    for (int i = 0; i < int(hexa.size()); i += 16) {
        hexa_slice = vector<int>(hexa.begin() + i, hexa.begin() + (i + 16));
        hexa_slice = aes_encrypt(hexa_slice, keys);
        copy(hexa_slice.begin(), hexa_slice.end(), hexa.begin() + i);
    }

    // return string
    string hexa_string = hexa2hexa(hexa);
    return hexa_string;
}

// combines the all other functions to fully decrypt text
string decrypt(const string& input, const vector<vector<int>>& keys) {
    //convert input string to hexadecimal
    vector<int> text = hexa2hexa(input);

    // AES Decryption
    vector<int> text_slice(16);
    for (int i = 0; i < int(text.size()); i += 16) {
        text_slice = vector<int>(text.begin() + i, text.begin() + (i + 16));
        text_slice = aes_decrypt(text_slice, keys);
        copy(text_slice.begin(), text_slice.end(), text.begin() + i);
    }

    // Remove Padding
    int i = int(text.size()) - 1;
    while (text.at(i) == 32) {
        text.pop_back();
        i -= 1;
    }

    string text_string = hexa2text(text);
    return text_string;
}