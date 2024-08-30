#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void encrypt(char *message, char *key, char *cipherText) {
    int msgLen = strlen(message);
    int keyLen = strlen(key);
    int col = keyLen;
    int row = (msgLen + col - 1) / col; // Calculate the number of rows needed

    // Fill the matrix with the message, including spaces
    char matrix[row][col];
    int k = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (k < msgLen)
                matrix[i][j] = message[k++];
            else
                matrix[i][j] = 'X'; // Padding character if message is shorter than the matrix
        }
    }

    // Sorting the key to determine the order of columns
    int sortedKey[keyLen];
    for (int i = 0; i < keyLen; i++) {
        sortedKey[i] = i;
    }

    // Sorting the columns based on the key
    for (int i = 0; i < keyLen - 1; i++) {
        for (int j = i + 1; j < keyLen; j++) {
            if (key[sortedKey[i]] > key[sortedKey[j]]) {
                int temp = sortedKey[i];
                sortedKey[i] = sortedKey[j];
                sortedKey[j] = temp;
            }
        }
    }

    // Generate the ciphertext by reading the columns in the order of the sorted key
    k = 0;
    for (int i = 0; i < col; i++) {
        int column = sortedKey[i];
        for (int j = 0; j < row; j++) {
            cipherText[k++] = matrix[j][column];
        }
    }
    cipherText[k] = '\0';
}

void decrypt(char *cipherText, char *key, char *decryptedText, int originalLen) {
    int msgLen = strlen(cipherText);
    int keyLen = strlen(key);
    int col = keyLen;
    int row = (msgLen + col - 1) / col; // Calculate the number of rows needed

    // Sorting the key to determine the order of columns
    int sortedKey[keyLen];
    for (int i = 0; i < keyLen; i++) {
        sortedKey[i] = i;
    }

    // Sorting the columns based on the key
    for (int i = 0; i < keyLen - 1; i++) {
        for (int j = i + 1; j < keyLen; j++) {
            if (key[sortedKey[i]] > key[sortedKey[j]]) {
                int temp = sortedKey[i];
                sortedKey[i] = sortedKey[j];
                sortedKey[j] = temp;
            }
        }
    }

    // Fill the matrix with the cipherText
    char matrix[row][col];
    int k = 0;
    for (int i = 0; i < col; i++) {
        int column = sortedKey[i];
        for (int j = 0; j < row; j++) {
            matrix[j][column] = cipherText[k++];
        }
    }

    // Generate the decrypted text by reading the matrix row by row
    k = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            decryptedText[k++] = matrix[i][j];
        }
    }
    decryptedText[originalLen] = '\0'; // Truncate the decrypted text to the original length
}

int main() {
    char message[100];
    char key[100];
    char cipherText[100];
    char decryptedText[100];

    printf("Enter the message to be encrypted: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';  // Remove trailing newline

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';  // Remove trailing newline

    int originalLen = strlen(message);

    encrypt(message, key, cipherText);
    printf("Encrypted message: %s\n", cipherText);

    decrypt(cipherText, key, decryptedText, originalLen);
    printf("Decrypted message: %s\n", decryptedText);

    return 0;
}
