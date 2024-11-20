import numpy as np
from math import gcd

def matrix_mod_inv(matrix, mod):
    # Calculate the determinant of the matrix and round it
    det = int(np.round(np.linalg.det(matrix)))
    # Check if determinant is coprime with the modulus
    if gcd(det, mod) != 1:
        raise ValueError("The determinant of the matrix is not coprime with the modulus (26). Choose a different keyword.")
    # Compute the modular multiplicative inverse of the determinant
    det_inv = pow(det, -1, mod)
    # Compute the adjugate matrix and round it
    matrix_adj = np.round(det * np.linalg.inv(matrix)).astype(int) % mod
    # Compute the inverse matrix by multiplying the adjugate with the determinant inverse
    return (det_inv * matrix_adj) % mod

def process_text(text, block_size):
    # Group characters into blocks of `block_size`, padding with 'X' if necessary
    pairs = []
    index = 0
    while index < len(text):
        block = text[index:index + block_size]
        if len(block) < block_size:
            block += 'X' * (block_size - len(block))
        pairs.append(block)
        index += block_size
    return pairs

def text_to_numbers(text_blocks):
    # Convert text blocks to numerical values (0-25)
    return [[ord(char) - ord('A') for char in block] for block in text_blocks]

def numbers_to_text(num_blocks):
    # Convert numerical blocks to text
    return [''.join(chr(num + ord('A')) for num in block) for block in num_blocks]

def hill_cipher():
    choice = input("Do you want to (E)ncrypt or (D)ecrypt? ").strip().upper()
    text = input("Enter the text: ").upper().replace(" ", "")
    keyword = input("Enter a 9-letter keyword: ").upper().replace(" ", "")
    
    if len(keyword) != 9:
        raise ValueError("The keyword must be exactly 9 letters long.")
    
    # Convert the keyword into a 3x3 matrix
    keyword_numbers = [ord(char) - ord('A') for char in keyword]
    matrix = np.array([keyword_numbers[i:i+3] for i in range(0, 9, 3)])
    
    # Check if the matrix determinant is valid for decryption
    if choice == 'D':
        det = int(np.round(np.linalg.det(matrix)))
        if gcd(det, 26) != 1:
            raise ValueError("The determinant of the matrix is not coprime with 26. Decryption is not possible with this keyword.")
    
    # Prepare text blocks and convert them to numbers
    text_blocks = process_text(text, 3)
    numerical_blocks = text_to_numbers(text_blocks)
    
    if choice == 'E':
        # Encrypt: Multiply blocks by the matrix and mod 26
        encrypted_blocks = np.dot(numerical_blocks, matrix) % 26
        encrypted_text = ''.join(numbers_to_text(encrypted_blocks))
        print(f"Encrypted Text: {encrypted_text}")
    
    elif choice == 'D':
        # Decrypt: Compute the inverse matrix, multiply by it, and mod 26
        inverse_matrix = matrix_mod_inv(matrix, 26)
        decrypted_blocks = np.dot(numerical_blocks, inverse_matrix) % 26
        decrypted_text = ''.join(numbers_to_text(decrypted_blocks)).replace('X', '')
        print(f"Decrypted Text: {decrypted_text}")
    
    else:
        print("Invalid choice! Please select either 'E' or 'D'.")

# Run the Hill Cipher program
hill_cipher()
