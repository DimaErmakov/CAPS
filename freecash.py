import time
import keyboard
from matplotlib.pylab import rand
import pyautogui
import webbrowser
import random

DELAY = 0.1
width, height = pyautogui.size()
MAX_ATTEMPTS = 100
confidence = 0.7
pyautogui.FAILSAFE = True
pyautogui.PAUSE = DELAY


def cord_click(coordinates):
    pyautogui.moveTo(coordinates[0], coordinates[1])
    pyautogui.click()


def find_and_click_image(image_filename, biasx=0, biasy=0, up_or_down=None):
    global confidence, width, height
    box = None
    attempt = 0
    while box is None and attempt < MAX_ATTEMPTS:
        try:
            box = pyautogui.locateOnScreen(
                image_filename,
                confidence=confidence,
                region=(0, 0, width, height),
            )

        except pyautogui.ImageNotFoundException:
            print(f"{image_filename} not found.")

        if up_or_down is not None and box is None:
            print(f"{image_filename} not found. Scrolling...")
            factor = 200 if up_or_down == "up" else -200
            pyautogui.scroll(factor)
            time.sleep(DELAY * 5)

        time.sleep(DELAY * 5)
        attempt += 1

    if box is not None:
        x, y, width, height = box
        x = box.left + width / 2 + biasx
        y = box.top + height / 2 + biasy

        cord_click((x, y))
    else:
        print("Image not found after multiple attempts.")


def open_websites(urls):
    for url in urls:
        webbrowser.open(url, new=0, autoraise=True)
        time.sleep(0.1)


def main():
    urls = ["https://freecash.com/rewards"]
    open_websites(urls)
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\freecashWinUpTo250.png",
    )
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\freecashTextBox.png",
    )
    keyboard.write("Free")
    pyautogui.press("tab")
    pyautogui.press("enter")
    time.sleep(5)
    # keyboard.press_and_release("ctrl+w")


# TODO include C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\freecashClaim.png when ready

if __name__ == "__main__":
    main()
