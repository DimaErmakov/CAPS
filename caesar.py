def caesar_cipher_decrypt(ciphertext):
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    ciphertext = ciphertext.upper()

    for shift in range(26):
        decrypted_text = ""

        for char in ciphertext:
            if char in alphabet:
                new_index = (alphabet.index(char) - shift) % 26
                decrypted_text += alphabet[new_index]
            else:
                decrypted_text += char

        print(f"Shift {shift}: {decrypted_text}")


# Example usage
encrypted_message = "EUAIGTZKRRGSGTYBOIKYHENOYLXOKTJYNOYBOXZAKYHENOYKTKSOKY"
caesar_cipher_decrypt(encrypted_message)
