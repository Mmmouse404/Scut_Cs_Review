#include <bits/stdc++.h>
using namespace std;

int ary[6][9];  // 定义僵尸图
int dx[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };  // 横向偏移量
int dy[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };  // 纵向偏移量
int mincost = INT_MAX;  

bool check(int x) {  // 检查是否为池塘位置
    return x == 2 || x == 3;
}

void dfs(int x, int y, int num, int cost) {  
    if (x >= 5 || num == 0) {
        if (num == 0)
        {
            mincost = min(mincost, cost);
        }
        return;
    }
    if (y >= 9) {
        dfs(x + 1, 0, num, cost);
        return;
    }
    if (cost >= mincost) {
        return;
    }

    int temp[6][9];  // 临时保存僵尸图
    memcpy(temp, ary, sizeof(temp));  // 复制僵尸图状态
    int zombies_cleared = 0;

    for (int i = 0; i < 9; ++i) {
        int nx = x + dx[i];  // 根据偏移量计算新的横坐标
        int ny = y + dy[i];  // 根据偏移量计算新的纵坐标
        if (nx >= 0 && nx < 6 && ny >= 0 && ny < 9 && ary[nx][ny] == 1) {  // 判断是否有僵尸
            ary[nx][ny] = 0;  // 消灭僵尸
            zombies_cleared++;
        }
    }

    if (zombies_cleared > 0) { // 如果有僵尸被消灭则继续递归
        if (check(x)) {
            dfs(x, y + 1, num - zombies_cleared, cost + 175);  // 如果是池塘位置，则种荷叶
        }
        else {
            dfs(x, y + 1, num - zombies_cleared, cost + 150);  // 否则只种樱桃炸弹
        }
    }

    memcpy(ary, temp, sizeof(ary));  // 恢复僵尸图状态
    dfs(x, y + 1, num, cost);  // 继续递归
}

void solve() {
    int sum = 0;  // 记录僵尸的数量
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 9; ++j) {
            cin >> ary[i][j];  // 读入僵尸图
            if (ary[i][j] == 1) {
                sum++;
            }
        }
    }
    dfs(1, 0, sum, 0); // 从第二行开始搜索
    cout << mincost << endl; 
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    solve(); 
    return 0;
}
