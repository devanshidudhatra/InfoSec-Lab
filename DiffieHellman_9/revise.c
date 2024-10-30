#include <stdio.h>
#include <stdlib.h>

// Linear Congruential Generator (LCG) parameters
#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32
#define P  32768 // 2^15

unsigned long long seed = 0;

// Function to initialize the random number generator
void initRandom() {
    seed = (rand() % (P- 1)) + 1;
}

// Function to generate a random number between 1 and p-1
long long generateRandom(long long p) {
    // Update the seed using LCG formula
    initRandom();
    seed = (A * seed + C) % M;
    
    // Scale the output to the range [1, p-1]
    return (seed % (p - 1)) + 1;
}

// Function to calculate (a^b) % c
long long powerMod(long long a , long long b, long long c) {
    long long res = 1;
    while (b > 0) {
        if (b & 1)
            res = (res * a) % c;
        a = (a * a) % c;
        b >>= 1;
    }
    return res;
}

int main() {
    long long p, g, R1, R2, x, y, ka, kb;

    // Both the persons will be agreed upon the public keys G and P
    p = 23; // A prime number P is taken
    g = 9; // A primitive root for P, G is taken

    // Alice will choose the private key a
    x = generateRandom(p);
    R1 = powerMod(g, x, p);

    // Bob will choose the private key b
    y = generateRandom(p);
    R2 = powerMod(g, y, p);

    // Generating the secret key after the exchange of keys
    ka = powerMod(R2, x, p);
    kb = powerMod(R1, y, p);

    printf("The value of P : %lld\n", p);
    printf("The value of G : %lld\n", g);
    printf("The private key x for Alice : %lld\n", x);
    printf("The private key y for Bob : %lld\n", y);
    printf("The public key R1 for Alice : %lld\n", R1);
    printf("The public key R2 for Bob : %lld\n", R2);
    printf("Secret key for the Alice is : %lld\n", ka);
    printf("Secret key for the Bob is : %lld\n", kb);

    return 0;
}