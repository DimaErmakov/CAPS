from collections import Counter


# Load the words.txt file into a set of valid words
def load_valid_words(file_path):
    with open(file_path, "r") as file:
        valid_words = set(word.strip().upper() for word in file.readlines())
    return valid_words


# Decrypt the message using the substitution cipher mapping
def decrypt_message(ciphertext, mapping):
    plaintext = []
    for char in ciphertext:
        if char in mapping:
            plaintext.append(mapping[char])
        else:
            plaintext.append("?")  # Unknown character
    return "".join(plaintext)


# Count the number of valid words in the decrypted message
def count_valid_words(decrypted_text, valid_words):
    word_count = 0
    for word_length in range(3, 11):  # Adjust range based on expected word lengths
        for i in range(0, len(decrypted_text) - word_length + 1):
            word = decrypted_text[i : i + word_length]
            if word in valid_words:
                word_count += 1
    return word_count


# Main function
def main():
    # Encrypted text
    encrypted_text = "FLDBRQPFLIFTBIPFRVDFLDGKRQGIVDRQBIYNVGDIFTBXIGUDNICREDIPPCMFWRKLIGIKFDGTBFTKBRYDRAFLDBDTBTYNTAADGDYKDFRRQFWIGNKTGKQXBFIYKDBARGBQKLIODGBRYKLDGTBLDBFLDKRYETKFTRYFLIFYRFLTYVCQFXRGIPVRRNYDBBIYNOGROGTDFMNDBDGEDBFRCDDTFLDGINXTGDNRGWTBLDNARGRGBFGTEDYIAFDGIYNFLIFLDRQVLFYRFFRCDBQCJDKFFRIYMXIYRGIYMOIBBTRYRGIYMIKKTNDYFRAARGFQYDFLDBDKRYNKLIGIKFDGTBFTKTBFLIFWLDYFLDBRQPTBNTBKTOPTYDNTYFLDWIMICREDXDYFTRYDNRYDBLRQPNNRNDDNBYRFRYPMVGDIFIYNTYFLDLTVLDBFNDVGDDQBDAQPCQFDSFGDXDPMIGNQRQBIYNPICRGTRQBIYNAGIQVLFWTFLNIYVDGCRFLFRPTADIYNFRXIYMFLTYVBFLIFXIUDPTADWRGFLPTETYV"
    encrypted_text = (
        encrypted_text.replace("V", "G")
        .replace("Y", "N")
        .replace("T", "I")
        .replace("E", "V")
        .replace("P", "L")
    )

    print(
        "Encrypted Text:",
        encrypted_text,
    )
    return
    # Known plaintext ending
    known_plaintext = "LIVING"
    known_ciphertext = encrypted_text[-len(known_plaintext) :]

    # Create a mapping from ciphertext to plaintext
    cipher_to_plain = {}

    # Map the known ending
    for c, p in zip(known_ciphertext, known_plaintext):
        cipher_to_plain[c] = p

    # Hard-coded mappings (you can add or modify these)
    hard_coded_mappings = {
        # Example mappings (replace with your guesses)
        "I": "A",
        "K": "C",
    }

    # Apply hard-coded mappings
    for cipher_char, plain_char in hard_coded_mappings.items():
        if cipher_char not in cipher_to_plain:
            cipher_to_plain[cipher_char] = plain_char

    # Frequency analysis of the encrypted text
    encrypted_freq = Counter(encrypted_text)
    sorted_encrypted_freq = sorted(
        encrypted_freq.items(), key=lambda x: x[1], reverse=True
    )

    # Frequency of letters in English (from The Brothers Karamazov and The Iliad)
    english_freq = [
        "E",
        "T",
        "A",
        "O",
        "I",
        "N",
        "S",
        "H",
        "R",
        "D",
        "L",
        "C",
        "U",
        "M",
        "W",
        "F",
        "G",
        "Y",
        "P",
        "B",
        "V",
        "K",
        "J",
        "X",
        "Q",
        "Z",
    ]

    # Map the most frequent encrypted letters to the most frequent English letters
    for i, (cipher_char, _) in enumerate(sorted_encrypted_freq):
        if cipher_char not in cipher_to_plain:
            if i < len(english_freq):
                cipher_to_plain[cipher_char] = english_freq[i]

    # Load valid words from words.txt
    valid_words = load_valid_words("words.txt")

    # Interactive loop for manual substitutions
    while True:
        # Decrypt the message using the current mapping
        decrypted_message = decrypt_message(encrypted_text, cipher_to_plain)

        # Count the number of valid words in the decrypted message
        word_count = count_valid_words(decrypted_message, valid_words)

        # Output the results
        print("\nCurrent Decrypted Message:")
        print(decrypted_message)
        print(f"Number of Valid Words Found: {word_count}")
        print("\nCurrent Mapping:")
        print(cipher_to_plain)

        # Ask the user for input
        print("\nEnter a substitution (e.g., 'F=A') or type 'done' to finish:")
        user_input = input().strip().upper()

        if user_input == "DONE":
            break

        # Parse the user input
        try:
            cipher_char, plain_char = user_input.split("=")
            cipher_char = cipher_char.strip()
            plain_char = plain_char.strip()

            # Update the mapping
            cipher_to_plain[cipher_char] = plain_char
            print(f"Updated mapping: {cipher_char} -> {plain_char}")
        except ValueError:
            print("Invalid input. Please use the format 'F=A'.")

    print("\nFinal Decrypted Message:")
    print(decrypted_message)
    print(f"Final Number of Valid Words Found: {word_count}")
    print("\nFinal Mapping:")
    print(cipher_to_plain)


# Run the main function
if __name__ == "__main__":
    main()
