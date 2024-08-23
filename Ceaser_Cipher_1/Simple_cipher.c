#include <stdio.h>
#include <string.h>

void encrypt(char plain[], int key, char cipher[]) {
    int i;
    for (i = 0; plain[i] != '\0'; i++) {
        char c = plain[i];
        
        if (c >= 'A' && c <= 'Z') {
            c = (c - 'A' + key) % 26 + 'A';
        }
        else if (c >= 'a' && c <= 'z') {
            c = (c - 'a' + key) % 26 + 'a';
        }
        
        cipher[i] = c;
    }
    cipher[i] = '\0';
}

void decrypt(char cipher[], int key , char plain[]) {
    int i;
    for (i = 0; cipher[i] != '\0'; i++) {
        char c = cipher[i];
        
        if (c >= 'A' && c <= 'Z') {
            c = (c - 'A' - key + 26) % 26 + 'A';
        }
        else if (c >= 'a' && c <= 'z') {
            c = (c - 'a' - key + 26) % 26 + 'a';
        }
        
        plain[i] = c;
    }
    plain[i] = '\0';
}

int main() {
    char plain[100];
    char cipher[100];
    int key=3;
    
    printf("Enter the plaintext: ");
    fgets(plain, sizeof(plain), stdin);
    
    // Encrypt 
    encrypt(plain, key, cipher);
    printf("Encrypted text: %s\n", cipher);
    
    // Decrypt 
    decrypt(cipher, key, plain);
    printf("Decrypted text: %s\n", plain);
    printf("This code is done by 22BCP171");
    return 0;
}
