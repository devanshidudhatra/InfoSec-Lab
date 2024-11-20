#include <bits/stdc++.h>
using namespace std;

int main()
{
    string plaintext;
    cout << "Enter the plaintext: ";
    cin >> plaintext;

    string key;
    cout << "Enter the key: ";
    cin >> key;

    int len = plaintext.size();
    int key_len = key.size();

    // Finding the minimum character
    char min = 'Z';
    for (int i = 0; i < len; i++)
    {
        if (plaintext[i] < min)
            min = plaintext[i];
    }

    // Padding the plaintext with the minimum character
    while (len % key_len != 0)
    {
        plaintext += min;
        len++;
    }

    int text_len = plaintext.size();
    string encryptedtext = "";

    // Encrypting the text
    int x = 0;
    for (int i = 0; i < text_len; i++)
    {
        encryptedtext += ((plaintext[i] - 'A') + (key[x] - '0')) % 26 + 'A';
        x++;
        x = x % key_len;
    }

    cout << "Encrypted text: " << encryptedtext;
}