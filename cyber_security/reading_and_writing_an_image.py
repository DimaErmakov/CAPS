# install via pip install opencv-contrib-python
import cv2
import numpy as np
import re
from numpy.typing import NDArray

data: NDArray = cv2.imread("iamcode.png")


def add_message_to_image_low_bits(data: NDArray, msg: str):
    print("encoding to image of shape", data.shape)
    print(f"{msg=}")

    msg = msg.upper()
    assert msg.isalpha()
    r, c, channels = data.shape

    # RGB or some permutation:
    assert channels == 3

    k: int = 0
    for i in range(r):
        for j in range(c):
            # clear lower nibble of each channel:
            data[i, j] &= 255 - 3

            # NOTE: continuing to clear bits makes it easier to see where
            # the msg ends because the low bits will be zeros; however, it
            # would be more secure to break when k >= len(msg) instead,
            # leaving garbage message afterwards.
            if k < len(msg):
                char: str = msg[k]

                # ind_c in {0, 1, ... 25}
                ind_c: int = ord(char) - ord("A")

                r, g, b = data[i, j]
                msg_r, msg_g, msg_b = ((ind_c & 48) >> 4, (ind_c & 12) >> 2, ind_c & 3)
                r |= msg_r
                g |= msg_g
                b |= msg_b
                data[i, j] = r, g, b

                k += 1


def get_message_from_image_low_bits(data: NDArray) -> str:
    msg: str = ""

    r, c, _ = data.shape
    for i in range(r):
        for j in range(c):
            # get lower nibble of each channel:
            msg_r, msg_g, msg_b = data[i, j] & 3

            ind_c = (msg_r << 4) | (msg_g << 2) | msg_b
            char = chr(ord("A") + ind_c)

            msg += char

    return msg


# add_message_to_image_low_bits(data, 'CANYOUREADTHIS')
# cv2.imwrite('iamcode.png', data)

data_recovered: NDArray = cv2.imread("iamcode.png")
msg_recovered: str = get_message_from_image_low_bits(data_recovered)
# remove trailing A values at the point beyond which all nibbles are 0
# in every channel (these encode 0 and thus 'A'):
print(re.sub(r"A*$", "", msg_recovered))
