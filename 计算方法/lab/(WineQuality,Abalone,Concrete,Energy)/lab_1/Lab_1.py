import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import f1_score
from imblearn.over_sampling import SMOTE
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt

# 本地路径配置
DATASET_PATHS = {
    # 从UCI下载葡萄酒质量数据集：https://archive.ics.uci.edu/dataset/186/wine+quality
    'WineQuality': './winequality-red.csv',
    # https://archive.ics.uci.edu/dataset/1/abalone
    'Abalone': './abalone.data'
}


def load_winequality():
    df = pd.read_csv(DATASET_PATHS['WineQuality'], sep=';')

    # 创建二元标签：quality >= 7 为高质量（1），否则为0
    y = (df['quality'] >= 7).astype(int)
    X = df.drop(columns=['quality']).values
    print("类别分布:", pd.Series(y).value_counts())  # 检查不平衡程度
    return X, y


def load_abalone():
    df = pd.read_csv(DATASET_PATHS['Abalone'], header=None)
    df[0] = df[0].map({'M': 0, 'F': 1, 'I': 2})
    X = df.iloc[:, :-1].values
    y = (df.iloc[:, -1] > 10).astype(int)
    return X, y


datasets = {
    'WineQuality': load_winequality(),
    'Abalone': load_abalone()
}

# 决策树参数和图表设置
depths = [3, 5, 7, None]
plt.figure(figsize=(15, 8))
for idx, (name, (X, y)) in enumerate(datasets.items(), 1):
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)

    sm = SMOTE(random_state=42)
    X_res, y_res = sm.fit_resample(X_train_scaled, y_train)

    # 训练决策树并记录结果
    dt_results = {'Original': [], 'SMOTE': []}
    for depth in depths:
        clf = DecisionTreeClassifier(max_depth=depth, random_state=42)
        # 原始数据
        clf.fit(X_train_scaled, y_train)
        y_pred = clf.predict(X_test_scaled)
        dt_results['Original'].append(f1_score(y_test, y_pred))
        # SMOTE数据
        clf.fit(X_res, y_res)
        y_pred = clf.predict(X_test_scaled)
        dt_results['SMOTE'].append(f1_score(y_test, y_pred))

    # 绘制决策树结果
    plt.subplot(1, 2, idx)
    x_labels = [str(d) if d else 'No Limit' for d in depths]
    x_ticks = np.arange(len(depths))
    plt.plot(x_ticks, dt_results['Original'], marker='o', label='Original')
    plt.plot(x_ticks, dt_results['SMOTE'], marker='o', label='SMOTE')
    plt.xticks(x_ticks, x_labels)
    plt.title(f'{name} - Decision Tree (F1 Score)')
    plt.xlabel('Max Depth')
    plt.ylabel('F1 Score')
    plt.legend()

plt.tight_layout()
plt.show()

# 贝叶斯参数和图表设置
var_smoothings = [1e-9,1e-6, 1e-3]
plt.figure(figsize=(15, 8))
for idx, (name, (X, y)) in enumerate(datasets.items(), 1):
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)
    scaler = StandardScaler()
    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    sm = SMOTE(sampling_strategy='not majority')
    X_res, y_res = sm.fit_resample(X_train_scaled, y_train)

    # 训练贝叶斯并记录结果（保持不变）
    nb_results = {'Original': [], 'SMOTE': []}
    for vs in var_smoothings:
        clf = GaussianNB(var_smoothing=vs)
        # 原始数据
        clf.fit(X_train_scaled, y_train)
        y_pred = clf.predict(X_test_scaled)
        nb_results['Original'].append(f1_score(y_test, y_pred))
        # SMOTE数据
        clf.fit(X_res, y_res)
        y_pred = clf.predict(X_test_scaled)
        nb_results['SMOTE'].append(f1_score(y_test, y_pred))
        # 打印调试信息

    # 绘制贝叶斯结果（保持不变）
    plt.subplot(1, 2, idx)
    x_ticks = np.arange(len(var_smoothings))
    width = 0.35
    bars_orig = plt.bar(x_ticks - width / 2, nb_results['Original'], width, label='Original')
    bars_smote = plt.bar(x_ticks + width / 2, nb_results['SMOTE'], width, label='SMOTE')
    # 在柱子顶部添加数值标签
    for bars, values in zip([bars_orig, bars_smote], [nb_results['Original'], nb_results['SMOTE']]):
        for bar, value in zip(bars, values):
            height = bar.get_height()
            plt.text(
                bar.get_x() + bar.get_width() / 2,  # x坐标：柱子中心
                height + 0.01,  # y坐标：柱子顶部上方0.01
                f'{value:.3f}',  # 显示值保留3位小数
                ha='center',  # 水平居中
                va='bottom'  # 垂直底部对齐
            )
    plt.xticks(x_ticks, [f'vs={vs:.0e}' for vs in var_smoothings])
    plt.title(f'{name} - Naive Bayes (F1 Score)')
    plt.xlabel('Var Smoothing')
    plt.ylabel('F1 Score')
    plt.legend()

plt.tight_layout()
plt.show()