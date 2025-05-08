//#include <iostream>
//#include <queue>
//#include <unordered_map>
//#include <unordered_set>
//using namespace std;
//bool isSamePath(const unordered_set<int>& path1, const unordered_set<int>& path2) {
//    return path1 == path2;
//}
//int main() {
//    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//    int n, m;
//    cin >> n >> m;
//
//    unordered_map<int, int> inDegree; // 存储每个城市的入度
//    unordered_map<int, unordered_set<int>> graph; // 城市间的有向图
//    unordered_map<int, int> Degree; // 存储每个城市的入度
//    for (int i = 0; i < m; ++i) {
//        int u, v;
//        cin >> u >> v;
//
//        // 构建路径
//        unordered_set<int> path1 = graph[u];
//        path1.insert(u);
//        unordered_set<int> path2 = graph[v];
//        path2.insert(v);
//
//        // 判断路径是否相同
//        if (isSamePath(path1, path2)) {
//            continue; // 路径相同，不进行处理
//        }
//
//        graph[u].insert(v);
//        inDegree[v]++;
//        Degree[v]++;
//    }
//
//    unordered_map<int, int> dp; // 存储不同城市作为终点的旅行计划数
//
//    queue<int> q;
//    for (int i = 1; i <= n; ++i) {
//        if (inDegree[i] == 0) { // 找到起点城市
//            q.push(i);
//            dp[i] = 1; // 起点城市的旅行计划数为1
//        }
//    }
//
//    while (!q.empty()) {
//        int city = q.front();
//        q.pop();
//        for (int nextCity : graph[city]) {
//            dp[nextCity] += dp[city]; // 动态规划转移方程
//            inDegree[nextCity]--;
//            if (inDegree[nextCity] == 0) {
//                q.push(nextCity);
//            }
//        }
//    }
//
//    int totalTravelPlan = 0;
//    for (int i = 1; i <= n; ++i) {
//        if (graph[i].empty()) { // 终点城市
//            if(Degree[i]!=0)
//            totalTravelPlan += dp[i];
//        }
//    }
//
//    cout << totalTravelPlan << endl;
//
//    return 0;
//}
