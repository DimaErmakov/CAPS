import pandas as pd
import datetime as dt
import matplotlib.pyplot as plt
import numpy as np


def _circ_array(x: np.array, n_cols: int) -> np.array:
    return np.array([x[i : i + n_cols] for i in range(len(x) - n_cols + 1)])


def estimate_and_print_sp500_recurrence(sp500: np.array, order: int):
    c_train: np.array = _circ_array(sp500, order)

    # solve(A, b) --> x : A x = b, so find what multiplies with old data to make next data:
    A: np.array = np.linalg.solve(
        c_train[:-1].T @ c_train[:-1], c_train[:-1].T @ c_train[1:]
    )
    eig_dom: float = sorted(np.abs(np.linalg.eigvals(A.T)))[-1]
    print("dominant eigenval:", eig_dom)
    print("yearly rate:", eig_dom**252)

    print(A.T[::-1, ::-1])
    recurrence = A.T[::-1, ::-1][0]
    terms: list[str] = [f"{val:4}*f[n-{i+1}]" for i, val in enumerate(recurrence)]
    formula: str = " + ".join(terms)
    print()
    print("Fit recurrence formula:")
    print("f[n] =", formula)
    print()

    plt.plot(sp500, label="S&P 500")
    last_points = c_train[0]
    points_val = []
    for i in range(len(sp500)):
        last_points = last_points @ A
        points_val.append(last_points)

    points_val = np.array(points_val)
    pred = points_val[:, -1]

    plt.plot(
        np.arange(order, len(sp500) + order),
        pred,
        label=f"Order $k=${order} recurrence",
    )
    plt.legend()
    print("[close plot window to continue]")
    plt.show()
    return eig_dom, A.T[::-1, ::-1]


### THE ABOVE CODE IS PROVIDED FOR YOUR CONVENIENCE TO USE AS YOU LIKE.

# FIXME: load daily S&P500 closing prices from sp500.csv using that
# and the code above, fit and print a 4th-order recurrent model to
# daily S&P 500 prices. this will resemble price(n) =
# some_value*price(n-1) + ... + another_value*price(n-4). implement a
# recursive function that uses this recurrent model to predict the
# closing price of the S&P 500 at an arbitrary day n. start with the
# most recent 4 days as n=0, n=1, n=2, and n=3 where n=3 is the most
# recent day for which we have data here.
# Assuming sp500.csv contains a column 'Date' with dates and 'Close' with closing prices
# Step 1: Load daily S&P 500 closing prices from sp500.csv
# Step 1: Load daily S&P 500 closing prices from sp500.csv
# Load only 'DATE' and 'SP500' columns, parsing 'DATE' as the date
data = pd.read_csv("sp500.csv", parse_dates=["DATE"])
data = data.dropna(subset=["SP500"])  # Drop rows with NaN values in 'SP500'
sp500 = data["SP500"].values  # Extract the closing prices as a NumPy array

# Step 2: Estimate a 4th-order recurrent model
order = 4
eig_dom, A_matrix = estimate_and_print_sp500_recurrence(sp500, order)


# Step 3: Implement a recursive function to predict S&P 500 price at an arbitrary day n
def predict_sp500_price(
    n: int, recent_prices: np.array, coefficients: np.array
) -> float:

    # Base case: If n is within the range of initial recent prices, return that price
    if n < 4:
        return recent_prices[n]

    # Recursive case: Compute the next price using the recurrence relation
    next_price = 0
    for i in range(4):
        next_price += coefficients[i] * predict_sp500_price(
            n - i - 1, recent_prices, coefficients
        )

    return next_price


# Extract the coefficients from the recurrence formula for predictions
coefficients = A_matrix[0]  # The coefficients of the recurrence relation

# Step 4: Test the recursive function with the most recent 4 prices in sp500
recent_prices = sp500[-4:]  # Most recent 4 days' prices
predicted_price = predict_sp500_price(
    5, recent_prices, coefficients
)  # Predicting price for day 5
print(f"Predicted S&P 500 price for day 5: {predicted_price:.2f}")

# # FIXME: using your recurrence, compute the expected rate of return in
# # a stock over the first year, starting at day n=4. note that a year
# # has 252 trading days.
expected_rate_of_return = eig_dom**252
print(f"Expected rate of return over the first year: {expected_rate_of_return:.2f}")

# # FIXME: now let's use your estimated rate of return. consider
# # starting with a principal of $0 on your 25th birthday. each year,
# # interest accrues and is reinvested. after that, you contribute a
# # lump sum of $5000 on your birthday each year. how much do you expect
# # your account to contain after you deposit your lump sump payment on
# # your 50th birthday?
principal = 0
annual_contribution = 5000
years = 50 - 25

for year in range(years):
    principal = (principal + annual_contribution) * (1 + expected_rate_of_return)

print(f"Expected account balance after 50th birthday: ${principal:.2f}")
