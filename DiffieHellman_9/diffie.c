#include<stdio.h>
#include<stdlib.h>

int modexp(int base,int exp,int mod){
    int res=1;
    base = base%mod;
    while(exp>0){
        if(exp%2==1){
            res = (res * base) %mod;
        }
        exp = exp/2;
        base = (base * base) %mod;
    }
    return res;
}

int generate_random(int p){
    return ((rand() % (p-1)) + 1);
}

int main(){
    long long p,g,R1,R2,x,y,ka,kb;
    p=23;
    g=9;

    x = generate_random(p);
    R1 = modexp(g,x,p);

    y = generate_random(p);
    R2 = modexp(g,y,p);

    ka = modexp(R2,x,p);
    kb = modexp(R1,y,p);

    printf("p = %lld\n",p);
    printf("g = %lld\n",g);
    printf("x = %lld\n",x);
    printf("y = %lld\n",y);
    printf("R1 = %lld\n",R1);
    printf("R2 = %lld\n",R2);
    printf("ka = %lld\n",ka);
    printf("kb = %lld\n",kb);
}