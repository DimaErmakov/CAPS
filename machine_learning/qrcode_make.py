import qrcode

img = qrcode.make(
    "https://docs.google.com/forms/d/e/1FAIpQLSetTIw8UnAdx6zTt9JEgpfFpY3clxpZFfH2FIF_Fbs8p_WFJw/viewform?usp=header"
)
img.save(r"C:\Users\ermak\Downloads\qrcode.png")
