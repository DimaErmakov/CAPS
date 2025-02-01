def caesar_cipher_decrypt(ciphertext):
    alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    ciphertext = ciphertext.upper()

    # Load a list of valid English words from SCOWL
    with open("words.txt", "r") as file:
        valid_words = set(word.strip().upper() for word in file.readlines())

    best_shift = 0
    max_word_count = 0
    best_decrypted_text = ""

    for shift in range(26):
        decrypted_text = ""

        # Decrypt the ciphertext with the current shift
        for char in ciphertext:
            if char in alphabet:
                new_index = (alphabet.index(char) - shift) % 26
                decrypted_text += alphabet[new_index]
            else:
                decrypted_text += char

        # Split the decrypted text into potential words (e.g., 3-10 characters long)
        word_count = 0
        for word_length in range(3, 11):  # Adjust range based on expected word lengths
            for i in range(0, len(decrypted_text) - word_length + 1):
                word = decrypted_text[i : i + word_length]
                if word in valid_words:
                    word_count += 1

        # Track the shift with the most valid words
        if word_count > max_word_count:
            max_word_count = word_count
            best_shift = shift
            best_decrypted_text = decrypted_text

        print(f"Shift {shift}: {decrypted_text} (Valid Words: {word_count})")

    # Output the best result
    print(f"\nBest Shift: {best_shift}")
    print(f"Decrypted Text: {best_decrypted_text}")
    print(f"Valid Words Found: {max_word_count}")


# Example usage
encrypted_message = "EUAIGTZKRRGSGTYBOIKYHENOYLXOKTJYNOYBOXZAKYHENOYKTKSOKY"
caesar_cipher_decrypt(encrypted_message)
