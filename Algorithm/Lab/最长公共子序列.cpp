//#include <iostream>
//#include <vector>
//using namespace std;
//
//int main() {
//	long long * a,* b ; int n, m;
//	cin >> n >> m;
//	ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//	a = new long long[n]; b = new long long[m]; 
//	
//	for (int i = 0; i < n; i++) {
//		cin >> a[i];
//	}
//	for (int i = 0; i < m; i++) {
//		cin >> b[i];
//	}
//	vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
//		for (int i = 0; i < n; i++) {
//			for (int j = 0; j < m; j++) {
//				if (a[i] == b[j]) {
//					dp[i + 1][j + 1] = dp[i][j] + 1;
//				}
//				else dp[i + 1][j + 1] = max(dp[i][j + 1],dp[i + 1][j] );
//			}
//		}
//		cout << dp[n][m] << endl;
//}