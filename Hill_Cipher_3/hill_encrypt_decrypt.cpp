#include <bits/stdc++.h>
using namespace std;

int modInverse(int a)
{
    a = a % 26; // 26 for English alphabets
    for (int x = 1; x < 26; x++)
    {
        if ((a * x) % 26 == 1)
        {
            return x;
        }
    }
    return -1;
}

// Function to find the determinant of a matrix
int determinant(int matrix[3][3], int n)
{
    int det = 0;
    if (n == 2) // For 2x2 Matrix
    {
        det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    else if (n == 3) // For 3x3 Matrix
    {
        det = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
              matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
              matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    }
    return (det % 26 + 26) % 26; // Ensure the determinant is non-negative
}

// Function to calculate the adjoint of the matrix
void adjoint(int matrix[3][3], int adj[3][3], int n)
{
    if (n == 2) // For 2x2 Matrix
    {
        adj[0][0] = matrix[1][1];
        adj[0][1] = -matrix[0][1];
        adj[1][0] = -matrix[1][0];
        adj[1][1] = matrix[0][0];
    }
    else if (n == 3) // For 3x3 Matrix
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                adj[j][i] = (matrix[(i + 1) % 3][(j + 1) % 3] * matrix[(i + 2) % 3][(j + 2) % 3] - matrix[(i + 1) % 3][(j + 2) % 3] * matrix[(i + 2) % 3][(j + 1) % 3]) % 26;
                if (adj[j][i] < 0)
                {
                    adj[j][i] += 26;
                }
            }
        }
    }
}

// Function to calculate the inverse of the matrix
bool inverse(int matrix[3][3], int inv[3][3], int n)
{
    int det = determinant(matrix, n);
    if (det == 0)
    {
        cout << "Matrix is singular, can't find its inverse." << endl;
        return false;
    }
    int det_inv = modInverse(det);
    if (det_inv == -1)
    {
        cout << "Determinant has no modular inverse, can't find the matrix inverse." << endl;
        return false;
    }

    int adj[3][3];
    adjoint(matrix, adj, n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            inv[i][j] = (adj[i][j] * det_inv) % 26;
            if (inv[i][j] < 0)
            {
                inv[i][j] += 26;
            }
        }
    }
    return true;
}

int main()
{
    int n;
    cout << "Enter the size of key (4 or 9): ";
    cin >> n;
    string key;
    cout << "Enter the key: ";
    cin >> key;

    int k = 0;
    int dim = sqrt(n);
    int key_matrix[3][3] = {0};     // Initialize as a 3x3 matrix
    int inv_key_matrix[3][3] = {0}; // Initialize as a 3x3 matrix

    // Fill the key_matrix
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            key_matrix[i][j] = key[k] - 'a';
            k++;
        }
    }

    // Print key matrix
    cout << "Key Matrix: " << endl;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            cout << key_matrix[i][j] << " ";
        }
        cout << endl;
    }

    string plaintext;
    cout << "Enter plaintext: ";
    cin >> plaintext;

    // Adding filler
    while (plaintext.length() % dim != 0)
    {
        plaintext += 'x';
    }

    // Encrypt plaintext
    string ciphertext;
    for (int i = 0; i < plaintext.size(); i += dim)
    {
        int text_matrix[3][1] = {0}; // Maximum size (3x1)
        for (int j = 0; j < dim; j++)
        {
            text_matrix[j][0] = plaintext[i + j] - 'a';
        }

        int rslt[3][1] = {0}; // Maximum size (3x1)

        // Multiply key_matrix and text_matrix
        for (int x = 0; x < dim; x++)
        {
            for (int y = 0; y < 1; y++)
            {
                rslt[x][y] = 0;
                for (int z = 0; z < dim; z++)
                {
                    rslt[x][y] += key_matrix[x][z] * text_matrix[z][y];
                }
                rslt[x][y] = rslt[x][y] % 26;
            }
        }

        for (int x = 0; x < dim; x++)
        {
            ciphertext += char(rslt[x][0] + 'a');
        }
    }

    cout << "Cipher Text: " << ciphertext << endl;

    // Decrypt ciphertext
    string decryptedtext;
    string finalText;

    if (!inverse(key_matrix, inv_key_matrix, dim))
    {
        return 0;
    }

    for (int i = 0; i < ciphertext.size(); i += dim)
    {
        int text_matrix[3][1] = {0}; // Maximum size (3x1)
        for (int j = 0; j < dim; j++)
        {
            text_matrix[j][0] = ciphertext[i + j] - 'a';
        }

        int rslt[3][1] = {0}; // Maximum size (3x1)
        for (int x = 0; x < dim; x++)
        {
            for (int y = 0; y < 1; y++)
            {
                rslt[x][y] = 0;
                for (int z = 0; z < dim; z++)
                {
                    rslt[x][y] += inv_key_matrix[x][z] * text_matrix[z][y];
                }
                rslt[x][y] = rslt[x][y] % 26;
            }
        }

        for (int x = 0; x < dim; x++)
        {
            decryptedtext += char(rslt[x][0] + 'a');
        }
    }

    for (int i = 0; i < decryptedtext.length(); i++)
    {
        if (decryptedtext[i] == 'x' && (i == decryptedtext.length() - 1 || decryptedtext[i - 1] == decryptedtext[i + 1]))
        {
            continue;
        }
        finalText += decryptedtext[i];
    }
    cout << "Decrypted Text: " << finalText << endl;

    cout << "22BCP177" << endl;
}