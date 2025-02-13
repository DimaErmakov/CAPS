import time
from collections import Counter

# Known pairings, from LIVING and recognized words from the text
known_pairings = {
    "P": "L",
    "T": "I",
    "E": "V",
    "Y": "N",
    "V": "G",
    # LIVING
    "K": "C",
    "I": "A",
    "B": "S",
    "D": "E",
    "N": "D",
    "F": "T",
    "R": "O",
    "L": "H",
    "A": "F",
    "O": "P",
    "X": "M",
    "C": "B",
    "G": "R",
    "S": "X",
}


def load_valid_words(file_path):
    with open(file_path, "r") as file:
        valid_words = set(word.strip().upper() for word in file.readlines())

    # import spacy
    # nlp = spacy.load("en_core_web_sm")
    # all_words = set(nlp.vocab.strings)
    # valid_words = all_words  # Initialize valid_words with all_words
    # valid_words = valid_words.intersection(all_words)
    # valid_words = sorted(valid_words, key=len)
    return valid_words


# Decrypt the message using the substitution cipher mapping
def decrypt_message(ciphertext, mapping):
    return "".join(mapping.get(char, " ") for char in ciphertext)


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

    # Initialize the mapping with known pairings
    cipher_to_plain = known_pairings.copy()

    # Initialize the best solution
    best_mapping = cipher_to_plain.copy()
    best_decrypted_message = decrypt_message(encrypted_text, best_mapping)
    best_score, _ = count_valid_words(best_decrypted_message, valid_words)

    # Recursive function to explore mappings
    def explore_mappings(mapping, remaining_cipher_chars, remaining_plain_chars):
        nonlocal best_mapping, best_decrypted_message, best_score

        # If no more characters to map, evaluate the current mapping
        if not remaining_cipher_chars:
            decrypted_message = decrypt_message(encrypted_text, mapping)
            weighted_score, words = count_valid_words(decrypted_message, valid_words)

            # Update the best solution if the current one is better
            if weighted_score > best_score:
                print(f"Words found: {words}")
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
            max_possible_score = weighted_score + len(remaining_cipher_chars) * 5
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


def main():
    encrypted_text = "FLDBRQPFLIFTBIPFRVDFLDGKRQGIVDRQBIYNVGDIFTBXIGUDNICREDIPPCMFWRKLIGIKFDGTBFTKBRYDRAFLDBDTBTYNTAADGDYKDFRRQFWIGNKTGKQXBFIYKDBARGBQKLIODGBRYKLDGTBLDBFLDKRYETKFTRYFLIFYRFLTYVCQFXRGIPVRRNYDBBIYNOGROGTDFMNDBDGEDBFRCDDTFLDGINXTGDNRGWTBLDNARGRGBFGTEDYIAFDGIYNFLIFLDRQVLFYRFFRCDBQCJDKFFRIYMXIYRGIYMOIBBTRYRGIYMIKKTNDYFRAARGFQYDFLDBDKRYNKLIGIKFDGTBFTKTBFLIFWLDYFLDBRQPTBNTBKTOPTYDNTYFLDWIMICREDXDYFTRYDNRYDBLRQPNNRNDDNBYRFRYPMVGDIFIYNTYFLDLTVLDBFNDVGDDQBDAQPCQFDSFGDXDPMIGNQRQBIYNPICRGTRQBIYNAGIQVLFWTFLNIYVDGCRFLFRPTADIYNFRXIYMFLTYVBFLIFXIUDPTADWRGFLPTETYV"

    valid_words = load_valid_words("words.txt")

    best_mapping, best_decrypted_message, best_score = solve_substitution_cipher(
        encrypted_text, valid_words
    )

    print("\nFinal Best Mapping:")
    print(best_mapping)
    print(f"\nFinal Decrypted Message: {best_decrypted_message}")
    print(f"Final Weighted Score: {best_score}")


# Explaination:

# Final Decrypted Message: THESOULTHATISALTOGETHERCOURAGEOUSANDGREATISMARKEDABOVEALLBYTWOCHARACTERISTICSONEOFTHESEISINDIFFERENCETOOUTWARDCIRCUMSTANCESFORSUCHAPERSONCHERISHESTHECONVICTIONTHATNOTHINGBUTMORALGOODNESSANDPROPRIETYDESERVESTOBEEITHERADMIREDORWISHEDFORORSTRIVENAFTERANDTHATHEOUGHTNOTTOBESUBJECTTOANYMANORANYPASSIONORANYACCIDENTOFFORTUNETHESECONDCHARACTERISTICISTHATWHENTHESOULISDISCIPLINEDINTHEWAYABOVEMENTIONEDONESHOULDDODEEDSNOTONLYGREATANDINTHEHIGHESTDEGREEUSEFULBUTEXTREMELYARDUOUSANDLABORIOUSANDFRAUGHTWITHDANGERBOTHTOLIFEANDTOMANYTHINGSTHATMAKELIFEWORTHLIVING
# After I ran the code, I personally recognized through sight the words discipline and accident in the current best attempt
# I hard coded the mapping for the letters matching up to discipline and accident
# I kept rerunning the code, finding more words through sight like conviction, until the message was decrypted
# the biggest issue I had was having a bad word bank. My word bank was only the most first 1000 most common words in the english language
# I then tired a much larger word bank, but it was way too slow and had horrible words like 'ULT', 'LTH', 'ATI', 'TIS', 'ISA', 'SAL'
# most of the words in the answer were not in the word bank (like accident and discipline) so my code had a hard time finding the answer
# overall, I think the code is pretty solid, but my word bank was not good enough
# although, it was quite fun to look at a decent current solution and visually see the words
# if you comment out lines 12-25, you can see the current best solution and recognize words like difference, discipline, and accident


if __name__ == "__main__":
    main()
