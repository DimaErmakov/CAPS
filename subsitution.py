import time
from collections import Counter

# Known pairings
known_pairings = {
    "I": "A",
    "K": "C",
    "A": "F",
    "B": "S",
    "O": "P",
    "T": "I",
    "D": "E",
    "N": "D",
    "P": "L",
    "Y": "N",
    "V": "G",
    "F": "T",
    "R": "O",
    "E": "V",
    "L": "H",
    "X": "M",
    "C": "B",
    "S": "X",
}


# Load the words.txt file into a set of valid words
def load_valid_words(file_path):
    with open(file_path, "r") as file:
        valid_words = set(word.strip().upper() for word in file.readlines())
    return valid_words


# Decrypt the message using the substitution cipher mapping
def decrypt_message(ciphertext, mapping):
    return "".join(mapping.get(char, "?") for char in ciphertext)


# Count the number of valid words in the decrypted message with weighted scoring
def count_valid_words(decrypted_text, valid_words):
    word_count = 0
    words = []
    unique_words = set()
    weighted_score = 0

    for word_length in range(3, 16):  # Adjust range based on expected word lengths
        for i in range(0, len(decrypted_text) - word_length + 1):
            word = decrypted_text[i : i + word_length]
            if word in valid_words:
                words.append(word)
                word_count += 1

                # Calculate weighted score
                length_bonus = max(
                    0, word_length - 3
                )  # +1 point for each character beyond 3
                uniqueness_bonus = (
                    2 if word not in unique_words else 0
                )  # +2 points for new words
                weighted_score += 1 + length_bonus + uniqueness_bonus

                unique_words.add(word)

    return weighted_score, words


# Branch and bound solver for substitution cipher
def solve_substitution_cipher(encrypted_text, valid_words):
    # Frequency analysis of the encrypted text
    encrypted_freq = Counter(encrypted_text)
    sorted_encrypted_freq = sorted(encrypted_freq.items(), key=lambda x: -x[1])

    # Frequency of letters in English (from The Brothers Karamazov and The Iliad)
    english_freq = [  # Rocognized words from the text by manually looking
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

    # Initialize the mapping with known pairings
    cipher_to_plain = known_pairings.copy()

    # Initialize the best solution
    best_mapping = cipher_to_plain.copy()
    best_decrypted_message = decrypt_message(encrypted_text, best_mapping)
    best_score, _ = count_valid_words(best_decrypted_message, valid_words)

    # Track progress
    total_mappings = 1
    for i in range(len(english_freq) - len(cipher_to_plain), 0, -1):
        total_mappings *= i
    explored_mappings = 0
    start_time = time.time()

    # Recursive function to explore mappings
    def explore_mappings(mapping, remaining_cipher_chars, remaining_plain_chars):
        nonlocal best_mapping, best_decrypted_message, best_score, explored_mappings

        # If no more characters to map, evaluate the current mapping
        if not remaining_cipher_chars:
            decrypted_message = decrypt_message(encrypted_text, mapping)
            weighted_score, words = count_valid_words(decrypted_message, valid_words)

            # Update the best solution if the current one is better
            if weighted_score > best_score:
                print(words)
                best_mapping = mapping.copy()
                best_decrypted_message = decrypted_message
                best_score = weighted_score
                print(f"New Best: {best_score} weighted score")
                print(f"Decrypted Message: {best_decrypted_message}")
            return

        # Get the next cipher character to map
        cipher_char = remaining_cipher_chars[0]

        # Try mapping the cipher character to each remaining plain character
        for plain_char in remaining_plain_chars:
            # Create a new mapping
            new_mapping = mapping.copy()
            new_mapping[cipher_char] = plain_char

            # Decrypt the message with the new mapping
            decrypted_message = decrypt_message(encrypted_text, new_mapping)

            # Calculate the weighted score
            weighted_score, _ = count_valid_words(decrypted_message, valid_words)

            # Prune the branch if it cannot possibly beat the current best
            max_possible_score = weighted_score + len(remaining_cipher_chars) * 4
            if max_possible_score <= best_score:
                continue

            # Recur with the new mapping and remaining characters
            explore_mappings(
                new_mapping,
                remaining_cipher_chars[1:],
                [p for p in remaining_plain_chars if p != plain_char],
            )

    # Start exploring mappings
    remaining_cipher_chars = [
        c for c, _ in sorted_encrypted_freq if c not in cipher_to_plain
    ]
    remaining_plain_chars = [
        p for p in english_freq if p not in cipher_to_plain.values()
    ]
    explore_mappings(cipher_to_plain, remaining_cipher_chars, remaining_plain_chars)

    return best_mapping, best_decrypted_message, best_score


# Main function
def main():
    # Encrypted text
    encrypted_text = "FLDBRQPFLIFTBIPFRVDFLDGKRQGIVDRQBIYNVGDIFTBXIGUDNICREDIPPCMFWRKLIGIKFDGTBFTKBRYDRAFLDBDTBTYNTAADGDYKDFRRQFWIGNKTGKQXBFIYKDBARGBQKLIODGBRYKLDGTBLDBFLDKRYETKFTRYFLIFYRFLTYVCQFXRGIPVRRNYDBBIYNOGROGTDFMNDBDGEDBFRCDDTFLDGINXTGDNRGWTBLDNARGRGBFGTEDYIAFDGIYNFLIFLDRQVLFYRFFRCDBQCJDKFFRIYMXIYRGIYMOIBBTRYRGIYMIKKTNDYFRAARGFQYDFLDBDKRYNKLIGIKFDGTBFTKTBFLIFWLDYFLDBRQPTBNTBKTOPTYDNTYFLDWIMICREDXDYFTRYDNRYDBLRQPNNRNDDNBYRFRYPMVGDIFIYNTYFLDLTVLDBFNDVGDDQBDAQPCQFDSFGDXDPMIGNQRQBIYNPICRGTRQBIYNAGIQVLFWTFLNIYVDGCRFLFRPTADIYNFRXIYMFLTYVBFLIFXIUDPTADWRGFLPTETYV"

    # Load valid words from words.txt
    valid_words = load_valid_words("words.txt")

    # Solve the substitution cipher
    best_mapping, best_decrypted_message, best_score = solve_substitution_cipher(
        encrypted_text, valid_words
    )

    # Output the final results
    print("\nFinal Best Mapping:")
    print(best_mapping)
    print(f"\nFinal Decrypted Message: {best_decrypted_message}")
    print(f"Final Weighted Score: {best_score}")


if __name__ == "__main__":
    main()
