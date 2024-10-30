#include<stdio.h>
#include<string.h>

int main(){
    char plaintext[100], ciphertext[100];
    int i = 0, word_index = 1, letter_index = 0;

    // Input the plaintext
    printf("Enter plaintext (in capital letters, up to nine words): ");
    fgets(plaintext, sizeof(plaintext), stdin);

    // Remove newline character at the end if it's there
    if(plaintext[strlen(plaintext) - 1] == '\n')
        plaintext[strlen(plaintext) - 1] = '\0';

    // Encrypt each letter in the plaintext
    printf("\nEncryption process:\n");
    while(plaintext[i] != '\0') {
        if(plaintext[i] == ' ') {
            // Space indicates new word, reset letter index and increment word index
            ciphertext[i] = ' ';  // Keep spaces in ciphertext
            printf("Space detected, move to next word.\n");
            word_index++;
            letter_index = 0;
        } else {
            // Calculate the shift (i + j)
            int shift = word_index + letter_index;
            printf("Plaintext letter: %c, Word index: %d, Letter index: %d, Key used: %d\n", plaintext[i], word_index, letter_index, shift);

            // Encrypt the letter
            ciphertext[i] = ((plaintext[i] - 'A' + shift) % 26) + 'A';
            letter_index++;
        }
        i++;
    }

    // Null-terminate the ciphertext
    ciphertext[i] = '\0';

    // Output the ciphertext
    printf("\nCiphertext: %s\n", ciphertext);

    return 0;
}
