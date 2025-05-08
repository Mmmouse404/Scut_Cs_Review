#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int maxJump(int N, vector<int>&nums) {
			int maxReach = 0;
			for (int i = 0; i < N; i++) {
				if (i <= maxReach) {
					//更新当前能跳的最长距离
					maxReach = max(maxReach, i + nums[i]);
					//如果当前能跳的最长距离大于数组的最后一个下标，则返回true
					if (maxReach >= N - 1) {
						return N-1;
					}
				}
			}
			return maxReach;
		}
//int main() {
//	ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//	vector<int> num; int n;
//	cin >> n;
//		num.resize(n);
//		for (int i = 0; i < n; i++) {
//			cin >> num[i];
//		}
//		int ans = maxJump(n, num);
//		cout << ans << endl;
//		num.clear();
//	
//}