#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Function to check if a number is prime
int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Function for modular exponentiation to avoid overflow
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to rotate right
unsigned int rotr(unsigned int value, unsigned int count) {
    return (value >> count) | (value << (32 - count));
}

// Function to calculate SHA-256 hash
void sha256(unsigned char *message, size_t message_len, unsigned char *digest) {
    unsigned int h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    unsigned int k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c4f, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa11, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    unsigned char padded_message[64 * ((message_len + 8) / 64 + 1)];
    memcpy(padded_message, message, message_len);
    padded_message[message_len] = 0x80;
    memset(padded_message + message_len + 1, 0, sizeof(padded_message) - message_len - 9);
    unsigned long long bits_len = message_len * 8;
    memcpy(padded_message + sizeof(padded_message) - 8, &bits_len, 8);

    for (size_t i = 0; i < sizeof(padded_message); i += 64) {
        unsigned int w[64] = {0};

        for (size_t j = 0; j < 16; j++) {
            w[j] = (padded_message[i + j * 4] << 24) |
                   (padded_message[i + j * 4 + 1] << 16) |
                   (padded_message[i + j * 4 + 2] << 8) |
                   (padded_message[i + j * 4 + 3]);
        }

        for (size_t j = 16; j < 64; j++) {
            unsigned int s0 = rotr(w[j - 15], 7) ^ rotr(w[j - 15], 18) ^ (w[j - 15] >> 3);
            unsigned int s1 = rotr(w[j - 2], 17) ^ rotr(w[j - 2], 19) ^ (w[j - 2] >> 10);
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }

        unsigned int a = h[0], b = h[1], c = h[2], d = h[3];
        unsigned int e = h[4], f = h[5], g = h[6], h0 = h[7];

        for (size_t j = 0; j < 64; j++) {
            unsigned int S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
            unsigned int ch = (e & f) ^ (~e & g);
            unsigned int temp1 = h0 + S1 + ch + k[j] + w[j];
            unsigned int S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
            unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
            unsigned int temp2 = S0 + maj;

            h0 = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a + temp2;
            a = temp1 + temp2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += h0;
    }

    for (int i = 0; i < 8; i++) {
        digest[i * 4] = (h[i] >> 24) & 0xFF;
        digest[i * 4 + 1] = (h[i] >> 16) & 0xFF;
        digest[i * 4 + 2] = (h[i] >> 8) & 0xFF;
        digest[i * 4 + 3] = h[i] & 0xFF;
    }
}

// RSA encryption function
void encrypt_RSA(char *message, int key, int n, int *encrypted_message) {
    for (int i = 0; message[i] != '\0'; i++) {
        encrypted_message[i] = mod_exp(message[i], key, n);
    }
}

// RSA decryption function
void decrypt_RSA(int *encrypted_message, int key, int n, char *decrypted_message, int length) {
    for (int i = 0; i < length; i++) {
        decrypted_message[i] = mod_exp(encrypted_message[i], key, n);
    }
    decrypted_message[length] = '\0';
}

// Other helper functions (gcd, modinv, find_e)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modinv(int e, int phi) {
    for (int d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) {
            return d;
        }
    }
    return -1;
}

int find_e(int phi) {
    int e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1) {
            return e;
        }
        e++;
    }
    return -1;
}

int main() {
    // Input original message
    char message[256];
    printf("Enter the message to encrypt: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;  // Remove newline character

    // Choose two large prime numbers p and q
    int p = 61; // Replace with a prime number of your choice
    int q = 53; // Replace with a prime number of your choice
    int n = p * q;  // Calculate n
    int phi = (p - 1) * (q - 1);  // Calculate phi(n)

    // Choose an encryption key e
    int e = find_e(phi);
    printf("Chosen e: %d\n", e);

    // Calculate the decryption key d
    int d = modinv(e, phi);
    printf("Calculated d: %d\n", d);

    // Encrypt the message
    int encrypted_message[256];
    encrypt_RSA(message, e, n, encrypted_message);
    
    // Output the encrypted message
    printf("Encrypted message: ");
    for (int i = 0; message[i] != '\0'; i++) {
        printf("%d ", encrypted_message[i]);
    }
    printf("\n");

    // Decrypt the message
    char decrypted_message[256];
    decrypt_RSA(encrypted_message, d, n, decrypted_message, strlen(message));

    // Output the decrypted message
    printf("Decrypted message: %s\n", decrypted_message);

    return 0;
}
