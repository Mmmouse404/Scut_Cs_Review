//#include<iostream>
//#include<algorithm>
//using namespace std;
//
//long long dp[30][30];
//int dx[8] = { -2, -2, -1, -1, 1, 1, 2, 2 }; //�Ǻ�
//int dy[8] = { 1, -1, 2, -2, 2, -2, 1, -1 }; //����
//
//int main() {
//    int n, m, x, y;
//    cin >> n >> m >> x >> y;
//    n++;m++;
//    x++;y++;  // ����ճ�һ��һ�У����ڱ߽���
//
//    for (int i = 0; i < 8; i++) {
//        if (x + dx[i] >= 1 && x + dx[i] <= n && y + dy[i] >= 1 && y + dy[i] <= m) {//���ǵĻ��Χ�ڱ��λ-1
//            dp[x + dx[i]][y + dy[i]] = -1;
//        }
//    }
//    dp[1][0] = 1;
//    dp[x][y] = -1;  
//
//    for (int i = 1; i <= n; i++) {
//        for (int j = 1; j <= m; j++) {
//            if (dp[i][j] == -1) {
//                dp[i][j] = 0;
//            }
//            else {
//                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
//            }
//        }
//    }
//
//    cout << dp[n][m] << endl;
//
//    return 0;
//}
