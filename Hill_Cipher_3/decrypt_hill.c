#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MOD 26

void multiplyMatrix(const int key[3][3], const int block[3], int result[3])
{
    int i, j, k;
    for (i = 0; i < 3; i++)
    {
        result[i] = 0;
        for (k = 0; k < 3; k++)
        {
            result[i] += key[i][k] * block[k];
        }
        result[i] %= MOD;
    }
}

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
    return det;
}

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
        }
    }
}

void modInverse(int matrix[3][3], int adj[3][3], int n, int det)
{
    int inv_det = 0;
    for (int i = 1; i < MOD; i++)
    {
        if ((det * i) % MOD == 1)
        {
            inv_det = i;
            break;
        }
    }

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
}

void encrypt(char *plaintext, const int key[3][3], char *ciphertext)
{
    int len = strlen(plaintext);
    int padding = 3 - len % 3;
    if (padding != 3)
    {
        for (int i = 0; i < padding; i++)
        {
            plaintext[len + i] = 'X';
        }
        plaintext[len + padding] = '\0';
    }

    int block[3], result[3];
    for (int i = 0; i < strlen(plaintext); i += 3)
    {
        for (int j = 0; j < 3; j++)
        {
            block[j] = plaintext[i + j] - 'A';
        }
        multiplyMatrix(key, block, result);
        for (int j = 0; j < 3; j++)
        {
            ciphertext[i + j] = result[j] + 'A';
        }
    }
    ciphertext[strlen(plaintext)] = '\0';
}

void decrypt(char *ciphertext, const int key[3][3], char *plaintext)
{
    int det = determinant(key, 3);
    int adj[3][3];
    adjoint(key, adj, 3);

    int inverseKey[3][3];
    modInverse(inverseKey, adj, 3, det);

    encrypt(ciphertext, inverseKey, plaintext);

    int len = strlen(plaintext);
    while (plaintext[len - 1] == 'X')
    {
        plaintext[--len] = '\0';
    }
}

int main()
{
    int key[3][3] = {{6, 24, 1}, {13, 16, 10}, {20, 17, 15}};
    char plaintext[100] = "HELLO";
    char ciphertext[100];

    encrypt(plaintext, key, ciphertext);
    printf("The Encrypted Text: %s\n", ciphertext);

    char decrypted[100];
    decrypt(ciphertext, key, decrypted);
    printf("The Decrypted Text: %s\n", decrypted);

    return 0;
}
