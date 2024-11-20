#include<bits/stdc++.h>
using namespace std;

int modexp(int base, int exp, int mod){
    int res = 1;
    base = base % mod;
    while(exp>0){
        if(exp%2==1){
            res = (res*base)%mod;
        }
        exp=exp/2;
        base = (base*base)%mod;
    }
    return res;
}

int modInverse(int e, int phi){
    int x=1;
    while(x<phi){
        if((e*x)%phi == 1){
            return x;
        }
        else{
            x++;
        }
    }
    return -1;
}

int main(){
    int p,q,n,phi;
    cout << "Enter primes : " << endl;
    cout << "ENter p :";
    cin >> p;
    cout << "Enter q : ";
    cin >> q;
    n = p*q;
    phi = (p-1)*(q-1);

    int e=2;
    while(e<phi){
        if(__gcd(e,phi) == 1){
            break;
        }
        else{
            e++;
        }
    }

    int plain;
    cout << "ENter plaintext : ";
    cin >> plain;
    int c = modexp(plain,e,n);
    cout << "Ciphertext : " << c << endl;

    int d = modInverse(e,phi);
    if(d == -1){
        cout << "Inverse no" << endl;
    }
    int decrypt = modexp(c,d,n);
    cout << "Decrypted : " << decrypt << endl;
}