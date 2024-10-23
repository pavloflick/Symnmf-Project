# SymNMF
The final project of "Software Project" course.
In this project we will implement a clustering algorithm that is based on symmetric Non-negative Matrix Factorization (symNMF).
We will further apply it to several datasets and compare to Kmeans. 

## Introduction
SymNMF (Symmetric Non-negative Matrix Factorization) is a powerful clustering algorithm that finds application in various fields. This project involves implementing the SymNMF algorithm in Python and C and performing a comparative analysis against K-means. The goal is to create an efficient clustering solution for a given dataset, where the number of clusters is a parameter of the method.

## SymNMF Algorithm
SymNMF is based on the following algorithm:

1. Form the similarity matrix A from the given dataset.
2. Compute the Diagonal Degree Matrix.
3. Compute the normalized similarity matrix W.
4. Find the matrix H with n rows and k columns that minimizes the Frobenius norm of k * W - H * H^T.

## Project Files
The project includes the following files:
1. `symnmf.py`: Python interface for the SymNMF algorithm.
2. `symnmf.h`: C header file for the algorithm.
3. `symnmf.c`: C implementation of the SymNMF algorithm.
4. `symnmfmodule.c`: Python C API wrapper for SymNMF.
5. `analysis.py`: A script for analyzing and comparing SymNMF with K-means using scikit-learn's silhouette score.
6. `setup.py`: The setup file for the project.
7. `Makefile`: Make script for building the C interface.
8. `*.c/h`: Additional modules and headers as per your design.

## Comparative Analysis
To evaluate the effectiveness of SymNMF, comparative analysis is performed against K-means. Both methods are applied to the given dataset, and the silhouette score from scikit-learn's metrics is reported. The silhouette score measures clustering quality by comparing between-cluster distance against within-cluster distance. A higher score indicates better-defined clusters.

Silhouette Coefficient Formula:
Silhouette coefficient = (b - a) / max(a, b)

Where:
- 'a' represents the mean distance between a data point and all other points within its cluster.
- 'b' represents the minimum over all other clusters of the mean distance between the data point and all points in the other cluster.

By comparing SymNMF and K-means using the silhouette score, the project aims to determine which clustering method performs better on the given datasets.

Feel free to explore the code and experiment with the SymNMF algorithm for your clustering needs.




# 2.1
    case 1: symnmf  : python symnmf.py 3 symnmf data.txt
    case 2: python symnmf.py sym data.txt 
    case 3: python symnmf.py ddg data.txt
    case 4: python symnmf.py norm data.txt



