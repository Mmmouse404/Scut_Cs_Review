//#include <iostream>
//#include <algorithm>
//#define ll long long
//using namespace std;
//const int maxn = 1e6;
//int T, n, m, hd, ed;
//int a[maxn], l[maxn], q[maxn];
//ll f[maxn];
//
//int main()
//{
//    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//    cin >> n;
//    cin >> m;
//    for (int i = 1; i <= n; ++i)    cin >> a[i], l[i] = 0;
//    for (int i = 1, lt, rt; i <= m; ++i) {
//        cin >> lt >> rt;
//        l[rt] = max(l[rt], lt);//确定包含区间的最右左端点
//    }
//    q[hd = ed = 1] = 0;
//    a[++n] = 0;//a[n+1]=0
//    for (int i = 1; i <= n; ++i) {
//        f[i] = f[q[hd]] + a[i];//状态转移f[i]=minf[j]+a[i]
//        while (hd <= ed && f[q[ed]] > f[i])   --ed;//维护单调队列
//        q[++ed] = i;
//        while (q[hd] < l[i])    ++hd;
//    }
//    cout << f[n] << endl;
//    return 0;
//}
//
