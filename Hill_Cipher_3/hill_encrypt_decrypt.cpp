#include <iostream>
#include <vector>

using namespace std;

const int MOD = 26;

vector<vector<int>> multiplyMatrix(const vector<vector<int>> &key, const vector<int> &block)
{
    int n = key.size();
    vector<vector<int>> result(n, vector<int>(1, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            for (int k = 0; k < n; k++)
            {
                result[i][j] += key[i][k] * block[k];
            }
            result[i][j] %= MOD;
        }
    }
    return result;
}

int determinant(const vector<vector<int>> &matrix)
{
    if (matrix.size() == 1)
    {
        return matrix[0][0];
    }
    int det = 0;
    int sign = 1;
    for (int i = 0; i < matrix.size(); i++)
    {
        vector<vector<int>> minor(matrix.size() - 1, vector<int>(matrix.size() - 1, 0));
        for (int j = 1; j < matrix.size(); j++)
        {
            for (int k = 0, col = 0; k < matrix.size(); k++)
            {
                if (k == i)
                    continue;
                minor[j - 1][col++] = matrix[j][k];
            }
        }
        det += sign * matrix[0][i] * determinant(minor);
        sign *= -1;
    }
    return det;
}

vector<vector<int>> adjoint(const vector<vector<int>> &matrix)
{
    int n = matrix.size();
    vector<vector<int>> adjoint(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            vector<vector<int>> minor(n - 1, vector<int>(n - 1, 0));
            for (int k = 0, row = 0; k < n; k++)
            {
                if (k == i)
                    continue;
                for (int l = 0, col = 0; l < n; l++)
                {
                    if (l == j)
                        continue;
                    minor[row][col++] = matrix[k][l];
                }
                row++;
            }
            adjoint[i][j] = determinant(minor) * ((i + j) % 2 == 0 ? 1 : -1);
        }
    }
    vector<vector<int>> result = adjoint;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            swap(result[i][j], result[j][i]);
        }
    }
    return result;
}

string encrypt(const string &plaintext, const vector<vector<int>> &key)
{
    string modifiedPlaintext = plaintext;
    int n = key.size();
    int padding = n - modifiedPlaintext.size() % n;
    if (padding != n)
    {
        modifiedPlaintext += string(padding, 'X');
    }

    string ciphertext = "";
    for (int i = 0; i < modifiedPlaintext.size(); i += n)
    {
        vector<int> block(n, 0);
        for (int j = 0; j < n; j++)
        {
            block[j] = modifiedPlaintext[i + j] - 'A';
        }
        vector<vector<int>> encryptedBlock = multiplyMatrix(key, block);
        for (const auto &row : encryptedBlock)
        {
            for (int value : row)
            {
                ciphertext += (char)(value + 'A');
            }
        }
    }
    return ciphertext;
}

string decrypt(const string &ciphertext, const vector<vector<int>> &key)
{
    int det = determinant(key);
    vector<vector<int>> adj = adjoint(key);
    vector<vector<int>> inverseKey = key;

    int n = key.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            inverseKey[i][j] = (adj[i][j] * det) % MOD;
            if (inverseKey[i][j] < 0)
            {
                inverseKey[i][j] += MOD;
            }
        }
    }
    return encrypt(ciphertext, inverseKey);
}

int main()
{
    vector<vector<int>> key = {{6, 24, 1}, {13, 16, 10}, {20, 17, 15}};
    string plaintext = "HELLO";
    string ciphertext = encrypt(plaintext, key);
    cout << "The Encrypted Text: " << ciphertext << endl;
    string decrypted = decrypt(ciphertext, key);
    cout << "The Decrypted Text: " << decrypted << endl;
    return 0;
}