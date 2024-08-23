#include <stdio.h>
#include <string.h>

void encrypt(const char plain[], int key, char cipher[]) {
    int i, j;
    int cipherIndex = 0;

    for (i = 0, j = 0; plain[i] != '\0'; i++, j++) {
        char c = plain[i];
        int shift = key;

        //LEft shift
        if (c >= 'A' && c <= 'Z') {
            cipher[cipherIndex++] = (c - 'A' + shift) % 26 + 'A';
        } else if (c >= 'a' && c <= 'z') {
            cipher[cipherIndex++] = (c - 'a' + shift) % 26 + 'a';
        }

        // right shift
        if (c >= 'A' && c <= 'Z') {
            cipher[cipherIndex++] = (c - 'A' - shift + 26) % 26 + 'A';
        } else if (c >= 'a' && c <= 'z') {
            cipher[cipherIndex++] = (c - 'a' - shift + 26) % 26 + 'a';
        }
    }
    cipher[cipherIndex] = '\0';
}

// Function to decrypt the ciphertext
void decrypt(const char cipher[], int key, char plain[]) {
    int i, j;
    int plainIndex = 0;

    for (i = 0, j = 0; cipher[i] != '\0'; i += 2, j++) {
        int leftShift = key;
        char leftChar = cipher[i];
        char rightChar = cipher[i + 1];

        // Decrypt left shift
        if (leftChar >= 'A' && leftChar <= 'Z') {
            leftChar = (leftChar - 'A' - leftShift + 26) % 26 + 'A';
        } else if (leftChar >= 'a' && leftChar <= 'z') {
            leftChar = (leftChar - 'a' - leftShift + 26) % 26 + 'a';
        }

        // Decrypt right shift
        if (rightChar >= 'A' && rightChar <= 'Z') {
            rightChar = (rightChar - 'A' + leftShift) % 26 + 'A';
        } else if (rightChar >= 'a' && rightChar <= 'z') {
            rightChar = (rightChar - 'a' + leftShift) % 26 + 'a';
        }

        // Ensure both decrypted characters are same
        if (leftChar == rightChar) {
            plain[plainIndex++] = leftChar;
        } else {
            printf("Decryption error at index %d\n", i / 2);
            return;
        }
    }
    plain[plainIndex] = '\0';
}

int main() {
    char plain[100];
    char cipher[200]; 
    int key;
    
    printf("Enter the plaintext: ");
    fgets(plain, sizeof(plain), stdin);
    plain[strcspn(plain, "\n")] = '\0';

    printf("Enter the key: ");
    scanf("%d", &key);
    
    // Encrypt
    encrypt(plain, key, cipher);
    printf("Encrypted text: %s\n", cipher);
    
    // Decrypt
    decrypt(cipher, key, plain);
    printf("Decrypted text: %s\n", plain);
    printf("This code is done by 22BCP171");
    return 0;
}
