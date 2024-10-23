import numpy as np
from mysymnmfsp import symnmf

def test_symnmf():
    # Initial matrices (adjust sizes as needed)
    H_initial = np.array([[0.5, 0.2], [0.3, 0.4]], dtype=np.float64)
    W_initial = np.array([[1.0, 2.0], [3.0, 4.0]], dtype=np.float64)
    
    # Number of iterations
    iterations = 100

    # Call the symnmf method
    final_H = symnmf(H_initial, W_initial, iterations)

    # Print the resulting H matrix
    print("Final H matrix:")
    print(final_H)

if __name__ == "_main_":
    test_symnmf()