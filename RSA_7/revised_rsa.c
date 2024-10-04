#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Function to calculate the greatest common divisor (GCD)
long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

// Function to calculate the modular exponentiation
long long powerMod(long long base, long long exponent, long long mod) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        exponent = exponent / 2;
        base = (base * base) % mod;
    }
    return result;
}

// Function to calculate the modular multiplicative inverse
long long modInverse(long long a, long long m) {
    long long m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        long long q = a / m;
        long long t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) x += m0;
    return x;
}

// Function to perform Miller-Rabin primality test
int isPrime(long long n, int k) {
    if (n <= 1 || n == 4) return 0;
    if (n <= 3) return 1;

    long long d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < k; i++) {
        long long a = 2 + rand() % (n - 4);
        long long x = powerMod(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;

            if (x == 1) return 0;
            if (x == n - 1) break;
        }

        if (x != n - 1)
            return 0;
    }
    return 1;
}

// Function to generate a random prime number within a given range
long long generateRandomPrime(long long lower, long long upper) {
    long long prime;
    do {
        prime = lower + rand() % (upper - lower + 1);
    } while (!isPrime(prime, 5));  // 5 iterations of Miller-Rabin test
    return prime;
}

// Function to encrypt a number
long long encrypt(long long message, long long publicExponent, long long modulus) {
    return powerMod(message, publicExponent, modulus);
}

// Function to decrypt a number
long long decrypt(long long ciphertext, long long privateExponent, long long modulus) {
    return powerMod(ciphertext, privateExponent, modulus);
}

// Function to convert text to a numeric representation
void textToNumber(char *text, long long *numRepresentation, int *length) {
    *length = strlen(text);
    for (int i = 0; i < *length; i++) {
        numRepresentation[i] = (long long)text[i];  // Convert each character to its ASCII value
    }
}

// Function to convert numeric representation back to text
void numberToText(long long *numRepresentation, int length, char *text) {
    for (int i = 0; i < length; i++) {
        text[i] = (char)numRepresentation[i];  // Convert each number back to its ASCII character
    }
    text[length] = '\0';  // Null-terminate the string
}

// Main function
int main() {
    // Initialize random seed
    srand(time(0));

    // Generate random primes p and q
    long long p = generateRandomPrime(10000, 50000);
    long long q = generateRandomPrime(10000, 50000);
    printf("Generated prime p: %lld\n", p);
    printf("Generated prime q: %lld\n", q);

    // Calculate n and f(n)
    long long n = p * q;
    long long f_n = (p - 1) * (q - 1);

    // Take user input for public exponent e and check if it's valid
    long long e;
    do {
        printf("Enter public exponent e (1 < e < %lld): ", f_n);
        scanf("%lld", &e);
        if (gcd(e, f_n) != 1) {
            printf("Invalid e! gcd(e, φ(n)) must be 1. Try again.\n");
        }
    } while (gcd(e, f_n) != 1);

    // Calculate private exponent d
    long long d = modInverse(e, f_n);

    // Input message (string)
    char message[1024];
    printf("Enter the message: ");
    scanf(" %[^\n]s", message);  // Read the whole line of input

    // Convert message to numeric representation
    long long numMessage[1024];
    int length;
    textToNumber(message, numMessage, &length);

    // Encrypt the numeric representation
    long long encryptedMessage[1024];
    for (int i = 0; i < length; i++) {
        encryptedMessage[i] = encrypt(numMessage[i], e, n);
    }

    // Print encrypted message
    printf("Encrypted message: ");
    for (int i = 0; i < length; i++) {
        printf("%lld ", encryptedMessage[i]);
    }
    printf("\n");

    // Decrypt the encrypted message
    long long decryptedMessage[1024];
    for (int i = 0; i < length; i++) {
        decryptedMessage[i] = decrypt(encryptedMessage[i], d, n);
    }

    // Convert decrypted numeric representation back to text
    char decryptedText[1024];
    numberToText(decryptedMessage, length, decryptedText);

    // Print decrypted message
    printf("Decrypted message: %s\n", decryptedText);

    return 0;
}
