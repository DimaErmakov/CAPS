import re
import sys
import keyboard
import pyautogui
import webbrowser
import tkinter as tk
from tkinter import simpledialog
import gspread
from google.oauth2.service_account import Credentials
from windowsDailyPolls import find_and_click_image
from datetime import datetime

SPREADSHEET_PATH = (
    r"C:\Users\ermak\OneDrive\Documents\STOCKWISE\moneytracker_client_secret.json"
)
SPREADSHEET_NAME = "Money Tracker"
IMAGE_PATHS = {
    "sign_in": r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\signInEntry.png",
    "employee": r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\employee.png",
    "time_entry": r"C:\Users\ermak\OneDrive\Documents\ATDP\windowsImages\timeEntry.png",
}


def main():
    import time

    find_and_click_image(
        r"C:\Users\ermak\OneDrive\Documents\ATDP\gift_card\chrome_yr1WCSoeQg.png"
    )
    pyautogui.press("tab")
    pyautogui.press("enter")
    time.sleep(2)
    pyautogui.press("tab")
    keyboard.write("I did not know the event was occuring")
    pyautogui.press("tab", presses=2)
    keyboard.write("Dimitry Ermakov")
    pyautogui.press("tab")
    keyboard.write("dermakov@hillsdale.edu")
    pyautogui.press("tab", presses=2)
    pyautogui.press("enter")


if __name__ == "__main__":
    main()
