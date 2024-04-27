#ifndef HASH_H
#define HASH_H

#include <chrono>
#include <cmath>
#include <iostream>
#include <random>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5]{983132572, 1468777056, 552714139, 984953261,
                            261934300};
    MyStringHash(bool debug = true) {
        if (false == debug) {
            generateRValues();
        }
    }

    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const {
        // Add your code here
        HASH_INDEX_T w[5] = {0, 0, 0, 0, 0};
        for (int i = (int)k.size() - 1, id = 4; i >= 0; i -= 6) {
            // len = min(6, i + 1)
            size_t len = i + 1 < 6 ? i + 1 : 6;
            std::string t = k.substr(i - len + 1, len);
            HASH_INDEX_T wi = 0;
            // for (int j = (int)t.size() - 1; j >= 0; j--) {
            for (size_t j = 0; j < t.size(); j++){
                wi = wi * 36 + letterDigitToNumber(t[j]);
            }
            w[id--] = wi;
        }
        
        HASH_INDEX_T result = 0;
        for (size_t i = 0; i < 5; i++) {
            result += w[i] * rValues[i];
        }
        return result;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const {
        // Add code here or delete this helper function if you do not want it
        letter = std::tolower(letter);
        if (letter >= 'a' && letter <= 'z')
            return letter - 'a';
        return letter - '0' + 26;
    }

    // Code to generate the random R values
    void generateRValues() {
        // obtain a seed from the system clock:
        unsigned seed =
            std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator(
            seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random
        // number
        for (int i{0}; i < 5; ++i) {
            rValues[i] = generator();
        }
    }
};

#endif
