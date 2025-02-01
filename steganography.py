import cv2
import numpy as np
from numpy.typing import NDArray

data: NDArray = cv2.imread("pic.png")


# TODO insert message into image
def insert_message_into_image(image_path: str, message: str) -> None:
    image = cv2.imread(image_path)
    binary_message = "".join(format(ord(char), "08b") for char in message)

    data = image.copy()
    data = data.reshape(-1, 3)  # Flatten the image array

    for i, bit in enumerate(binary_message):
        if i < len(data):
            data[i][0] = (data[i][0] & ~1) | int(bit)  # Modify LSB of the blue channel

    data = data.reshape(image.shape)  # Reshape back to original dimensions


cv2.imwrite("pic.png", data)


# TODO extract message from image
def extract_message_from_image(image_path: str) -> str:
    image = cv2.imread(image_path)
    binary_message = ""

    for row in image:
        for pixel in row:
            for color in pixel[:3]:  # Ignore alpha channel if present
                binary_message += str(color & 1)  # Extract LSB

    # Convert binary to string
    message = ""
    for i in range(0, len(binary_message), 8):
        byte = binary_message[i : i + 8]
        if len(byte) < 8:
            break
        message += chr(int(byte, 2))

    return message
