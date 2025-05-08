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
//    unordered_map<int, int> inDegree; // �洢ÿ�����е����
//    unordered_map<int, unordered_set<int>> graph; // ���м������ͼ
//    unordered_map<int, int> Degree; // �洢ÿ�����е����
//    for (int i = 0; i < m; ++i) {
//        int u, v;
//        cin >> u >> v;
//
//        // ����·��
//        unordered_set<int> path1 = graph[u];
//        path1.insert(u);
//        unordered_set<int> path2 = graph[v];
//        path2.insert(v);
//
//        // �ж�·���Ƿ���ͬ
//        if (isSamePath(path1, path2)) {
//            continue; // ·����ͬ�������д���
//        }
//
//        graph[u].insert(v);
//        inDegree[v]++;
//        Degree[v]++;
//    }
//
//    unordered_map<int, int> dp; // �洢��ͬ������Ϊ�յ�����мƻ���
//
//    queue<int> q;
//    for (int i = 1; i <= n; ++i) {
//        if (inDegree[i] == 0) { // �ҵ�������
//            q.push(i);
//            dp[i] = 1; // �����е����мƻ���Ϊ1
//        }
//    }
//
//    while (!q.empty()) {
//        int city = q.front();
//        q.pop();
//        for (int nextCity : graph[city]) {
//            dp[nextCity] += dp[city]; // ��̬�滮ת�Ʒ���
//            inDegree[nextCity]--;
//            if (inDegree[nextCity] == 0) {
//                q.push(nextCity);
//            }
//        }
//    }
//
//    int totalTravelPlan = 0;
//    for (int i = 1; i <= n; ++i) {
//        if (graph[i].empty()) { // �յ����
//            if(Degree[i]!=0)
//            totalTravelPlan += dp[i];
//        }
//    }
//
//    cout << totalTravelPlan << endl;
//
//    return 0;
//}
