#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char plaintext[20],ciphertext[20],key[20],decrypted[20];
    printf("Enter plaintext : ");
    fgets(plaintext,sizeof(plaintext),stdin);
    plaintext[strcspn(plaintext,"\n")] = '\0';
    int orglen = strlen(plaintext);

    printf("Enter key: ");
    fgets(key,sizeof(key),stdin);
    key[strcspn(key,"\n")] = '\0';

    // encryption
    int msglen = strlen(plaintext);
    int keylen = strlen(key);
    int col = keylen;
    int row = (msglen + col - 1)/col;
    char matrix[row][col];
    // fill matrix
    int k=0;
    for(int i=0; i<row;i++){
        for(int j=0;j<col;j++){
            if(k<msglen){
                matrix[i][j] = plaintext[k++];
            }
            else{
                matrix[i][j] = 'x';
            }
        }
    }

    // sorting of key
    int sorted_key[keylen];
    for(int i=0;i<keylen;i++){
        sorted_key[i] = i;
    }
    for(int i=0;i<keylen;i++){
        for(int j=i+1;j<keylen;j++){
            if(key[sorted_key[i]] > key[sorted_key[j]]){
                int temp = sorted_key[i];
                sorted_key[i] = sorted_key[j];
                sorted_key[j] = temp;
            }
        }
    }

    // making ciphertext
    k=0;
    for(int i=0;i<col;i++){
        int column = sorted_key[i];
        for(int j=0;j<row;j++){
            ciphertext[k++] = matrix[j][column];
        }
    }
    printf("Ciphertext : %s\n" , ciphertext);

    //decryption
    msglen = strlen(ciphertext);
    keylen = strlen(key);
    col = keylen;
    row = (msglen + col - 1)/col;

    // sorting of key
    for(int i=0;i<keylen;i++){
        sorted_key[i] = i;
    }
    for(int i=0;i<keylen;i++){
        for(int j=i+1;j<keylen;j++){
            if(key[sorted_key[i]] > key[sorted_key[j]]){
                int temp = sorted_key[i];
                sorted_key[i] = sorted_key[j];
                sorted_key[j] = temp;
            }
        }
    }

    k=0;
    for(int i=0;i<col;i++){
        int column = sorted_key[i];
        for(int j=0;j<row;j++){
            matrix[j][column] = ciphertext[k++];
        }
    }

    k=0;
    for(int i=0; i<row;i++){
        for(int j=0;j<col;j++){
            decrypted[k++] = matrix[i][j];
        }
    }
    decrypted[orglen] = '\0';
    printf("Decrypted : %s\n" , decrypted);
}