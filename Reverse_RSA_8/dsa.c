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

// Function to calculate SHA-1 hash (same as before)
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
            w[j] = (w[j] << 1) | (w[j] >> 31);
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
            c = b + (k << 5 | k >> 27);
            b = b + k;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

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

// Other helper functions (gcd, modinv, find_e) remain the same
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
    printf("Enter the Original Message (M): ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0;

    // RSA Key Generation
    int prime_1, prime_2, n, phi, public_key_A, private_key_A;
    printf("Enter the first Prime Number: ");
    scanf("%d", &prime_1);
    printf("Enter the second Prime Number: ");
    scanf("%d", &prime_2);

    // Check if the numbers are prime using the is_prime function
    if (!is_prime(prime_1) || !is_prime(prime_2)) {
        printf("Please use valid prime numbers.\n");
        return 1;
    }

    n = prime_1 * prime_2;
    phi = (prime_1 - 1) * (prime_2 - 1);

    // Find public and private keys for A
    public_key_A = find_e(phi);
    private_key_A = modinv(public_key_A, phi);

    // Step 1: A calculates MD1 (SHA-1 of the message)
    unsigned char digest_A[20];
    sha1((unsigned char *)message, strlen(message), digest_A);

    printf("Message Digest (MD1): ");
    for (int i = 0; i < 20; i++) {
        printf("%02x", digest_A[i]);
    }
    printf("\n");

    // Step 2: A encrypts MD1 with private key to create DS
    int digital_signature_A[20];
    encrypt_RSA((char *)digest_A, private_key_A, n, digital_signature_A);
    printf("Digital Signature (DS): ");
    for (int i = 0; i < 20; i++) {
        printf("%d ", digital_signature_A[i]);
    }
    printf("\n");

    // Step 3: B decrypts the received DS with A's public key
    unsigned char decrypted_digest_A[20];
    decrypt_RSA(digital_signature_A, public_key_A, n, (char *)decrypted_digest_A, 20);
    printf("Decrypted Message Digest (MD1) from DS: ");
    for (int i = 0; i < 20; i++) {
        printf("%02x", decrypted_digest_A[i]);
    }
    printf("\n");

    // Step 4: B calculates MD2 (SHA-1 of the received message)
    unsigned char digest_B[20];
    sha1((unsigned char *)message, strlen(message), digest_B);
    printf("Message Digest (MD2): ");
    for (int i = 0; i < 20; i++) {
        printf("%02x", digest_B[i]);
    }
    printf("\n");

    // Step 5: Compare MD1 and MD2
    if (memcmp(decrypted_digest_A, digest_B, 20) == 0) {
        printf("Message integrity verified. The message has not been altered.\n");
    } else {
        printf("Message integrity verification failed. The message may have been altered.\n");
    }

    return 0;
}
