#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void generate_random_key(char* key, int length) {
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length; i++) {
        key[i] = alphabet[rand() % 26]; // Select a random character from the alphabet
    }
    key[length] = '\0'; // Null-terminate the key string
}

int main(){
    char key[100],plaintext[100],ciphertext[100];
    int key_len;

    printf("Enter key number: ");
    scanf("%d" , &key_len);
    printf("Enter plain text : ");
    scanf("%s" , plaintext);

    generate_random_key(key,key_len);
    printf("Key generated : %s\n" , key);
    // Encrypt
    int key_index=0;
    for(int i=0 ; i<strlen(plaintext) ; i++){
        ciphertext[i] = (plaintext[i]-'a' + key[key_index] - 'a') + 'a';
        if(key_index < strlen(key)){
            key_index++;
        }
        if(key_index >= strlen(key)){
            key_index=0;
        }
    }
    printf("Encrypted text : %s\n" , ciphertext);
    ciphertext[strlen(plaintext)] = '\0';
    
    // Decrypt
    key_index=0;
    char decryptedtext[100];
    for(int i=0 ; i<strlen(ciphertext) ; i++){
        decryptedtext[i] = ((ciphertext[i]-'a') - (key[key_index] - 'a'))%26 + 'a';
        if(key_index < strlen(key)){
            key_index++;
        }
        if(key_index >= strlen(key)){
            key_index=0;
        }
    }
    decryptedtext[strlen(ciphertext)] = '\0';
    printf("Decrypted text : %s " , decryptedtext);
}