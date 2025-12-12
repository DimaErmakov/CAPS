from blur import *

blurred = load_bw("quack.png").astype(np.float32)


# ideally we'd somply deconvolve; however, the floating point values
# are rounded to ints in the set [0,255] when saving as .png. these
# small changes cause numeric instability when dividing
# fftn(prod)/fftn(factor) during deconvolution; factor, our Gaussian,
# may be close to zero but the product (the blurred image) has been
# adjusted by this quantization and is no longer so. we proceed with a
# steepest descent approach to find the unblurred image that would
# blur into the blurred image (as described in
# https://stackoverflow.com/questions/53743561/deblurring-an-image):
def loss(recovered):
    return np.mean((convolve(recovered, kernel, mode="same") - blurred) ** 2)


# NOTE: gradient descent would likely be more efficient on a GPU:
def gradient(recovered):
    return convolve(
        convolve(recovered, kernel, mode="same") - blurred, kernel.T, mode="same"
    )


n_gauss = 64
lr: float = 1.0
momentum: float = 0.9
delta = None
# sigma (for a fixed Gaussian kernel size, this determines the amount
# of blur) is unknown; try a few and use the one that best
# generatively reconstructs the blurred image:
reconst_err_best: float = np.inf

# in practice, could try more sigmas here:
for sigma in [4, 8, 16]:
    print(f"trying {sigma=}")

    kernel = gauss2d(n_gauss, n_gauss, sigma=sigma).astype(np.float32)

    # for speed, print and re-draw only ever 32nd epoch:
    interval_print = 2**5

    print("error when reproducing blurred image:")
    recovered = blurred.copy()
    for epoch in range(2**11):
        if delta is None:
            delta = lr * gradient(recovered)
        else:
            delta = lr * gradient(recovered) + momentum * delta
        recovered -= delta
        if epoch % interval_print == interval_print - 1:
            err: float = loss(recovered)
            print(f"\t{err:.4f}")
            img(recovered)
            plt.pause(0.01)
            if err < reconst_err_best:
                reconst_err_best = err
                save_bw("best_reconstruction.png", recovered)
