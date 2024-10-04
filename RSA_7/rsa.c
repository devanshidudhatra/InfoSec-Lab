#include <stdio.h>

// Function to calculate the modular exponentiation
// This function calculates the result of (base^exponent) % mod
long long powerMod(long long base, long long exponent, long long mod) {
    // Initialize the result to 1
    long long result = 1;
    
    // Continue the loop until the exponent is greater than 0
    while (exponent > 0) {
        // If the exponent is odd, multiply the result by the base
        if (exponent % 2 == 1)
            // Use the modulo operator to prevent overflow
            result = (result * base) % mod;
        
        // Divide the exponent by 2 for the next iteration
        exponent = exponent / 2;
        
        // Square the base for the next iteration
        base = (base * base) % mod;
    }
    
    // Return the final result
    return result;
}

// Function to calculate the modular multiplicative inverse
// This function calculates the modular multiplicative inverse of a modulo m
long long modInverse(long long a, long long m) {
    // Store the original value of m
    long long m0 = m;
    
    // Initialize the variables for the extended Euclidean algorithm
    long long y = 0, x = 1;
    
    // If m is 1, return 0
    if (m == 1)
        return 0;
    
    // Continue the loop until a is greater than 1
    while (a > 1) {
        // Calculate the quotient of a divided by m
        long long q = a / m;
        
        // Store the value of m in a temporary variable
        long long t = m;
        
        // Update the value of m to be the remainder of a divided by m
        m = a % m;
        
        // Update the value of a to be the original value of m
        a = t;
        
        // Update the values of t, y, and x for the next iteration
        t = y;
        y = x - q * y;
        x = t;
    }
    
    // If x is negative, add m0 to it to make it positive
    if (x < 0)
        x = x + m0;
    
    // Return the final result
    return x;
}

// Function to encrypt the message
// This function encrypts the message using the public key (e, n)
long long encrypt(long long message, long long publicExponent, long long modulus) {
    // Use the powerMod function to calculate the ciphertext
    return powerMod(message, publicExponent, modulus);
}

// Function to decrypt the message
// This function decrypts the ciphertext using the private key (d, n)
long long decrypt(long long ciphertext, long long privateExponent, long long modulus) {
    // Use the powerMod function to calculate the decrypted message
    return powerMod(ciphertext, privateExponent, modulus);
}

// Main function
int main() {
    // Take user input for prime numbers p and q
    long long p, q;
    // Prompt the user to enter the value of p
    printf("Enter prime number p: ");
    // Read the value of p from the user
    scanf("%lld", &p);
    
    // Prompt the user to enter the value of q
    printf("Enter prime number q: ");
    // Read the value of q from the user
    scanf("%lld", &q);

    // Calculate n and f(n)
    // Calculate the value of n as the product of p and q
    long long n = p * q;
    // Calculate the value of f(n) as (p-1)*(q-1)
    long long f_n = (p - 1) * (q - 1);

    // Take user input for public exponent e
    long long e;
    // Prompt the user to enter the value of e
    printf("Enter public exponent e (1 < e < %lld): ", f_n);
    // Read the value of e from the user
    scanf("%lld", &e);

    // Calculate private exponent d
    // Use the modInverse function to calculate the modular multiplicative inverse of e modulo f_n
    long long d = modInverse(e, f_n);

    // Take user input for message M
    long long M;
    // Prompt the user to enter the value of M
    printf("Enter message M (0 <= M < %lld): ", n);
    // Read the value of M from the user
    scanf("%lld", &M);

    // Public key: {e, n}
    // Private key: {d, n}

    // Encrypt the message
    long long ciphertext = encrypt(M, e, n);
    printf("Ciphertext: %lld\n", ciphertext);

    // Decrypt the ciphertext
    long long decryptedMessage = decrypt(ciphertext, d, n);
    printf("Decrypted Message: %lld\n", decryptedMessage);

    return 0;
}