#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void remove_duplicates(char *str) {
    int len = strlen(str);
    int index = 0;
    for (int i = 0; i < len; i++) {
        int j;
        for (j = 0; j < index; j++) {
            if (str[i] == str[j])
                break;
        }
        if (j == index)
            str[index++] = str[i];
    }
    str[index] = '\0';
}

void fill_matrix(char *keyword, int matrix[5][5]) {
    int used[26] = {0}; // Track used characters
    int k = 0;

    for (int i = 0; keyword[i]; i++) {
        char ch = keyword[i];
        if (ch == 'J')
            ch = 'I'; // Treat 'J' as 'I'
        if (!used[ch - 'A']) {
            used[ch - 'A'] = 1;
            matrix[k / 5][k % 5] = ch;
            k++;
        }
    }

    for (char ch = 'A'; ch <= 'Z'; ch++) {
        if (ch == 'J')
            continue;
        if (!used[ch - 'A']) {
            matrix[k / 5][k % 5] = ch;
            k++;
        }
    }
}

void paircheck(char *plaintext) {
    int len = strlen(plaintext);
    // Convert all characters to uppercase
    for (int i = 0; i < len; i++) {
        plaintext[i] = toupper(plaintext[i]);
        if (plaintext[i] == 'J')
            plaintext[i] = 'I';
    }

    // Check for pairs of identical characters and insert 'X'
    for (int i = 0; i < len - 1; i++) {
        if (plaintext[i] == plaintext[i + 1]) {
            // Shift characters to the right to make space for 'X'
            for (int j = len; j > i + 1; j--) {
                plaintext[j + 1] = plaintext[j];
            }
            plaintext[i + 1] = 'X';
            len++;
        }
    }

    // Add 'X' if the plaintext length is odd
    if (len % 2 != 0) {
        plaintext[len] = 'X';
        plaintext[len + 1] = '\0';
    } else {
        plaintext[len] = '\0';
    }
}

void encrypt(char a, char b, int matrix[5][5], char *ciphertext, int cipher_index) {
    int row1, col1, row2, col2;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == a) {
                row1 = i;
                col1 = j;
            }
            if (matrix[i][j] == b) {
                row2 = i;
                col2 = j;
            }
        }
    }

    if (row1 == row2) {
        // Move right in the same row
        ciphertext[cipher_index] = matrix[row1][(col1 + 1) % 5];
        ciphertext[cipher_index + 1] = matrix[row2][(col2 + 1) % 5];
    } else if (col1 == col2) {
        // Move down in the same column
        ciphertext[cipher_index] = matrix[(row1 + 1) % 5][col1];
        ciphertext[cipher_index + 1] = matrix[(row2 + 1) % 5][col2];
    } else {
        // Rectangle case: swap columns
        ciphertext[cipher_index] = matrix[row1][col2];
        ciphertext[cipher_index + 1] = matrix[row2][col1];
    }
}

void decrypt(char a, char b, int matrix[5][5], char *plaintext, int plain_index) {
    int row1, col1, row2, col2;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == a) {
                row1 = i;
                col1 = j;
            }
            if (matrix[i][j] == b) {
                row2 = i;
                col2 = j;
            }
        }
    }

    if (row1 == row2) {
        // Move left in the same row
        plaintext[plain_index] = matrix[row1][(col1 + 4) % 5];
        plaintext[plain_index + 1] = matrix[row2][(col2 + 4) % 5];
    } else if (col1 == col2) {
        // Move up in the same column
        plaintext[plain_index] = matrix[(row1 + 4) % 5][col1];
        plaintext[plain_index + 1] = matrix[(row2 + 4) % 5][col2];
    } else {
        // Rectangle case: swap columns
        plaintext[plain_index] = matrix[row1][col2];
        plaintext[plain_index + 1] = matrix[row2][col1];
    }
}

int main() {
    char keyword[100], plaintext[100], ciphertext[100], decryptedtext[100];
    int matrix[5][5];

    // Take input of keyword and plaintext
    printf("Enter keyword: ");
    scanf("%s", keyword);
    printf("Enter Plaintext: ");
    scanf("%s", plaintext);

    // Remove duplicates and process keyword
    remove_duplicates(keyword);
    for (int i = 0; keyword[i]; i++)
        keyword[i] = toupper(keyword[i]);

    // Fill matrix
    fill_matrix(keyword, matrix);

    // Process plaintext
    paircheck(plaintext);
    int len = strlen(plaintext);

    // Encrypt plaintext
    int cipher_index = 0;
    for (int i = 0; i < len; i += 2) {
        encrypt(plaintext[i], plaintext[i + 1], matrix, ciphertext, cipher_index);
        cipher_index += 2;
    }
    ciphertext[cipher_index] = '\0';

    // Print ciphertext
    printf("Ciphertext: %s\n", ciphertext);

    // Decrypt ciphertext
    int plain_index = 0;
    for (int i = 0; i < cipher_index; i += 2) {
        decrypt(ciphertext[i], ciphertext[i + 1], matrix, decryptedtext, plain_index);
        plain_index += 2;
    }
    decryptedtext[plain_index] = '\0';

    // Print decrypted text
    printf("Decrypted text: %s\n", decryptedtext);

    

    return 0;
}
