#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// Define constants for key, IV, and key stream sizes
#define KEY_SIZE 80
#define IV_SIZE 80
#define KEY_STREAM_SIZE 512

// Class representing the Trivium cipher
class TriviumCipher
{
    vector<bool> streamOne;   // Stream 1: 93 bits
    vector<bool> streamTwo;   // Stream 2: 84 bits
    vector<bool> streamThree; // Stream 3: 111 bits

public:
    // Constructor to initialize the Trivium cipher with IV and KEY
    TriviumCipher(const vector<bool> &IV, const vector<bool> &KEY)
    {
        // Initialize the streams with their respective sizes
        streamOne.resize(93, 0);
        streamTwo.resize(84, 0);
        streamThree.resize(111, 0);

        // Set the initial values of streamOne and streamTwo with KEY and IV respectively
        setStream(KEY, streamOne, 80, 1); // Initialize streamOne with KEY
        setStream(IV, streamTwo, 80, 0);  // Initialize streamTwo with IV
        
        // Set the last three bits of streamThree to 1 as per Trivium cipher specifications
        streamThree[110] = 1;
        streamThree[109] = 1;
        streamThree[108] = 1;

        // Initialize the internal state by running the updator function 1152 times
        // This is part of the key scheduling to produce a valid keystream
        for (int i = 0; i < 1152; ++i) {
            updator();
        }
    }

    // Method to set the stream with either KEY or IV data
    void setStream(const vector<bool> &source, vector<bool> &destination, int keySize, int rem)
    {
        if (rem == 1) {
            // When rem is 1, initialize streamOne with KEY data
            for (int i = 0; i < keySize; ++i) {
                destination[i] = source[i];
            }
        } else {
            // When rem is 0, initialize streamTwo with IV data
            for (int i = 0; i < keySize; ++i) {
                destination[i] = source[i];
            }
        }
    }

    // Generate a keystream of the specified size
    vector<bool> genKeyStream()
    {
        vector<bool> keyStream(KEY_STREAM_SIZE);
        // Generate each bit of the keystream by calling the updator function
        for (int i = 0; i < KEY_STREAM_SIZE; ++i) {
            keyStream[i] = updator();
        }

        // Reverse the bits in each byte of the keystream to match Trivium output format
        revKeystreamBits(keyStream); 
        return keyStream;
    }

    // Reverse the bits in each byte of the keystream
    void revKeystreamBits(vector<bool> &keyStream)
    {
        // Iterate over the keystream in 8-bit (1-byte) chunks
        for (size_t i = 0; i < KEY_STREAM_SIZE; i += 8)
        {
            vector<bool> byte(8);
            // Extract the 8-bit byte from the keystream
            for (size_t j = 0; j < 8; ++j) {
                byte[j] = keyStream[i + j];
            }

            // Reverse the bits in this 8-bit byte
            for (size_t j = 0; j < 8; ++j) {
                keyStream[i + j] = byte[7 - j];
            }
        }
    }

private:
    // Update the internal state and return the keystream bit
    bool updator()
    {
        // Compute intermediate values used to update the streams
        bool t1 = streamOne[65] ^ streamOne[92];
        bool t2 = streamTwo[68] ^ streamTwo[83];
        bool t3 = streamThree[65] ^ streamThree[110];
        bool kbit = t1 ^ t2 ^ t3; // The output bit of the keystream

        // Compute new bits to be inserted into the streams
        bool sb1 = t1 ^ (streamOne[91] & streamOne[90]) ^ streamTwo[77];
        bool sb2 = t2 ^ (streamTwo[82] & streamTwo[81]) ^ streamThree[86];
        bool sb3 = t3 ^ (streamThree[109] & streamThree[108]) ^ streamOne[68];

        // Update the streams by inserting new bits at the beginning
        streamOne.insert(streamOne.begin(), sb3);
        streamTwo.insert(streamTwo.begin(), sb1);
        streamThree.insert(streamThree.begin(), sb2);

        // Remove the oldest bit from each stream to maintain the stream size
        streamOne.pop_back();
        streamTwo.pop_back();
        streamThree.pop_back();

        return kbit;
    }
};

// Convert a hexadecimal string to a binary string
string hexToBinary(const string &hex)
{
    string binaryString = "";
    // Convert each hexadecimal digit to its 4-bit binary representation
    for (char c : hex)
    {
        switch (toupper(c))
        {
        case '0': binaryString += "0000"; break;
        case '1': binaryString += "0001"; break;
        case '2': binaryString += "0010"; break;
        case '3': binaryString += "0011"; break;
        case '4': binaryString += "0100"; break;
        case '5': binaryString += "0101"; break;
        case '6': binaryString += "0110"; break;
        case '7': binaryString += "0111"; break;
        case '8': binaryString += "1000"; break;
        case '9': binaryString += "1001"; break;
        case 'A': binaryString += "1010"; break;
        case 'B': binaryString += "1011"; break;
        case 'C': binaryString += "1100"; break;
        case 'D': binaryString += "1101"; break;
        case 'E': binaryString += "1110"; break;
        case 'F': binaryString += "1111"; break;
        }
    }
    return binaryString;
}

// Convert a binary string to a vector of bools
vector<bool> stringToVector(const string &binary)
{
    vector<bool> vec;
    for (char bit : binary)
        vec.push_back(bit == '1'); // Convert '0' to false and '1' to true
    return vec;
}

// Convert a vector of bools to a hexadecimal string
string bitsToHex(const vector<bool> &bits)
{
    stringstream ss;
    ss << hex << uppercase;
    // Process the bits in 4-bit chunks (nibbles)
    for (size_t i = 0; i < bits.size(); i += 4)
    {
        int value = 0;
        // Compute the integer value of the 4-bit nibble
        for (size_t j = 0; j < 4; ++j) {
            value = (value << 1) | bits[i + j];
        }
        // Output the nibble as a hexadecimal digit
        ss << setw(1) << setfill('0') << value;
    }
    return ss.str();
}

// Reverse pairs of characters in a string and then reverse the entire string
string reversePairs(const string& str) {
    string result;
    int length = str.length();
    
    // Process each pair in the string
    for (int i = 0; i < length; i += 2) {
        if (i + 1 < length) {
            result += str[i + 1]; // Add the second character of the pair first
        }
        result += str[i]; // Add the first character of the pair second
    }
    
    // Reverse the entire resulting string
    reverse(result.begin(), result.end());
    
    return result;
}

int main()
{
    string iv, key;

    // Prompt the user to enter the IV and KEY values
    cout << "Enter IV: ";
    cin >> iv;
    cout << "Enter KEY: ";
    cin >> key;

    // Validate the lengths of IV and KEY
    if (iv.length() != IV_SIZE / 4 || key.length() != KEY_SIZE / 4)
    {
        cout << "IV and KEY must be exactly " << IV_SIZE / 4 << " and " << KEY_SIZE / 4 << " hexadecimal digits respectively." << endl;
        return 1;
    }

    // Reverse the order of pairs and then reverse the entire string for IV and KEY
    iv = reversePairs(iv);
    key = reversePairs(key);

    // Convert IV and KEY from hexadecimal to binary representation
    string ivVal = hexToBinary(iv);
    string keyVal = hexToBinary(key);

    // Convert binary strings to vectors of bools
    vector<bool> IV = stringToVector(ivVal);
    vector<bool> KEY = stringToVector(keyVal);

    // Create a TriviumCipher object with the provided IV and KEY
    TriviumCipher trivium(IV, KEY);

    // Generate the keystream using the Trivium cipher
    vector<bool> keyStream = trivium.genKeyStream();

    // Convert the generated keystream to hexadecimal format and display it
    string hexKeyStream = bitsToHex(keyStream);
    cout << "Generated Keystream (Hex): " << hexKeyStream << endl;

    return 0;
}
