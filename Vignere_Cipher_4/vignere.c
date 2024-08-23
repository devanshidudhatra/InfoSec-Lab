#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char key[100],plaintext[100],ciphertext[100];

    printf("Enter key: ");
    scanf("%s" , key);
    printf("Enter plain text : ");
    scanf("%s" , plaintext);

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