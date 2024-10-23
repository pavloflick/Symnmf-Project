import sys
import pandas as pd
import numpy as np
import mysymnmfsp as msm
from sklearn.metrics import silhouette_score
np.random.seed(0)

def defX(filename):
    return pd.read_csv(filename, header=None).values.tolist()

def initial_H(W, n, k):
    W_np = np.array(W)
    m = np.mean(W_np)
    upper_bound = 2 * np.sqrt(m / k)
    H0_np = np.random.uniform(0, upper_bound, size=(n, k))
    H0 = H0_np.tolist()
    return H0
    

def belong_point_to_centroid(centroidsH):
    max_column_indices = np.argmax(H, axis=1)
    max_column_indices += 1                         #add 1 because indecies are 0 - k-1 and centroids are 1 - k
    belong_lst = max_column_indices.tolist()
    return belong_lst


def kmeans(k, iter, input_data):
    # providing K "best" centroids of N points in R^d, allowing error of eps=0.001

    # store the vectors from the txt file in a list, each vector store as a list of d point
    points = []
    with open(input_data, 'r') as file:
        lines = file.read().split("\n")  # Read the file and split lines by newline character
        for line in lines:
            if line:  # Skip empty lines
                coordinates = line.strip().split(",")  # Split line by comma
                point = [float(coord) for coord in coordinates]  # Convert coordinates to floats
                points.append(point)  # Add the point to the list
    d = len(points[0])
    N = len(points)


    # k-mean algorithm
    centroids = points[:k]  # initialize centroids as first k data_points
    for t in range(iter):
        partition = []  # saves j for each point (from below) by order of points
        for pointt in points:
            dis_from_cents = [euc_dis(pointt, centroid) for centroid in centroids]  # list of distances from centroids
            j = dis_from_cents.index(min(dis_from_cents))  # the index of the closest centroid
            partition.append(j)  # store in partition

        if t == iter - 1:  # if its the last iteration we don't want to update the centroids
            centroids = [[float(f"{num:.4f}") for num in cenntroidd] for cenntroidd in centroids]
            break

        # initialize the new centroids
        new_centroids = []
        for j in range(k):
            points_sum = [0] * d  # sum of points that belong to cluster j
            cnt_point = 0  # num of points in cluster j
            for i in range(N):
                if partition[i] == j:
                    points_sum = vec_sum(points_sum, points[i])
                    cnt_point += 1
            new_centroid = [(1 / cnt_point) * point for point in points_sum]
            new_centroids.append(new_centroid)

        # check if all the distance between new centroid and old one is smaller than eps
        eps = 0.00001
        max_dist = 0  # store the maximum distance old centroid to new one
        for i in range(k):
            centroids_dis = euc_dis(centroids[i], new_centroids[i])
            max_dist = max(max_dist, centroids_dis)
        if max_dist < eps:
            break
        else:
            centroids = new_centroids
    centroids = [[float(f"{num:.4f}") for num in cenntroidd] for cenntroidd in centroids]
    return partition

def euc_dis(v1, v2):
    # providing the euclidean distance between 2 vectors in R^d
    dis = 0
    d = len(v1)
    for i in range(d):
        dis += (v1[i] - v2[i]) ** 2
    dis = dis ** 0.5
    return dis


def vec_sum(v1, v2):
    # providing the sum of 2 vectors
    d = len(v1)
    return [v1[i] + v2[i] for i in range(d)]


k = int(sys.argv[1])
inputfile = sys.argv[2]

X = defX(inputfile)
dim = len(X[0])
N = len(X)
W = msm.norm(X, N, dim)
H0 = initial_H(W, N, k)
H = msm.symnmf(W, H0, k, N, dim)
belong_lst = belong_point_to_centroid(H)


print("nmf:", str("{:.4f}".format(silhouette_score(X, belong_lst))))
print("kmeans:", str("{:.4f}".format(silhouette_score(X, kmeans(k, 300, inputfile)))))
