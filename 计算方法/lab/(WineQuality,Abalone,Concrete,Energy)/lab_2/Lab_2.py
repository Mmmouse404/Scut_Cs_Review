import pandas as pd
from sklearn.linear_model import LinearRegression, Ridge
from sklearn.model_selection import cross_val_score, KFold
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
import numpy as np

# 本地路径配置（用户需自行修改路径）
DATASET_PATHS = {
    'Concrete': './Concrete_Data.xls',  # 替换为本地路径
    'Energy': './ENB2012_data.xlsx'     # 替换为本地路径
}

# 加载混凝土强度数据集（本地路径）
def load_concrete():
    df = pd.read_excel(DATASET_PATHS['Concrete'])  # 注意使用 read_excel 读取 .xls 文件
    X = df.iloc[:, :-1].values
    y = df.iloc[:, -1].values
    return X, y

# 加载能源效率数据集（本地路径）
def load_energy():
    df = pd.read_excel(DATASET_PATHS['Energy'])
    X = df.iloc[:, :-2].values
    y = df.iloc[:, -2].values
    return X, y

datasets = {
    'Concrete': load_concrete(),
    'Energy': load_energy()
}

# ------------------------- 任务2：普通线性回归（无正则项） -------------------------
plt.figure(figsize=(10, 5))
for idx, (name, (X, y)) in enumerate(datasets.items(), 1):
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    model = LinearRegression()
    kf = KFold(n_splits=5, shuffle=True, random_state=42)
    scores = -cross_val_score(model, X_scaled, y, cv=kf, scoring='neg_mean_squared_error')
    mse = np.mean(scores)

    plt.subplot(1, 2, idx)
    plt.bar(['Linear Regression'], [mse])
    plt.title(f'{name} - Task 2: MSE (No Regularization)')
    plt.ylabel('Mean Squared Error')

plt.tight_layout()
plt.show()

# ------------------------- 任务3：岭回归（带L2正则项） -------------------------
plt.figure(figsize=(15, 6))
for idx, (name, (X, y)) in enumerate(datasets.items(), 1):
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    # 定义不同λ值的岭回归模型，并包含普通线性回归作为对比
    models = {
        'Linear': LinearRegression(),
        'Ridge (λ=0.1)': Ridge(alpha=0.1),
        'Ridge (λ=1)': Ridge(alpha=1),
        'Ridge (λ=10)': Ridge(alpha=10)
    }

    results = {}
    kf = KFold(n_splits=5, shuffle=True, random_state=42)

    for model_name, model in models.items():
        scores = -cross_val_score(model, X_scaled, y, cv=kf, scoring='neg_mean_squared_error')
        results[model_name] = np.mean(scores)

    plt.subplot(1, 2, idx)
    plt.bar(results.keys(), results.values())
    plt.xticks(rotation=45)
    plt.title(f'{name} - Task 3: MSE with L2 Regularization')
    plt.ylabel('Mean Squared Error')

plt.tight_layout()
plt.show()