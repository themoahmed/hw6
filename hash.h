#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <cstdlib>
#include <chrono>



typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const {
        // Add your code here

        // setting variables
        unsigned long long segments[5] = {0}; 
        unsigned long long baseMultiplier = 1; 
        unsigned long long segmentValue = 0;   
        int charCount = 0;                     
        int segmentIndex = 4;                  

        // Processing each character 
        int pos = k.length() - 1;
            while (pos >= 0) {
                // Convert each character to its base-36 equivalent 
                segmentValue += baseMultiplier * convertCharToBase36(k[pos]);
                baseMultiplier *= 36; 
                charCount++; 

                // 6 characters 
                if (charCount == 6) {
                    segments[segmentIndex--] = segmentValue; // Save the segment value
                    // Reset variables to start a new segment
                    baseMultiplier = 1;
                    segmentValue = 0;
                    charCount = 0;
                }
                pos--;
            }


        // If there's a remaining segment value, store it
        if (segmentIndex >= 0) segments[segmentIndex] = segmentValue;

        //  final hash 
        HASH_INDEX_T finalHash = 0;
        for (int i = 0; i < 5; i++) {
            finalHash += rValues[i] * segments[i];
        }

        return finalHash;
    }

    unsigned long long convertCharToBase36(char c) const {
        if (isdigit(c))
            return c - '0' + 26;
        else
            return tolower(c) - 'a';
    }


    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const {
    // Normalize the k letter to lowercase
        char normalized = tolower(letter);


        if (normalized >= 'a' && normalized <= 'z') {
       
            return (HASH_INDEX_T)(normalized - 'a');
        }

        else if (normalized >= '0' && normalized <= '9') {
           
            return (HASH_INDEX_T)(26 + normalized - '0');
        }

        return (HASH_INDEX_T)-1;
}


    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
