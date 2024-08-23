#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main() {
    int key;
    char msg[30], ciphertext[20][20], decryptedtext[30];
    memset(ciphertext, 0, sizeof(ciphertext));
    memset(decryptedtext, 0, sizeof(decryptedtext));

    printf("Enter message: ");
    fgets(msg, sizeof(msg), stdin);

    // Remove the newline character if present
    msg[strcspn(msg, "\n")] = 0;

    printf("Enter key: ");
    scanf("%d", &key);

    int row = 0;
    int direction = 1;  // 1 for moving down, -1 for moving up

    // Encryption
    for (int i = 0; i < strlen(msg); i++) {
        // If space, replace with 'Z'
        if (msg[i] == ' ') {
            ciphertext[row][i] = 'Z';
        } else {
            ciphertext[row][i] = msg[i];
        }

        // Change direction when we reach the top or bottom rail
        if (row == 0) {
            direction = 1;
        } else if (row == key - 1) {
            direction = -1;
        }

        // Move to the next row
        row += direction;
    }

    // Print the ciphertext by reading the rows in order
    printf("Encrypted message: ");
    for (int i = 0; i < key; i++) {
        for (int j = 0; j < strlen(msg); j++) {
            if (ciphertext[i][j] != 0) {  
                printf("%c", ciphertext[i][j]);
            }
        }
    }
    printf("\n");

     // Decryption
    row = 0;
    direction = 1;  // 1 for moving down, -1 for moving up
    for (int i = 0; i < strlen(msg); i++) {
        if(ciphertext[row][i] == 'Z'){
            decryptedtext[i] = ' ';
        }
        else{
            decryptedtext[i] = ciphertext[row][i];
        }

        // Change direction when we reach the top or bottom rail
        if (row == 0) {
            direction = 1;
        } else if (row == key - 1) {
            direction = -1;
        }

        // Move to the next row
        row += direction;
    }
    
    // Null-terminate the decrypted string
    decryptedtext[strlen(msg)] = '\0';

    printf("Decrypted message is: %s\n", decryptedtext);

    return 0;
}
