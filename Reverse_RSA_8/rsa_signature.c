#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Helper functions for modular exponentiation and primality testing

// Function to compute (base^exp) % mod
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Check if a number is prime (basic check, not optimized)
int is_prime(unsigned long long num) {
    if (num < 2) return 0;
    for (unsigned long long i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Function to generate a random prime number within a range
unsigned long long generate_prime(unsigned long long lower, unsigned long long upper) {
    unsigned long long prime;
    do {
        prime = (rand() % (upper - lower)) + lower;
    } while (!is_prime(prime));
    return prime;
}

// Function to calculate the greatest common divisor (GCD)
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Function to calculate the modular inverse of e mod φ(n)
unsigned long long mod_inverse(unsigned long long e, unsigned long long phi) {
    unsigned long long t = 0, newt = 1;
    unsigned long long r = phi, newr = e;
    while (newr != 0) {
        unsigned long long quotient = r / newr;
        t = newt; newt = t - quotient * newt;
        r = newr; newr = r - quotient * newr;
    }
    if (t < 0) t += phi;
    return t;
}

// Structure to hold RSA key pair (public and private)
typedef struct {
    unsigned long long n;  // modulus
    unsigned long long e;  // public exponent
    unsigned long long d;  // private exponent
} RSAKeyPair;

// Function to generate RSA keys
RSAKeyPair generateRSAKeys() {
    RSAKeyPair keypair;

    // Step 1: Generate two large prime numbers p and q
    unsigned long long p = generate_prime(100, 500);
    unsigned long long q = generate_prime(100, 500);

    // Step 2: Compute n = p * q
    keypair.n = p * q;

    // Step 3: Compute φ(n) = (p-1)*(q-1)
    unsigned long long phi = (p - 1) * (q - 1);

    // Step 4: Choose public exponent e such that 1 < e < φ(n) and gcd(e, φ(n)) = 1
    keypair.e = 3;  // Common choice of e
    while (gcd(keypair.e, phi) != 1) {
        keypair.e++;
    }

    // Step 5: Compute the private exponent d such that (d * e) % φ(n) = 1
    keypair.d = mod_inverse(keypair.e, phi);

    return keypair;
}

// Function to calculate SHA-1 hash
void sha1(unsigned char *message, int message_len, unsigned char *digest) {
    unsigned int h0 = 0x67452301;
    unsigned int h1 = 0xEFCDAB89;
    unsigned int h2 = 0x98BADCFE;
    unsigned int h3 = 0x10325476;
    unsigned int h4 = 0xC3D2E1F0;

    unsigned int message_block[16];
    unsigned int w[80];
    unsigned int a, b, c, d, e;
    unsigned int i, j;

    // Padding the message (this is a basic implementation and can be improved)
    unsigned char padded_message[64 * ((message_len + 8) / 64 + 1)];
    memcpy(padded_message, message, message_len);
    padded_message[message_len] = 0x80;
    memset(padded_message + message_len + 1, 0, sizeof(padded_message) - message_len - 9);
    unsigned long long bits_len = message_len * 8;
    memcpy(padded_message + sizeof(padded_message) - 8, &bits_len, 8);

    for (i = 0; i < sizeof(padded_message); i += 64) {
        for (j = 0; j < 16; j++) {
            message_block[j] = (padded_message[i + j * 4] << 24) |
                               (padded_message[i + j * 4 + 1] << 16) |
                               (padded_message[i + j * 4 + 2] << 8) |
                               (padded_message[i + j * 4 + 3]);
        }

        for (j = 16; j < 80; j++) {
            w[j] = (w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16]);
            w[j] = (w[j] << 1) | (w[j] >> 31); // Circular left shift
        }

        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;

        for (j = 0; j < 80; j++) {
            unsigned int k;
            if (j < 20) {
                k = (b & c) | ((~b) & d);
                k += 0x5A827999;
            } else if (j < 40) {
                k = b ^ c ^ d;
                k += 0x6ED9EBA1;
            } else if (j < 60) {
                k = (b & c) | (b & d) | (c & d);
                k += 0x8F1BBCDC;
            } else {
                k = b ^ c ^ d;
                k += 0xCA62C1D6;
            }
            k += a + w[j];
            a = e;
            e = d;
            d = c;
            c = b + (k << 5 | k >> 27); // Circular left shift
            b = b + k;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // Store the hash value in the digest
    digest[0] = (h0 >> 24) & 0xFF;
    digest[1] = (h0 >> 16) & 0xFF;
    digest[2] = (h0 >> 8) & 0xFF;
    digest[3] = h0 & 0xFF;
    digest[4] = (h1 >> 24) & 0xFF;
    digest[5] = (h1 >> 16) & 0xFF;
    digest[6] = (h1 >> 8) & 0xFF;
    digest[7] = h1 & 0xFF;
    digest[8] = (h2 >> 24) & 0xFF;
    digest[9] = (h2 >> 16) & 0xFF;
    digest[10] = (h2 >> 8) & 0xFF;
    digest[11] = h2 & 0xFF;
    digest[12] = (h3 >> 24) & 0xFF;
    digest[13] = (h3 >> 16) & 0xFF;
    digest[14] = (h3 >> 8) & 0xFF;
    digest[15] = h3 & 0xFF;
    digest[16] = (h4 >> 24) & 0xFF;
    digest[17] = (h4 >> 16) & 0xFF;
    digest[18] = (h4 >> 8) & 0xFF;
    digest[19] = h4 & 0xFF;
}

// Function to sign a message using the private key
unsigned long long signMessage(unsigned char *message, int message_len, RSAKeyPair keypair) {
    unsigned char digest[20]; // SHA-1 produces a 20-byte hash
    sha1(message, message_len, digest); // Generate hash

    unsigned long long message_hash = 0;
    for (int i = 0; i < 20; i++) {
        message_hash = (message_hash << 8) | digest[i]; // Convert hash to unsigned long long
    }

    // Generate the signature by encrypting the hash with the private key
    return mod_exp(message_hash, keypair.d, keypair.n);
}

// Function to verify the signature using the public key
int verifySignature(unsigned char *message, int message_len, unsigned long long signature, RSAKeyPair keypair) {
    unsigned char digest[20];
    sha1(message, message_len, digest); // Generate hash of the message

    unsigned long long message_hash = 0;
    for (int i = 0; i < 20; i++) {
        message_hash = (message_hash << 8) | digest[i]; // Convert hash to unsigned long long
    }

    // Decrypt the signature
    unsigned long long decrypted_signature = mod_exp(signature, keypair.e, keypair.n);
    return decrypted_signature == message_hash; // Verify
}

// Main function to demonstrate RSA signing and verification
int main() {
    RSAKeyPair keypair = generateRSAKeys();
    unsigned char message[] = "Hello, RSA!";
    int message_len = strlen((char *)message);

    // Sign the message
    unsigned long long signature = signMessage(message, message_len, keypair);
    printf("Signature: %llu\n", signature);

    // Verify the signature
    int is_valid = verifySignature(message, message_len, signature, keypair);
    if (is_valid) {
        printf("Signature is valid!\n");
    } else {
        printf("Signature is invalid!\n");
    }

    return 0;
}
