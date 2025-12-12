import pyautogui
import time
import random
import keyboard
from windowsDailyPolls import find_and_click_image


DELAY = 0.5 + random.uniform(0, 0.1)
CTRL_W = "ctrl+w"
SHOP_DONE = False


import re


def gamehag():
    global CENSUS, CENSUSANDEARN
    if CENSUS or CENSUSANDEARN:
        ocr_region("", 1695, 497, 1744, 525, None, 10)
    if CENSUS == False:
        with open("counter.txt", "r+") as f:
            lines = f.readlines()
            counter = int(lines[1])
            counter += 1
            print(counter)
            lines[1] = str(counter) + "\n"
            f.seek(0)
            f.writelines(lines)
        if counter % 7 == 0:
            find_and_click_image(
                f"{prepath}\\steamDailyChest.png",
                0,
                0,
                "down",
            )
            for _ in range(9):
                pyautogui.press("tab")
            pyautogui.press("space")
    close_application(1)


def extract_number_from_text(text):
    match = re.search(r"(\d+) answers", text)
    if match:
        return int(match.group(1))
    else:
        return 0


def toluna():
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\tolunaQuestion.png",
    )
    pyautogui.press("tab")
    pyautogui.press("space")
    if random.randint(0, 1) == 1:
        pyautogui.press("down")
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\tolunaVote.png",
        0,
        0,
        "down",
    )
    time.sleep(DELAY)
    keyboard.press_and_release(CTRL_W)


def lootup():
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\lootUpClaim.png",
    )
    time.sleep(DELAY + 1.5)
    keyboard.press_and_release(CTRL_W)


def coupert():
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\coupertOfferWall.png",
    )
    for _ in range(6):
        pyautogui.press("down")
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\coupertClick.png",
    )
    keyboard.press_and_release("ctrl+r")
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\coupertOfferWall.png",
    )
    for _ in range(21):
        pyautogui.press("down")
    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\coupertViewHotDeals.png",
        750,
    )
    while not SHOP_DONE:
        find_and_click_image(
            r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\coupertShop.png",
        )
        if not SHOP_DONE:
            time.sleep(DELAY)
            keyboard.press_and_release(CTRL_W)
    time.sleep(DELAY)
    keyboard.press_and_release(CTRL_W)
