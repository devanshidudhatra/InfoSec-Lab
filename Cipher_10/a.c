#include<stdio.h>
#include<string.h>

int main(){
    char key[100], plaintext[100], ciphertext[100];
    
    // Get the key and plaintext as input
    printf("Enter key: ");
    scanf("%s", key);
    printf("Enter plaintext (in capital letters): ");
    scanf("%s", plaintext);
    
    int len_plain = strlen(plaintext);
    int len_key = strlen(key);
    int min_char = plaintext[0];

    // Find the smallest letter from the plaintext
    for(int i = 1; i < len_plain; i++) {
        if(plaintext[i] < min_char) {
            min_char = plaintext[i];
        }
    }

    // Calculate how many filler characters we need
    int remainder = len_plain % len_key;
    int fill_count = (remainder == 0) ? 0 : (len_key - remainder);

    // Add filler characters (the smallest letter)
    for(int i = 0; i < fill_count; i++) {
        plaintext[len_plain + i] = min_char;
    }
    
    // Update the new length of the plaintext after adding fillers
    len_plain += fill_count;
    plaintext[len_plain] = '\0';  // Null-terminate the extended plaintext
    
    // Encrypt the plaintext using the key
    int key_index = 0;
    for(int i = 0; i < len_plain; i++) {
        int shift = key[key_index] - '0';  // Convert key character to integer
        ciphertext[i] = ((plaintext[i] - 'A' + shift) % 26) + 'A';  // Shift and wrap around alphabet
        
        key_index = (key_index + 1) % len_key;  // Move to the next key character
    }
    ciphertext[len_plain] = '\0';  // Null-terminate the ciphertext
    
    // Output the encrypted message
    printf("Encrypted text: %s\n", ciphertext);
    
    return 0;
}
