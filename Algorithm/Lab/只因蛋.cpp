//#include<iostream>
//
//using namespace std;
//int n, e[250], s[250][250], maxans = 0;
//int main() {
//    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//    cin >> n; 
//    for (int i = 1; i <= n; i++) { cin >> e[i]; e[i + n] = e[i]; }
//    //重复存储一次环，便于连接开始和尾巴
//    for (int i = 2; i < 2 * n; i++) {
//        for (int j = i - 1; i - j < n && j >= 1; j--) {//dp的过程，从i开始向前推,变为子问题
//
//            //   |    A   |   B     |    C 
//            //       j(k)     i
//            //  e[j]    e[k+1]    e[i+1]
//            for (int k = j; k < i; k++)//k是待更新左右区间的划分点
//                s[j][i] = max(s[j][i], s[j][k] + s[k + 1][i] + e[j] * e[k + 1] * e[i + 1]);
//            //状态转移方程：max(原来能量，左区间能量+右区间能量+合并后生成能量）  
//            if (s[j][i] > maxans)maxans = s[j][i];//求最大值 
//        }
//    }
//    cout << maxans;
//    return 0;
//}