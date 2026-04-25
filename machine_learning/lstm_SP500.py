from torch.utils.data import DataLoader, TensorDataset
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
from numpy.typing import NDArray
import numpy as np
import pandas as pd
import torch
import torch.nn as nn
from tqdm import tqdm
import argparse
import yfinance as yf
import datetime
import csv

parser = argparse.ArgumentParser()
parser.add_argument("--cuda", type=int)
parser.add_argument("--lr", type=float, default=0.005)
parser.add_argument("--bs", type=int, default=32)
parser.add_argument("--n_epochs", type=int, default=100)
parser.add_argument("--save_parameters", action="store_true")
args = parser.parse_args()


if torch.cuda.is_available() and args.cuda is not None:
    dev = torch.device(f"cuda:{args.cuda}")
else:
    dev = torch.device("cpu")

print("Fetch data from Yahoo Finance")
tickers: list[str] = ["^GSPC", "ES=F", "BTC-USD", "^VIX"]
df_raw: pd.DataFrame = yf.download(tickers, period="60d", interval="5m")["Close"]
df: pd.DataFrame = df_raw.ffill().dropna()

feature_data: NDArray = df.values
target_data: NDArray = df["^GSPC"].values.reshape(-1, 1)

sc_features: MinMaxScaler = MinMaxScaler()
sc_target: MinMaxScaler = MinMaxScaler()

scaled_features: NDArray = sc_features.fit_transform(feature_data)
scaled_target: NDArray = sc_target.fit_transform(target_data).reshape(-1)

k_in: int = 24
k_out: int = 13
n_layers: int = 2
num_features: int = 4


def all_windows_in_and_out(features: NDArray, target: NDArray, k_in: int, k_out: int):
    res_x: list[NDArray] = []
    res_y: list[NDArray] = []
    for i in range(len(features) - k_in - k_out + 1):
        res_x.append(features[i : i + k_in])
        res_y.append(target[i + k_in : i + k_in + k_out])

    dt: torch.dtype = torch.float32
    return torch.tensor(np.array(res_x), dtype=dt), torch.tensor(
        np.array(res_y), dtype=dt
    )


x, y = all_windows_in_and_out(scaled_features, scaled_target, k_in, k_out)

x_train, x_val, y_train, y_val = train_test_split(x, y, test_size=0.1, shuffle=False)


class RecurrentModel(nn.Module):
    def __init__(
        self, num_features: int, hidden_size: int, k_out: int, n_layers: int, mode: str
    ):
        super().__init__()
        self.mode = mode

        self.rnn = torch.nn.GRU(
            num_features,
            hidden_size,
            batch_first=True,
            num_layers=n_layers,
            dropout=0.1,
        )

        self.fc = nn.Linear(hidden_size, k_out)

    def forward(self, x):
        _, h_out = self.rnn(x)

        out = self.fc(h_out[-1])
        return out


func_loss: torch.nn.MSELoss = torch.nn.MSELoss()
ds: TensorDataset = TensorDataset(x_train, y_train)
dl: DataLoader = DataLoader(ds, batch_size=args.bs, shuffle=True)

model = RecurrentModel(
    num_features=num_features,
    hidden_size=64,
    k_out=k_out,
    n_layers=n_layers,
    mode="gru",
)
model: RecurrentModel = model.to(dev)

OptimizerType: type = torch.optim.Adam
optimizer: torch.optim.Optimizer = OptimizerType(model.parameters(), lr=args.lr)

print("Training Model")
pbar: tqdm = tqdm(range(args.n_epochs))
for epoch in pbar:
    err_avg: float = 0.0
    model.train()
    for x_batch, y_batch in dl:
        x_batch: torch.Tensor = x_batch.to(dev)
        y_batch: torch.Tensor = y_batch.to(dev)

        y_hat = model(x_batch)

        optimizer.zero_grad()
        err: torch.Tensor = func_loss(y_hat, y_batch)
        err_avg += err.detach().item()

        err.backward()
        optimizer.step()

    err_avg /= len(dl)
    if epoch % 10 == 0:
        pbar.set_description(f"Loss: {err_avg:.5f}")

model.eval()
with torch.no_grad():
    y_train_hat: torch.Tensor = model(x_train.to(dev))
    y_val_hat: torch.Tensor = model(x_val.to(dev))

print("Loss (train):", func_loss(y_train_hat.cpu(), y_train).item())
print("Loss (val):  ", func_loss(y_val_hat.cpu(), y_val).item())

print("Generate forecast for 9:30am to 10:30am")
model.eval()
with torch.no_grad():
    last_window: torch.Tensor = (
        torch.tensor(scaled_features[-k_in:], dtype=torch.float32).unsqueeze(0).to(dev)
    )
    future_prediction_scaled: np.ndarray = (
        model(last_window).cpu().numpy().reshape(-1, 1)
    )

    future_prediction: np.ndarray = sc_target.inverse_transform(
        future_prediction_scaled
    ).flatten()

start_time: datetime.datetime = datetime.datetime(2026, 4, 24, 9, 30)
times: list[datetime.time] = [
    (start_time + datetime.timedelta(minutes=5 * i)).time() for i in range(k_out)
]

filename = "ErmakovDimitry.csv"

with open(filename, mode="w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["time", "price"])
    for t, p in zip(times, future_prediction):
        writer.writerow([t.strftime("%H:%M"), f"{p:.2f}"])

print(f"Predictions saved to {filename}")
