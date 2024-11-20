import hashlib
import math

def modinv(e,phi):
    for d in range(1,phi):
        if(e*d)%phi == 1:
            return d
    return None 

def encrypt(message,key,n):
    return [(ord(char) ** key) % n for char in message]

def decrypt(encrypted_message,key,n):
    return ''.join([chr((char ** key) % n) for char in encrypted_message])

def find_e(phi):
    e=2
    while e < phi:
        if math.gcd(e,phi) == 1:
            return e
        e +=1
    return None

def calculate_message_digest(message):
    sha1 = hashlib.sha1()
    sha1.update(message.encode('utf-8'))
    return sha1.hexdigest()

message = input("Enter message : ")
p = int(input("Enter p: "))
q = int(input("Enter q :"))
n = p*q
phi = (p-1)*(q-1)

e = find_e(phi)
d = modinv(e,phi)

md1 = calculate_message_digest(message)
print(f"Message Digest 1 : {md1}")
ds = encrypt(md1,e,n)
print(f"Digital Signature : {ds} ")

md2 = calculate_message_digest(message)
print("Message Digest 2 on reciver side : {md2}")
md1_d = decrypt(ds,d,n)
print(f"Message Digest 1 on reciver side : {md1_d}")

if md1_d == md2:
    print("Authenticated\n")
else:
    print("Not Authenticated\n")