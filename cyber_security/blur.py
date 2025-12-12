import cv2
import numpy as np
from numpy.fft import fftn, ifftn
import matplotlib.pyplot as plt
from numpy.typing import NDArray
from tqdm import tqdm
from scipy.signal import convolve


def load_bw(fname: str) -> NDArray:
    return cv2.imread(fname, cv2.IMREAD_GRAYSCALE)


def save_bw(fname: str, mat: NDArray):
    cv2.imwrite(fname, mat)


def img(*args, **kwargs):
    plt.imshow(*args, **kwargs, cmap="Greys_r")


def gauss2d(n_r: int, n_c: int, sigma: float) -> NDArray:
    assert n_r % 2 == 0 and n_c % 2 == 0
    gauss_r = np.exp(-np.linspace(-1.0, 1.0, n_r) ** 2 / sigma**2)
    gauss_c = np.exp(-np.linspace(-1.0, 1.0, n_c) ** 2 / sigma**2)
    res = (
        gauss_r.reshape(-1, 1)
        @ gauss_c.reshape(1, -1)
        / (np.sum(gauss_r) * np.sum(gauss_c))
    )
    return res


if __name__ == "__main__":
    data_pure = load_bw("pure.png")
    img(data_pure)
    n_gauss = 64
    data_blurred = convolve(
        data_pure, gauss2d(n_gauss, n_gauss, sigma=8.0), mode="same"
    )
    plt.figure()
    img(data_blurred)
    save_bw("blurred.png", data_blurred)
    plt.show()
