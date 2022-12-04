#include <iostream>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <map>

using std::string;
using std::vector;
using std::bitset;

// initialize hash values
const uint32_t h0 = 0x6a09e667;
const uint32_t h1 = 0xbb67ae85;
const uint32_t h2 = 0x3c6ef372;
const uint32_t h3 = 0xa54ff53a;
const uint32_t h4 = 0x510e527f;
const uint32_t h5 = 0x9b05688c;
const uint32_t h6 = 0x1f83d9ab;
const uint32_t h7 = 0x5be0cd19;

// initialize array of round constants
const vector<uint32_t> k = { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
							 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
							 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
							 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
							 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
							 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
							 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
							 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };


// pads string and returns usable vector (must be less than 55 char or 447 bits)
vector<uint32_t> preprocess(const string& text) {
	string binary_text{ "" };
	for (auto& ch : text)
		binary_text += bitset<8>(ch).to_string();

	string binary_length = bitset<64>(binary_text.length()).to_string();
	binary_text.push_back('1');

	int difference = (binary_text.length() % 448 != 0) ? 448 - binary_text.length() : 0;
	string padding(difference, '0');
	binary_text += padding + binary_length;

	vector<uint32_t> hash(16);

	for (int i = 0; i < 16; i++)
		hash.at(i) = stoul(binary_text.substr(i * 32, 32), 0, 2);

	return hash;
}

// creates schedule array
vector<uint32_t> schedule(const vector<uint32_t>& hash) {
	vector<uint32_t> timing(48);
	timing.insert(timing.begin(), hash.begin(), hash.end());

	uint32_t x, y, z;
	uint32_t SigmaZero, SigmaOne;
	for (int i = 16; i < 64; i++) {
		x = (timing.at(i - 15) >> 7) ^ (timing.at(i - 15) << 25);
		y = (timing.at(i - 15) >> 18) ^ (timing.at(i - 15) << 14);
		z = (timing.at(i - 15) >> 3);
		SigmaZero = x ^ y ^ z;

		x = (timing.at(i - 2) >> 17) ^ (timing.at(i - 2) << 15);
		y = (timing.at(i - 2) >> 19) ^ (timing.at(i - 2) << 13);
		z = (timing.at(i - 2) >> 10);
		SigmaOne = x ^ y ^ z;

		timing.at(i) = timing.at(i - 16) + SigmaZero + timing.at(i - 7) + SigmaOne;
	}
	return timing;
}

// compression
vector<uint32_t> compress(vector<uint32_t>& timing) {
	vector<uint32_t> hashes = { h0, h1, h2, h3, h4, h5, h6, h7 };

	uint32_t x, y, z;
	uint32_t S1, ch, S0, maj;
	uint32_t temp1, temp2;

	for (int i = 0; i < 64; i++) {
		x = (hashes.at(4) >> 6) ^ (hashes.at(4) << 26);
		y = (hashes.at(4) >> 11) ^ (hashes.at(4) << 21);
		z = (hashes.at(4) >> 25) ^ (hashes.at(4) << 7);
		S1 = x ^ y ^ z;
		ch = (hashes.at(4) & hashes.at(5)) ^ (~hashes.at(4) & hashes.at(6));

		temp1 = hashes.at(7) + S1 + ch + k.at(i) + timing.at(i);

		x = (hashes.at(0) >> 2) ^ (hashes.at(0) << 30);
		y = (hashes.at(0) >> 13) ^ (hashes.at(0) << 19);
		z = (hashes.at(0) >> 22) ^ (hashes.at(0) << 10);
		S0 = x ^ y ^ z;
		maj = (hashes.at(0) & hashes.at(1)) ^ (hashes.at(0) & hashes.at(2)) ^ (hashes.at(1) & hashes.at(2));
		temp2 = S0 + maj;

		hashes.at(7) = hashes.at(6);
		hashes.at(6) = hashes.at(5);
		hashes.at(5) = hashes.at(4);
		hashes.at(4) = hashes.at(3) + temp1;
		hashes.at(3) = hashes.at(2);
		hashes.at(2) = hashes.at(1);
		hashes.at(1) = hashes.at(0);
		hashes.at(0) = temp1 + temp2;
	}

	return hashes;
}


// combines all other functions to generate hash
string convert2hash(string &text) {
	vector<uint32_t> hash = preprocess(text);

	vector<uint32_t> timing = schedule(hash);

	vector<uint32_t> hashes = compress(timing);

	// final calculations
	uint32_t h0Cpy = h0 + hashes.at(0);
	uint32_t h1Cpy = h1 + hashes.at(1);
	uint32_t h2Cpy = h2 + hashes.at(2);
	uint32_t h3Cpy = h3 + hashes.at(3);
	uint32_t h4Cpy = h4 + hashes.at(4);
	uint32_t h5Cpy = h5 + hashes.at(5);
	uint32_t h6Cpy = h6 + hashes.at(6);
	uint32_t h7Cpy = h7 + hashes.at(7);

	string hexa_num{};
	std::stringstream ss;
	ss << std::hex << h0Cpy << h1Cpy << h2Cpy << h3Cpy << h4Cpy << h5Cpy << h6Cpy << h7Cpy;
	hexa_num = ss.str();

	return hexa_num;
}