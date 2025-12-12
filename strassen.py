import numpy as np
import time


def strassen(A, B, LEAF_SIZE=1):

    if len(A) == LEAF_SIZE:
        return A @ B

    n = len(A) // 2
    A11, A12, A21, A22 = A[:n, :n], A[:n, n:], A[n:, :n], A[n:, n:]
    B11, B12, B21, B22 = B[:n, :n], B[:n, n:], B[n:, :n], B[n:, n:]

    P1 = strassen((A11 + A12), B22)
    P2 = strassen(A11, (B12 - B22))
    P3 = strassen((A21 + A22), B11)
    P4 = strassen(A22, (B21 - B11))
    P5 = strassen((A11 + A22), (B11 + B22))
    P6 = strassen((A12 - A22), (B21 + B22))
    P7 = strassen((A21 - A11), (B11 + B12))

    C11 = P5 + P4 - P1 + P6
    C12 = P2 + P1
    C21 = P3 + P4
    C22 = P5 + P2 - P3 + P7

    top = np.hstack((C11, C12))
    bottom = np.hstack((C21, C22))
    C = np.vstack((top, bottom))

    return C


def main():

    n = 16
    A = np.random.randint(0, 32, n * n).reshape(n, n)
    B = np.random.randint(0, 32, n * n).reshape(n, n)
    start_time = time.time()
    C = strassen(A, B, 2)
    end_time = time.time()
    print(f"strassen: {end_time - start_time}")
    start_time = time.time()
    exact = A @ B
    end_time = time.time()
    print(f"naive: {end_time - start_time}")
    print("absolute error:", max(np.fabs(np.array(C - exact)).flatten()))


if __name__ == "__main__":
    main()
