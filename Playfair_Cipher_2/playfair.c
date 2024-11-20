#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define SIZE 5

void createMatrix(char *key, char matrix[SIZE][SIZE]) {
    int c[26] = {0}; // To track used letters
    int x = 0;

    for (int i = 0; key[i]; i++) {
        char ch = tolower(key[i]);
        if (ch == 'j') ch = 'i'; // Replace J with I
        if (isalpha(ch) && !c[ch - 'a']) {
            c[ch - 'a'] = 1;
            matrix[x / SIZE][x % SIZE] = ch;
            x++;
        }
    }

    for (char ch = 'a'; ch <= 'z'; ch++) {
        if (ch == 'j') continue; // Skip J
        if (!c[ch - 'a']) {
            matrix[x / SIZE][x % SIZE] = ch;
            x++;
        }
    }
}

void findPosition(char ch, char matrix[SIZE][SIZE], int *row, int *col) {
    if (ch == 'j') ch = 'i'; // Treat J as I
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == ch) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void playfairEncrypt(char *plaintext, char matrix[SIZE][SIZE], char *ciphertext) {
    int len = strlen(plaintext);
    int k = 0;

    for (int i = 0; i < len; i += 2) {
        char a = tolower(plaintext[i]);
        char b = (i + 1 < len) ? tolower(plaintext[i + 1]) : 'x';

        if (a == b) {
            b = 'x'; // Insert 'x' if letters are the same
            i--;
        }

        int rowA, colA, rowB, colB;
        findPosition(a, matrix, &rowA, &colA);
        findPosition(b, matrix, &rowB, &colB);

        if (rowA == rowB) { // Same row
            ciphertext[k++] = matrix[rowA][(colA + 1) % SIZE];
            ciphertext[k++] = matrix[rowB][(colB + 1) % SIZE];
        } else if (colA == colB) { // Same column
            ciphertext[k++] = matrix[(rowA + 1) % SIZE][colA];
            ciphertext[k++] = matrix[(rowB + 1) % SIZE][colB];
        } else { // Rectangle
            ciphertext[k++] = matrix[rowA][colB];
            ciphertext[k++] = matrix[rowB][colA];
        }
    }
    ciphertext[k] = '\0';
}

void playfairDecrypt(char *ciphertext, char matrix[SIZE][SIZE], char *plaintext) {
    int len = strlen(ciphertext);
    int k = 0;

    for (int i = 0; i < len; i += 2) {
        char a = ciphertext[i];
        char b = ciphertext[i + 1];

        int rowA, colA, rowB, colB;
        findPosition(a, matrix, &rowA, &colA);
        findPosition(b, matrix, &rowB, &colB);

        if (rowA == rowB) { // Same row
            plaintext[k++] = matrix[rowA][(colA + 4) % SIZE];
            plaintext[k++] = matrix[rowB][(colB + 4) % SIZE];
        } else if (colA == colB) { // Same column
            plaintext[k++] = matrix[(rowA + 4) % SIZE][colA];
            plaintext[k++] = matrix[(rowB + 4) % SIZE][colB];
        } else { // Rectangle
            plaintext[k++] = matrix[rowA][colB];
            plaintext[k++] = matrix[rowB][colA];
        }
    }
    plaintext[k] = '\0';

    // Remove 'x' if it was added as a filler
    char finalText[100];
    k = 0;
    for (int i = 0; i < strlen(plaintext); i++) {
        if (plaintext[i] == 'x' && (i == strlen(plaintext)-1 || plaintext[i - 1] == plaintext[i + 1])) {
            continue;
    }
        finalText[k++] = plaintext[i];
    }
    finalText[k] = '\0';
    strcpy(plaintext, finalText);
}

int main() {
    char key[100], plaintext[100], ciphertext[100];
    char matrix[SIZE][SIZE];

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0; // Remove newline character

    printf("Enter the plain text: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = 0; // Remove newline character

    createMatrix(key, matrix);
    playfairEncrypt(plaintext, matrix, ciphertext);
    printf("Encrypted text: %s\n", ciphertext);

    char decryptedtext[100];
    playfairDecrypt(ciphertext, matrix, decryptedtext);
    printf("Decrypted text: %s\n", decryptedtext);

    return 0;
}