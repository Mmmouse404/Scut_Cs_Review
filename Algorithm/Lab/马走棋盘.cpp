//#include <iostream>
//#include <vector>
//#include <queue>
//#include <tuple>
//using namespace std;
//
//int main() {
//    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//    int n;
//    cin >> n;
//
//    vector<int> dx = { 1, 2, 2, 1, -1, -2, -2, -1 };
//    vector<int> dy = { 2, 1, -1, -2, -2, -1, 1, 2 };
//
//    for (int i = 0; i < n; i++) {
//        int L, x1, y1, x2, y2;
//        cin >> L >> x1 >> y1 >> x2 >> y2;
//
//        if (x1 == x2 && y1 == y2) {
//            cout << "0" << endl;
//            continue;
//        }
//
//        vector<vector<bool>> visited(L, vector<bool>(L, false));
//        queue<tuple<int, int, int>> q; // (x, y, steps)
//        q.push(make_tuple(x1, y1, 0));//马的位置入队
//        visited[x1][y1] = true;
//
//        while (!q.empty()) {
//            int x, y, steps;
//            tie(x, y, steps) = q.front();
//            q.pop();
//
//            for (int k = 0; k < 8; k++) {
//                int new_x = x + dx[k];
//                int new_y = y + dy[k];
//                if (new_x >= 0 && new_x < L && new_y >= 0 && new_y < L && !visited[new_x][new_y]) {
//                    if (new_x == x2 && new_y == y2) {
//                        cout << steps + 1 << endl;
//                        while (!q.empty()) q.pop(); // 清空队列
//                        break;
//                    }
//                    q.push(make_tuple(new_x, new_y, steps + 1));
//                    visited[new_x][new_y] = true;//本质BFS
//                }
//            }
//        }
//    }
//
//    return 0;
//}
