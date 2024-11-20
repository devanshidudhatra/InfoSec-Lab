#include<bits/stdc++.h>
using namespace std;

int modinv(int a){
    a = a % 26;
    for(int i=1;i<26;i++){
        if((a*i)%26 == 1){
            return i;
        }
    }
    return -1;
}

int determinant(int matrix[3][3] , int n){
    int det =0;
    if(n==2){
        det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    else if(n==3){
        det = matrix[0][0] * ((matrix[1][1] * matrix[2][2]) - (matrix[1][2]*matrix[2][1]))-
              matrix[0][1] * ((matrix[1][0] * matrix[2][2]) - (matrix[1][2]*matrix[2][0]))+
              matrix[0][2] * ((matrix[1][0] * matrix[2][1]) - (matrix[1][1]*matrix[2][0]));
    }
    return (det % 26 + 26) % 26;
}

void adjoint(int matrix[3][3] , int adj[3][3] , int n){
    if(n==2){
        adj[0][0] = matrix[1][1];
        adj[0][1] = -matrix[0][1];
        adj[1][0] = -matrix[1][0];
        adj[1][1] = matrix[0][0];
    }
    else if(n==3){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                adj[j][i] = (matrix[(i+1)%3][(j+1)%3] * matrix[(i+2)%3][(j+2)%3]) - (matrix[(i+1)%3][(j+2)%3] * matrix[(i+2)%3][(j+1)%3]) % 26;
                if(adj[j][i] < 0){
                    adj[j][i] += 26;
                }
            }
        }
    }
}

bool inverse(int matrix[3][3] , int inv[3][3] , int n){
    int det = determinant(matrix,n);
    if(det == 0){
        cout << "dterminant No inverse " << endl;
        return false;
    }
    int inv_det = modinv(det);
    if(inv_det == -1){
        cout << "No inverse" << endl;
        return false;
    }
    int adj[3][3] = {0};
    adjoint(matrix,adj,n);

    for (int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            inv[i][j] = (adj[i][j] * inv_det)% 26;
            if(inv[i][j] < 0){
                inv[i][j] += 26;
            }
        }
    }
    return true;
}

int main(){
    int n;
    cout << "Enter size of matrix : ";
    cin >> n;
    string key;
    cout << "Enter key : ";
    cin >> key;

    int dim = sqrt(n);
    int key_matrix [3][3] = {0};
    int inv_key_matrix[3][3] = {0};

    int k=0;

    for(int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            key_matrix[i][j] = key[k++] - 'a';
        }
    }

    string plaintext;
    cout << "Enter plaintext : ";
    cin >> plaintext;

    while(plaintext.length() % dim != 0){
        plaintext += 'x';
    }

    string ciphertext;
    

    for(int i=0;i<plaintext.size();i+=dim){
        int test_matrix[3][1] = {0};
        for(int j=0;j<dim;j++){
            test_matrix[j][0] = plaintext[i+j] - 'a';
        }
        int result[3][1] = {0};

        for(int x=0;x<dim;x++){
            for(int y=0;y<1;y++){
                result[x][y] = 0;
                for(int z=0;z<dim;z++){
                    result[x][y] += key_matrix[x][z] * test_matrix[z][y];
                }
                result[x][y] = result[x][y] % 26;
            }
        }

        for(int x=0; x<dim ;x++){
            ciphertext += char(result[x][0] + 'a');
        }
    }

    cout << "Ciphertext : " << ciphertext << endl;

    string decrypted;
    string final;

    if(!inverse(key_matrix,inv_key_matrix,dim)){
        cout << "No Inverse" << endl;
    }

    for(int i=0;i<ciphertext.size();i+=dim){
        int test_matrix[3][1] = {0};
        for(int j=0;j<dim;j++){
            test_matrix[j][0] = ciphertext[i+j] - 'a';
        }
        int result[3][1] = {0};
        for(int x=0;x<dim;x++){
            for(int y=0;y<1;y++){
                result[x][y] = 0;
                for(int z=0;z<dim;z++){
                    result[x][y] += inv_key_matrix[x][z] * test_matrix[z][y];
                }
                result[x][y] = result[x][y] % 26;
            }
        }

        for(int x=0; x<dim ;x++){
            decrypted += char(result[x][0] + 'a');
        }
    }
    k=0;
    for(int i=0;i<decrypted.length();i++){
        if(decrypted[i] == 'x' && (decrypted[i-1] == decrypted[i+1] || i == decrypted.length()-1)){
            continue;
        }
        final += decrypted[i];
    }
    cout << "Final : " << final << endl;
}