import hashlib
import math

def modinv(e,phi):
    for d in range(1,phi):
        if (e*d)%phi == 1:
            return d
    return None

def find_e(phi):
    e=2
    while e<phi:
        if math.gcd(e,phi) == 1:
            return e
        e+=1

def encrypt(message,key,n):
    return [(ord(char)**key)%n for char in message]

def decrypt(encrypted_message,key,n):
    return ''.join([chr((char ** key) % n) for char in encrypted_message])

def calculate_md(message):
    sha1 = hashlib.sha1()
    sha1.update(message.encode('utf-8'))
    return sha1.hexdigest()

message = input("Enter message : ")
p = int(input("Enter p : "))
q = int(input("Enter q : "))
n=p*q
phi=(p-1)*(q-1)

e = find_e(phi)
d = modinv(e,phi)

md1 = calculate_md(message)
ds = encrypt(md1,e,n)

md2 = calculate_md(message)
md1_d = decrypt(ds,d,n)

if md1_d == md2:
    print("Authenticated\n")
else:
    print("Not Authenticated\n")