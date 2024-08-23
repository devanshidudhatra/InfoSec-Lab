#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MOD 26

// Function to calculate the GCD (Greatest Common Divisor)
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to multiply a matrix with a vector
void multiplyMatrix(const int key[3][3], const int block[3], int result[3], int dim)
{
    int i, j, k;
    for (i = 0; i < dim; i++)
    {
        result[i] = 0;
        for (k = 0; k < dim; k++)
        {
            result[i] += key[i][k] * block[k];
        }
        result[i] %= MOD;
    }
}

// Function to calculate the determinant of a matrix
int determinant(const int matrix[3][3], int n)
{
    if (n == 1)
    {
        return matrix[0][0];
    }

    int det = 0, sign = 1;
    int minor[3][3];
    for (int i = 0; i < n; i++)
    {
        int minor_row = 0;
        for (int j = 1; j < n; j++)
        {
            int minor_col = 0;
            for (int k = 0; k < n; k++)
            {
                if (k == i)
                    continue;
                minor[minor_row][minor_col] = matrix[j][k];
                minor_col++;
            }
            minor_row++;
        }
        det += sign * matrix[0][i] * determinant(minor, n - 1);
        sign *= -1;
    }
    return det % MOD;
}

// Function to calculate the adjoint of a matrix
void adjoint(const int matrix[3][3], int adjoint[3][3], int n)
{
    int minor[3][3];
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int minor_row = 0;
            for (int k = 0; k < n; k++)
            {
                if (k == i)
                    continue;
                int minor_col = 0;
                for (int l = 0; l < n; l++)
                {
                    if (l == j)
                        continue;
                    minor[minor_row][minor_col] = matrix[k][l];
                    minor_col++;
                }
                minor_row++;
            }
            adjoint[j][i] = determinant(minor, n - 1) * ((i + j) % 2 == 0 ? 1 : -1);
            adjoint[j][i] = (adjoint[j][i] + MOD) % MOD; // Ensure positive modulo
        }
    }
}

// Function to calculate the modular inverse of a matrix
int modInverse(int matrix[3][3], int adj[3][3], int n, int det)
{
    int inv_det = 0;
    det = (det + MOD) % MOD;
    
    // Check if determinant has a modular inverse
    if (gcd(det, MOD) != 1)
    {
        return 0; // No inverse exists
    }

    // Find modular inverse of determinant
    for (int i = 1; i < MOD; i++)
    {
        if ((det * i) % MOD == 1)
        {
            inv_det = i;
            break;
        }
    }

    // Calculate the inverse matrix
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = (adj[i][j] * inv_det) % MOD;
            if (matrix[i][j] < 0)
            {
                matrix[i][j] += MOD;
            }
        }
    }
    return 1;
}

// Function to encrypt the plaintext
void encrypt(char *plaintext, const int key[3][3], char *ciphertext, int dim)
{
    int len = strlen(plaintext);

    // Adding padding if necessary
    while (len % dim != 0)
    {
        plaintext[len] = 'X';
        len++;
    }
    plaintext[len] = '\0';

    int block[3], result[3];
    for (int i = 0; i < strlen(plaintext); i += dim)
    {
        for (int j = 0; j < dim; j++)
        {
            block[j] = plaintext[i + j] - 'A';
        }
        multiplyMatrix(key, block, result, dim);
        for (int j = 0; j < dim; j++)
        {
            ciphertext[i + j] = result[j] + 'A';
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

// Function to decrypt the ciphertext
void decrypt(char *ciphertext, const int key[3][3], char *plaintext, int dim)
{
    int det = determinant(key, dim);
    int adj[3][3];
    adjoint(key, adj, dim);

    int inverseKey[3][3];
    if (!modInverse(inverseKey, adj, dim, det))
    {
        printf("Matrix is not invertible, decryption failed.\n");
        return;
    }

    encrypt(ciphertext, inverseKey, plaintext, dim);

    int len = strlen(plaintext);
    while (plaintext[len - 1] == 'X')
    {
        plaintext[--len] = '\0';
    }
}

int main()
{
    char key[100], plaintext[100];

    // Get the key and plaintext from the user
    printf("Enter the key (length should be 4 or 9): ");
    scanf("%s", key);
    printf("Enter plain text: ");
    scanf("%s", plaintext);

    int key_len = strlen(key); 
    int dim = sqrt(key_len);   

    if (dim * dim != key_len)
    {
        printf("Invalid key length. It should be 4 or 9.\n");
        return 1;
    }

    int key_matrix[3][3] = {0};

    // Convert the key into a numerical matrix (A=0 to Z=25)
    int index = 0;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            key_matrix[i][j] = key[index] - 'A';
            index++;
        }
    }

    char ciphertext[100] = {0};
    encrypt(plaintext, key_matrix, ciphertext, dim);
    printf("The Encrypted Text: %s\n", ciphertext);

    char decrypted[100] = {0};
    decrypt(ciphertext, key_matrix, decrypted, dim);
    printf("The Decrypted Text: %s\n", decrypted);

    return 0;
}
