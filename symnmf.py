import sys
import pandas as pd
import numpy as np
import mysymnmfsp as msm  # Importing the C extension
np.random.seed(0)

def defX(filename):
    """Loads the matrix from a file"""
    return pd.read_csv(filename, header=None, sep=r'\s+').astype(float).values.tolist()

def initial_H(W, n, k):
    """Initializes the H matrix"""
    W_np = np.array(W)
    m = np.mean(W_np)
    upper_bound = 2 * np.sqrt(m / k)
    H0_np = np.random.uniform(low=0, high=upper_bound, size=(n, k))
    H0 = H0_np.tolist()
    return H0

def printMatrix(M):
    """Prints the matrix in a formatted way"""
    for vector in M:
        ret_val = ""
        for cord in vector:
            ret_val = ret_val + str("{:.4f}".format(cord)) + ","
        ret_val = ret_val[:len(ret_val)-1]
        print(ret_val)
    return None

# Main logic
if len(sys.argv) == 3:
    goal = sys.argv[1]
    inputfile = sys.argv[2]
else:
    k = int(sys.argv[1])
    goal = sys.argv[2]
    inputfile = sys.argv[3]

X = defX(inputfile)
dim = len(X[0])
N = len(X)

if goal == "symnmf":
    W = msm.norm(X, N, dim)  # Normalized similarity matrix
    H0 = initial_H(W, N, k)  # Initial H matrix
    H = msm.symnmf(W, H0, k, N, dim)  # Perform SymNMF
    printMatrix(H)

elif goal == "sym":
    A = msm.sym(X, N, dim)  # Similarity matrix
    printMatrix(A)

elif goal == "ddg":
    D = msm.ddg(X, N, dim)  # Diagonal degree matrix
    printMatrix(D)

else:  # goal == "norm"
    W = msm.norm(X, N, dim)  # Normalized similarity matrix
    printMatrix(W)