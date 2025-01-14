#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

void DES_encrypt_block(const unsigned char* input, unsigned char* output, const unsigned char* key) {
    for (int i = 0; i < 8; ++i) {
        output[i] = input[i] ^ key[i % 8];
    }
}

void DES_decrypt_block(const unsigned char* input, unsigned char* output, const unsigned char* key) {
    for (int i = 0; i < 8; ++i) {
        output[i] = input[i] ^ key[i % 8];
    }
}

void TripleDES_encrypt(const vector<unsigned char>& plaintext, vector<unsigned char>& ciphertext,
    const unsigned char* key1, const unsigned char* key2, const unsigned char* key3) {
    size_t block_size = 8; 
    size_t num_blocks = (plaintext.size() + block_size - 1) / block_size;

    ciphertext.resize(num_blocks * block_size);

    for (size_t i = 0; i < num_blocks; ++i) {
        unsigned char block[8] = { 0 };
        unsigned char temp[8] = { 0 };

        size_t start = i * block_size;
        size_t length = min(block_size, plaintext.size() - start);
        memcpy(block, plaintext.data() + start, length);

        DES_encrypt_block(block, temp, key1); 
        DES_decrypt_block(temp, block, key2); 
        DES_encrypt_block(block, temp, key3); 

        memcpy(ciphertext.data() + start, temp, block_size);
    }
}

void TripleDES_decrypt(const vector<unsigned char>& ciphertext, vector<unsigned char>& plaintext,
    const unsigned char* key1, const unsigned char* key2, const unsigned char* key3) {
    size_t block_size = 8;
    size_t num_blocks = ciphertext.size() / block_size;

    plaintext.resize(ciphertext.size());

    for (size_t i = 0; i < num_blocks; ++i) {
        unsigned char block[8] = { 0 };
        unsigned char temp[8] = { 0 };

        size_t start = i * block_size;
        memcpy(block, ciphertext.data() + start, block_size);

        DES_decrypt_block(block, temp, key3); 
        DES_encrypt_block(temp, block, key2); 
        DES_decrypt_block(block, temp, key1); 

        memcpy(plaintext.data() + start, temp, block_size);
    }
}

void generate_random_key(unsigned char* key, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        key[i] = rand() % 256; 
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); 

    string plaintext = "Evertec simplificando o mercado de pagamentos";
    unsigned char key1[8], key2[8], key3[8];


    generate_random_key(key1, 8);
    generate_random_key(key2, 8);
    generate_random_key(key3, 8);


    vector<unsigned char> plaintext_bytes(plaintext.begin(), plaintext.end());
    vector<unsigned char> ciphertext;
    vector<unsigned char> decryptedtext;


    TripleDES_encrypt(plaintext_bytes, ciphertext, key1, key2, key3);


    cout << "Ciphertext (hex): ";
    for (unsigned char c : ciphertext) {
        cout << hex << setw(2) << setfill('0') << (int)c;
    }
    cout << endl;


    TripleDES_decrypt(ciphertext, decryptedtext, key1, key2, key3);


    string decrypted_string(decryptedtext.begin(), decryptedtext.end());
    cout << "Decrypted text: " << decrypted_string << endl;

    return 0;
}