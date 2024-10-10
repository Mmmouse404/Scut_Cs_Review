#include <bits/stdc++.h>
using namespace std;

int ary[6][9];  // ���彩ʬͼ
int dx[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };  // ����ƫ����
int dy[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };  // ����ƫ����
int mincost = INT_MAX;  

bool check(int x) {  // ����Ƿ�Ϊ����λ��
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

    int temp[6][9];  // ��ʱ���潩ʬͼ
    memcpy(temp, ary, sizeof(temp));  // ���ƽ�ʬͼ״̬
    int zombies_cleared = 0;

    for (int i = 0; i < 9; ++i) {
        int nx = x + dx[i];  // ����ƫ���������µĺ�����
        int ny = y + dy[i];  // ����ƫ���������µ�������
        if (nx >= 0 && nx < 6 && ny >= 0 && ny < 9 && ary[nx][ny] == 1) {  // �ж��Ƿ��н�ʬ
            ary[nx][ny] = 0;  // ����ʬ
            zombies_cleared++;
        }
    }

    if (zombies_cleared > 0) { // ����н�ʬ������������ݹ�
        if (check(x)) {
            dfs(x, y + 1, num - zombies_cleared, cost + 175);  // ����ǳ���λ�ã����ֺ�Ҷ
        }
        else {
            dfs(x, y + 1, num - zombies_cleared, cost + 150);  // ����ֻ��ӣ��ը��
        }
    }

    memcpy(ary, temp, sizeof(ary));  // �ָ���ʬͼ״̬
    dfs(x, y + 1, num, cost);  // �����ݹ�
}

void solve() {
    int sum = 0;  // ��¼��ʬ������
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 9; ++j) {
            cin >> ary[i][j];  // ���뽩ʬͼ
            if (ary[i][j] == 1) {
                sum++;
            }
        }
    }
    dfs(1, 0, sum, 0); // �ӵڶ��п�ʼ����
    cout << mincost << endl; 
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
    solve(); 
    return 0;
}
