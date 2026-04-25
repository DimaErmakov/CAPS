import numpy as np
from numpy.typing import NDArray
import pandas as pd
import torch
import torch.nn as nn
from torch.utils.data import DataLoader, TensorDataset
from sklearn.preprocessing import MinMaxScaler
from sklearn.model_selection import train_test_split

from scipy.signal import detrend

from tqdm import tqdm

import matplotlib.pyplot as plt

import argparse

parser = argparse.ArgumentParser()

parser.add_argument("--cuda", type=int)
parser.add_argument("--lr", type=float, default=0.005)
parser.add_argument("--bs", type=int, default=16)
parser.add_argument("--n_epochs", type=int, default=1000)
parser.add_argument("--save_parameters", action="store_true")
parser.add_argument("--plot", action="store_true")

args = parser.parse_args()

if torch.cuda.is_available() and args.cuda is not None:
    dev = torch.device(f"cuda:{args.cuda}")
else:
    dev = torch.device("cpu")

# TODO: would be cooler to model S&P 500
df_passengers: pd.DataFrame = pd.read_csv("airline_passengers.csv")
n_passengers_s: NDArray = df_passengers.Passengers.to_numpy()

n_passengers_detrended_exp_s: NDArray = np.exp(detrend(np.log(n_passengers_s)))


def all_windows_in_and_out(x: NDArray, k_in: int, k_out: int) -> NDArray:
    res_x = []
    res_y = []
    k: int = 5
    for i in range(len(x) - k_in - k_out + 1):
        res_x.append(x[i : i + k_in])
        res_y.append(x[i + k_in : i + k_in + k_out])
    dt = torch.float32
    return torch.tensor(np.array(res_x), dtype=dt), torch.tensor(
        np.array(res_y), dtype=dt
    )


sc = MinMaxScaler()
signal = sc.fit_transform(n_passengers_detrended_exp_s.reshape(-1, 1)).reshape(-1)

# NOTE: to inverse transform, use the following
# data_predict = sc.inverse_transform(data_predict.reshape(-1,1))
# dataY_plot = sc.inverse_transform(dataY_plot)

k_in: int = 9
k_out: int = 100
n_layers: int = 5
x, y = all_windows_in_and_out(signal, k_in, k_out)

# NOTE: possible information leakage due to a few windows with intersecting input and output
x_train, x_val, y_train, y_val = train_test_split(x, y, test_size=0.1, shuffle=False)


class RecurrentModel(nn.Module):
    def __init__(self, k_in: int, k_out: int, n_layers: int, mode: str):
        super().__init__()
        self.k_in = k_in
        self.k_out = k_out

        self.mode = mode
        if mode == "gru":
            # k_in previous days go in, k_out subsequent days go out
            self.rnn = torch.nn.GRU(
                k_in, k_out, batch_first=True, num_layers=n_layers, dropout=0.1
            )
        elif mode == "lstm":
            self.rnn = torch.nn.LSTM(
                k_in, k_out, batch_first=True, num_layers=n_layers, dropout=0.1
            )
        else:
            raise ValueError(f"{mode=} is not valid")

    def forward(self, x):
        size_batch = x.shape[0]

        if self.mode == "gru":
            y, h_out = self.rnn(x)
        elif self.mode == "lstm":
            y, (h_out, _) = self.rnn(x)
        return y


func_loss = torch.nn.MSELoss()  # mean-squared error for regression

ds = TensorDataset(x_train, y_train)
dl = DataLoader(ds, batch_size=args.bs, shuffle=True)

all_name_to_param_and_grad: list[dict[str, torch.tensor]] = []


def get_named_param_and_grad_s(model):
    # clone tensors to be on the safe side (in case some other process modifies them in place):
    return {
        name: (param.clone(), param.grad.clone())
        for name, param in model.named_parameters()
    }


for OptimizerType in [torch.optim.SGD, torch.optim.Adam]:
    model = RecurrentModel(k_in, k_out, n_layers=n_layers, mode="gru")
    model = model.to(dev)

    optimizer = OptimizerType(model.parameters(), lr=args.lr)

    print(OptimizerType)
    pbar = tqdm(range(args.n_epochs))
    for epoch in pbar:
        err_avg = 0.0
        model.train()
        for x_batch, y_batch in dl:
            x_batch = x_batch.to(dev)
            y_batch = y_batch.to(dev)
            y_hat = model(x_batch)

            optimizer.zero_grad()
            # obtain the loss function
            err = func_loss(y_hat, y_batch)
            err_avg += err.detach().item()

            err.backward()
            optimizer.step()

        err_avg /= len(dl)
        if epoch % 100 == 0:
            pbar.set_description(f"{err_avg=:.5f}")

        if args.save_parameters:
            all_name_to_param_and_grad.append(get_named_param_and_grad_s(model))

    model.eval()
    with torch.no_grad():
        # repeat so that we have data with eval(), which disables dropout
        y_train_hat = model(x_train.to(dev))
        y_val_hat = model(x_val.to(dev))

    print("Loss (train):", func_loss(y_train_hat.cpu(), y_train))
    print("Loss (val):  ", func_loss(y_val_hat.cpu(), y_val))

    if args.save_parameters:
        # get name of optimizer type: hacky, but fine for now:
        name_opt: str = str(OptimizerType).split("'")[1].split(".")[-1].lower()

        fname = f"{name_opt}_params_and_grads.pth"
        torch.save(all_name_to_param_and_grad, fname)

    if args.plot:
        plt.figure()
        plt.plot(y_val[-1])
        # TODO: not important to the project itself, but it would be nice to
        # plot entire detrended result but with hold out data a different
        # color
        plt.plot(y_val_hat.cpu()[-1].detach().numpy())
        plt.title(str(OptimizerType))

if args.plot:
    plt.show()
