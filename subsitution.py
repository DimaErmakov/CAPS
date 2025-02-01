def branch_and_bound(encrypted_text, known_mapping, remaining_encrypted_letters, remaining_plaintext_letters):
    if not remaining_encrypted_letters:
        # All letters are mapped, check if the decrypted text makes sense
        decrypted_text = decrypt(encrypted_text, known_mapping)
        if is_meaningful(decrypted_text):
            return decrypted_text
        else:
            return None

    # Choose the next encrypted letter to map
    next_encrypted_letter = remaining_encrypted_letters[0]

    for plaintext_letter in remaining_plaintext_letters:
        # Try mapping the encrypted letter to the plaintext letter
        new_mapping = known_mapping.copy()
        new_mapping[next_encrypted_letter] = plaintext_letter

        # Remove the mapped letters from the remaining lists
        new_remaining_encrypted = remaining_encrypted_letters[1:]
        new_remaining_plaintext = [letter for letter in remaining_plaintext_letters if letter != plaintext_letter]

        # Recursively continue with the new mapping
        result = branch_and_bound(encrypted_text, new_mapping, new_remaining_encrypted, new_remaining_plaintext)
        if result:
            return result

    return None

def decrypt(encrypted_text, mapping):
    decrypted_text = []
    for char in encrypted_text:
        if char in mapping:
            decrypted_text.append(mapping[char])
        else:
            decrypted_text.append('?')  # Unknown character
    return ''.join(decrypted_text)

def is_meaningful(text):
    # Expected frequency of letters in the English language
    expected_frequencies = {
        'A': 8.2, 'B': 1.5, 'C': 2.8, 'D': 4.3, 'E': 12.7,
        'F': 2.2, 'G': 2.0, 'H': 6.1, 'I': 6.7, 'J': 0.2,
        'K': 0.8, 'L': 4.0, 'M': 2.4, 'N': 6.7, 'O': 7.5,
        'P': 1.9, 'Q': 0.1, 'R': 6.0, 'S': 6.3, 'T': 9.1,
        'U': 2.8, 'V': 1.0, 'W': 2.4, 'X': 0.2, 'Y': 2.0,
        'Z': 0.1
    }

    # Calculate the frequency of each letter in the text
    text_length = len(text)
    if text_length == 0:
        return False

    observed_frequencies = {}
    for char in text.upper():
        if char in expected_frequencies:
            observed_frequencies[char] = observed_frequencies.get(char, 0) + 1

    # Normalize the observed frequencies to percentages
    for char in observed_frequencies:
        observed_frequencies[char] = (observed_frequencies[char] / text_length) * 100

    # Calculate the difference between observed and expected frequencies
    total_difference = 0
    for char in expected_frequencies:
        expected_freq = expected_frequencies[char]
        observed_freq = observed_frequencies.get(char, 0)
        total_difference += abs(expected_freq - observed_freq)

    # Define a threshold for meaningfulness
    # This threshold can be adjusted based on experimentation
    threshold = 50  # Example threshold, adjust as needed

    # If the total difference is below the threshold, the text is considered meaningful
    return total_difference < threshold

# Example usage
encrypted_text = "WKH TXLFN EURZQ IRA MXPSV RYHU WKH ODCB GRJ QJNJOK"
known_mapping = {'E1': 'L', 'E2': 'I', 'E3': 'V', 'E4': 'I', 'E5': 'N', 'E6': 'G'}
remaining_encrypted_letters = ['X', 'Y', 'Z', 'A', 'B', 'C']  # Replace with actual remaining letters
remaining_plaintext_letters = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

result = branch_and_bound(encrypted_text, known_mapping, remaining_encrypted_letters, remaining_plaintext_letters)
if result:
    print("Decrypted text:", result)
else:
    print("No valid decryption found.")