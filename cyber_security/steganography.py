import cv2
import numpy as np
from numpy.typing import NDArray


# Insert message into the image
def insert_message_into_image(image_path: str, message: str, output_path: str) -> None:
    image = cv2.imread(image_path)
    if image is None:
        raise ValueError("Image not found or could not be read.")

    # Convert the message to binary
    binary_message = "".join(format(ord(char), "08b") for char in message)
    binary_message += "00000000"  # Add a null terminator to mark the end of the message

    # Flatten the image array
    data = image.copy()
    data = data.reshape(-1)

    # Embed the message in the LSB of blue channel
    for i in range(len(binary_message)):
        data[i] = (data[i] & ~1) | int(binary_message[i])  # Modify LSB

    # Reshape back to the original image dimensions
    data = data.reshape(image.shape)

    # Save the modified image
    cv2.imwrite(output_path, data)


# Extract message from the image
def extract_message_from_image(image_path: str) -> str:
    image = cv2.imread(image_path)

    # Flatten the image array
    data = image.reshape(-1)

    # Extract the LSBs from the blue channel
    binary_message = ""
    for i in range(len(data)):
        binary_message += str(data[i] & 1)  # Extract LSB
        if len(binary_message) % 8 == 0 and binary_message.endswith("00000000"):
            break  # Stop when the null terminator is found

    # Convert binary to string
    message = ""
    for i in range(0, len(binary_message) - 8, 8):  # Exclude the null terminator
        byte = binary_message[i : i + 8]
        message += chr(int(byte, 2))

    return message


if __name__ == "__main__":
    # Insert the message into the image
    insert_message_into_image(
        "pic.png", "https://www.youtube.com/watch?v=dQw4w9WgXcQ", "pic_stego.png"
    )

    # Extract the message from the modified image
    extracted_message = extract_message_from_image("pic_stego.png")
    print("Extracted Message:", extracted_message)
# Explaination:
# converted message to binary, Embed the message, and then extracted it back.
