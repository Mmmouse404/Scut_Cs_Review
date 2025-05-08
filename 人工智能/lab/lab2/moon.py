
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import random
import math

plt.rcParams["font.sans-serif"] = ["SimHei"]

plt.rcParams["axes.unicode_minus"] = False
# 生成双月数据集
from sklearn.datasets import make_moons

# 生成2D分类数据集
X, y = make_moons(n_samples=1000, shuffle=True, noise=0.1, random_state=10)

# 将生成的数据存入 DataFrame 方便后续处理
df = pd.DataFrame(dict(x=X[:, 0], y=X[:, 1], label=y))

# 原始数据散点图，按照类值上色
colors = {0: 'red', 1: 'blue'}
fig, ax = plt.subplots()
grouped = df.groupby('label')

for key, group in grouped:
    group.plot(ax=ax, kind='scatter', x='x', y='y', label=key, color=colors[key])

plt.title("原始双月数据")
plt.show()

# DBSCAN 聚类
def epsilon(data, MinPts):
    m, n = np.shape(data)
    xMax = np.max(data, axis=0)
    xMin = np.min(data, axis=0)
    volume = np.prod(xMax - xMin)
    eps = ((volume * MinPts * math.gamma(0.5 * n + 1)) / (m * math.sqrt(math.pi ** n))) ** (1.0 / n)
    return eps

def distance(data):
    m, n = data.shape
    dis = np.zeros((m, m))
    for i in range(m):
        for j in range(i, m):
            dis[i, j] = np.linalg.norm(data[i] - data[j])
            dis[j, i] = dis[i, j]
    return dis

def find_eps(distance_D, eps):
    return np.where(distance_D <= eps)[0].tolist()

def dbscan(data, eps, MinPts):
    m = data.shape[0]
    labels = np.full(m, -1)  # -1 indicates noise
    visited = np.zeros(m, dtype=bool)
    current_label = 0
    dis = distance(data)

    for i in range(m):
        if not visited[i]:
            visited[i] = True
            neighbors = find_eps(dis[i], eps)

            if len(neighbors) < MinPts:
                labels[i] = -1  # Noise
            else:
                current_label += 1
                labels[i] = current_label

                for neighbor in neighbors:
                    if not visited[neighbor]:
                        visited[neighbor] = True
                        neighbors_next = find_eps(dis[neighbor], eps)
                        if len(neighbors_next) >= MinPts:
                            neighbors.extend(neighbors_next)
                    if labels[neighbor] == -1:
                        labels[neighbor] = current_label

    return labels

# KMeans 聚类
def kmeans(data, k, max_iters=100):
    m, n = data.shape
    centroids = data[np.random.choice(m, k, replace=False)]

    for _ in range(max_iters):
        distances = np.array([np.linalg.norm(data - centroid, axis=1) for centroid in centroids])
        labels = np.argmin(distances, axis=0)

        new_centroids = np.array([data[labels == i].mean(axis=0) for i in range(k)])
        if np.all(centroids == new_centroids):
            break
        centroids = new_centroids

    return labels

# 聚类参数
MinPts = 5
eps = epsilon(X, MinPts)

# DBSCAN 结果
dbscan_labels = dbscan(X, eps, MinPts)

# KMeans 结果
k = 2
kmeans_labels = kmeans(X, k)

# 绘制结果
fig, axs = plt.subplots(1, 2, figsize=(12, 5))

# DBSCAN 结果
axs[0].scatter(X[:, 0], X[:, 1], c=dbscan_labels, cmap='viridis', marker='o', s=10)
axs[0].set_title("DBSCAN 聚类结果")
axs[0].set_xlabel("特征1")
axs[0].set_ylabel("特征2")

# KMeans 结果
axs[1].scatter(X[:, 0], X[:, 1], c=kmeans_labels, cmap='viridis', marker='o', s=10)
axs[1].set_title("KMeans 聚类结果")
axs[1].set_xlabel("特征1")
axs[1].set_ylabel("特征2")

plt.tight_layout()
plt.show()
