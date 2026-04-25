import yfinance as yf
import pandas as pd
import numpy as np
import torch
import torch.nn as nn
from datetime import datetime, timedelta


def build_dataset():
    TIMEPERIOD = "20d"

    spx = yf.download("^GSPC", period=TIMEPERIOD, interval="5m", progress=False)[
        "Close"
    ]
    fut = yf.download("ES=F", period=TIMEPERIOD, interval="5m", progress=False)["Close"]
    vix = yf.download("^VIX", period=TIMEPERIOD, interval="5m", progress=False)["Close"]
    btc = yf.download("BTC-USD", period=TIMEPERIOD, interval="5m", progress=False)[
        "Close"
    ]

    df = pd.concat([spx, fut, vix, btc], axis=1)
    df.columns = ["spx", "fut", "vix", "btc"]
    df = df.dropna()

    df["futures_basis"] = df["fut"] - df["spx"]

    df["crypto_proxy"] = df["btc"].pct_change().rolling(12).mean()
    df["crypto_proxy"] = df["crypto_proxy"].fillna(0)

    df["vix_norm"] = df["vix"] / 20

    df["target"] = np.log(df["spx"]).diff().shift(-1)

    df = df.dropna()

    X = df[["spx", "futures_basis", "crypto_proxy", "vix_norm"]].values
    y = df["target"].values

    # normalize
    X_mean = X.mean(axis=0)
    X_std = X.std(axis=0) + 1e-8

    X = (X - X_mean) / X_std

    return X, y, X_mean, X_std


def fetch_inputs():

    spx = yf.Ticker("^GSPC")
    last_spx = spx.history(period="1d", interval="1m")["Close"].iloc[-1]

    futures = yf.Ticker("ES=F")
    futures_price = futures.history(period="1d", interval="1m")["Close"].iloc[-1]

    btc = yf.Ticker("BTC-USD")
    hist = btc.history(period="2d", interval="1h")
    crypto_proxy = hist["Close"].iloc[-1] / hist["Close"].iloc[-24]

    vix = yf.Ticker("^VIX")
    vix_proxy = vix.history(period="1d", interval="1m")["Close"].iloc[-1]

    return last_spx, futures_price, crypto_proxy, vix_proxy



def train_model():
    X, y, mean, std = build_dataset()

    X = torch.FloatTensor(X)
    y = torch.FloatTensor(y).view(-1, 1)

    dataset = torch.utils.data.TensorDataset(X, y)
    loader = torch.utils.data.DataLoader(dataset, batch_size=256, shuffle=True)

    model = nn.Sequential(
        nn.Linear(4, 64),
        nn.ReLU(),
        nn.Linear(64, 32),
        nn.ReLU(),
        nn.Linear(32, 1),
        nn.Tanh(),  # bound
    )

    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    loss_fn = nn.MSELoss()

    for epoch in range(100):
        total_loss = 0

        for xb, yb in loader:
            pred = model(xb)
            loss = loss_fn(pred, yb)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            total_loss += loss.item()

        if epoch % 10 == 0:
            print(f"Epoch {epoch}, Loss {total_loss:.6f}")

    return model, mean, std

def generate_forecast():
    model, mean, std = train_model()
    model.eval()

    last_spx, futures_price, crypto_proxy, vix_proxy = fetch_inputs()

    forecast_times = []
    prices = []

    current_time = datetime.strptime("09:30", "%H:%M")
    temp_price = last_spx

    for i in range(13):

        raw = np.array(
            [[temp_price, futures_price - temp_price, crypto_proxy, vix_proxy / 20]]
        )

        # normalize
        raw = (raw - mean) / std

        input_tensor = torch.FloatTensor(raw)

        with torch.no_grad():
            delta = model(input_tensor).item()

        # clamp output
        delta = max(min(delta, 0.002), -0.002)

        temp_price *= np.exp(delta)

        forecast_times.append(current_time.strftime("%H:%M"))
        prices.append(round(temp_price, 2))
        current_time += timedelta(minutes=5)

    pd.DataFrame({"time": forecast_times, "price": prices}).to_csv(
        "ErmakovDimitry.csv", index=False
    )


if __name__ == "__main__":
    generate_forecast()
