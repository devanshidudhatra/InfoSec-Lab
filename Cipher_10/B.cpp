#include <bits/stdc++.h>
using namespace std;

int main()
{
    string plaintext;
    cout << "Enter the plaintext: ";
    getline(cin, plaintext);

    // Constructing the key with the given logic
    int len = plaintext.size();
    int word_cnt = 1; // Count of words in the plaintext
    int start = 1;
    string key = "";
    for (int i = 0; i < len; i++)
    {
        char ch = plaintext[i];
        if (ch == ' ') // Updates the word count and the start position of key
        {
            word_cnt++;
            start = word_cnt;
            key += " ";
        }

        else // Adds the start to the key
        {
            key += (start + '0');
            start++;
        }
    }
    cout << "Key: " << key << endl;

    //  Encrypting the plaintext (similar to vigenere cipher)
    string encryptedtext = "";
    int x = 0;
    for (int i = 0; i < len; i++)
    {
        char ch = plaintext[i];
        if (ch == ' ') // Adding the space to ciphertext
        {
            encryptedtext += " ";
            x++;
            continue;
        }

        encryptedtext += ((plaintext[i] - 'A') + (key[x] - '0')) % 26 + 'A';
        x++;
    }
    cout << "Encrypted Text: " << encryptedtext << endl;
}