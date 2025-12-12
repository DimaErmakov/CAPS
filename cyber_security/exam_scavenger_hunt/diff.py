def save_diff_bytes(img1_path, img2_path, output_file="hidden_output.bin"):
    from PIL import Image

    img1 = Image.open(img1_path).convert("RGB")
    img2 = Image.open(img2_path).convert("RGB")

    width, height = img1.size
    pixels1 = img1.load()
    pixels2 = img2.load()

    bits = ""
    for y in range(height):
        for x in range(width):
            r1, g1, b1 = pixels1[x, y]
            r2, g2, b2 = pixels2[x, y]

            for c1, c2 in zip((r1, g1, b1), (r2, g2, b2)):
                if c1 != c2:
                    bits += str((c1 ^ c2) & 1)

    # Convert bits to bytes
    byte_array = bytearray()
    for i in range(0, len(bits), 8):
        byte = bits[i : i + 8]
        if len(byte) < 8:
            break
        byte_array.append(int(byte, 2))

    with open(output_file, "wb") as f:
        f.write(byte_array)

    print(f"[+] Raw binary data saved to: {output_file}")


if __name__ == "__main__":
    save_diff_bytes("lightning.png", "iamcode.png")
