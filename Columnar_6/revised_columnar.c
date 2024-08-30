#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to determine the column order based on ASCII values of the key characters
void getColumnOrderASCII(char *key, int *order) {
    int keyLen = strlen(key);

    // Initialize the order array
    for (int i = 0; i < keyLen; i++) {
        order[i] = i;
    }

    // Sort the columns based on the ASCII values of the key characters
    for (int i = 0; i < keyLen - 1; i++) {
        for (int j = i + 1; j < keyLen; j++) {
            if ((int)key[order[i]] > (int)key[order[j]]) {
                int temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }
}

void columnarEncrypt(char *message, char *key, char *cipherText) {
    int msgLen = strlen(message);
    int keyLen = strlen(key);
    int col = keyLen;
    int row = (msgLen + col - 1) / col;

    // Allocate memory for the matrix
    char **matrix = (char **)malloc(row * sizeof(char *));
    for (int i = 0; i < row; i++) {
        matrix[i] = (char *)malloc(col * sizeof(char));
    }

    int k = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (k < msgLen)
                matrix[i][j] = message[k++];
            else
                matrix[i][j] = 'X'; // Padding character
        }
    }

    // Get the custom column order based on ASCII values
    int *order = (int *)malloc(keyLen * sizeof(int));
    getColumnOrderASCII(key, order);

    // Encrypt by reading the columns in the custom order
    k = 0;
    for (int i = 0; i < col; i++) {
        int column = order[i];
        for (int j = 0; j < row; j++) {
            cipherText[k++] = matrix[j][column];
        }
    }
    cipherText[k] = '\0';

    // Free allocated memory
    for (int i = 0; i < row; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(order);
}

void columnarDecrypt(char *cipherText, char *key, char *decryptedText, int originalLen) {
    int msgLen = strlen(cipherText);
    int keyLen = strlen(key);
    int col = keyLen;
    int row = (msgLen + col - 1) / col;

    // Allocate memory for the matrix
    char **matrix = (char **)malloc(row * sizeof(char *));
    for (int i = 0; i < row; i++) {
        matrix[i] = (char *)malloc(col * sizeof(char));
    }

    // Get the custom column order based on ASCII values
    int *order = (int *)malloc(keyLen * sizeof(int));
    getColumnOrderASCII(key, order);

    // Fill the matrix with the ciphertext based on the custom column order
    int k = 0;
    for (int i = 0; i < col; i++) {
        int column = order[i];
        for (int j = 0; j < row; j++) {
            matrix[j][column] = cipherText[k++];
        }
    }

    // Decrypt by reading the matrix row by row
    k = 0;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            decryptedText[k++] = matrix[i][j];
        }
    }
    decryptedText[originalLen] = '\0';

    // Free allocated memory
    for (int i = 0; i < row; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(order);
}

void doubleTranspositionEncrypt(char *message, char *key1, char *key2, char *cipherText) {
    char intermediateText[100];
    columnarEncrypt(message, key1, intermediateText);  // First transposition
    columnarEncrypt(intermediateText, key2, cipherText); // Second transposition
}

void doubleTranspositionDecrypt(char *cipherText, char *key1, char *key2, char *decryptedText, int originalLen) {
    char intermediateText[100];
    columnarDecrypt(cipherText, key2, intermediateText, originalLen); // Reverse second transposition
    columnarDecrypt(intermediateText, key1, decryptedText, originalLen); // Reverse first transposition
}

int main() {
    char message[100];
    char key1[100];
    char key2[100];
    char cipherText[100];
    char decryptedText[100];

    printf("Enter the message to be encrypted: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    printf("Enter the first key: ");
    fgets(key1, sizeof(key1), stdin);
    key1[strcspn(key1, "\n")] = '\0';

    printf("Enter the second key: ");
    fgets(key2, sizeof(key2), stdin);
    key2[strcspn(key2, "\n")] = '\0';

    int originalLen = strlen(message);

    doubleTranspositionEncrypt(message, key1, key2, cipherText);
    printf("Encrypted message: %s\n", cipherText);

    doubleTranspositionDecrypt(cipherText, key1, key2, decryptedText, originalLen);
    printf("Decrypted message: %s\n", decryptedText);

    return 0;
}
