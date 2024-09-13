# Trivium
 A stream cipher that's used in cryptography and is designed to be lightweight and secure.

Overview
This project implements the Trivium Cipher, a lightweight stream cipher designed for efficiency in hardware and software. Trivium generates a stream of bits (keystream) which can be XORed with plaintext to produce ciphertext, and the same process can be used to decrypt the ciphertext back to plaintext.

The implementation generates a 512-bit keystream based on a key and initialization vector (IV), both provided as input in hexadecimal format.

Features
Keystream Generation: Produces 512 bits of keystream from a 80-bit key and IV.
Hexadecimal Input and Output: The key and IV are taken as hexadecimal values, and the generated keystream is output as hexadecimal.
Reversing 2-Byte Pairs: The key and IV are reversed in 2-byte pairs before initialization, and the keystream is reversed in the same way before output.
Test Vector Support: You can provide specific test vectors with predefined key and IV values for validation.
Input Requirements
Key: A 80-bit hexadecimal string (20 hex characters), entered without the 0x prefix.
IV: A 80-bit hexadecimal string (20 hex characters), entered without the 0x prefix.
Output
The program outputs the 512-bit keystream in hexadecimal format.
