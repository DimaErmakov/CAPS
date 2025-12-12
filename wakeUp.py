import webbrowser

# Register the Opera browser
webbrowser.register(
    "opera",
    None,
    webbrowser.BackgroundBrowser(
        r"C:\Users\ermak\AppData\Local\Programs\Opera\opera.exe"
    ),
)  # Replace the path with your Opera executable

# URL of the YouTube video
youtube_url = "https://www.youtube.com/watch?v=C82fI3zKcdQ&list=PLiGbATBS6ioMc5syFd7TCzmFJJKo347Zy&index=4"

# Open the URL in Opera
webbrowser.get("opera").open(youtube_url)
