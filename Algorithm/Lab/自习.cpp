//#include <bits/stdc++.h>
//using namespace std;
//long long n, m, l, r, ans, num, mid;
//struct study {
//	long long teach, self;
//}a[300010];
//long long lesson_num(long long target, long long eta) {//eta 代表工作效率
//	//工作时间=工作总量/工作效率，向上取整
//	if (target % eta == 0)return target / eta;
//	else return target / eta + 1;
//}
//bool check(long long target) {//target 代表目标 
//	num = 0;//num代表达到目标所需的上课次数 
//	for (int i = 1; i <= n; i++) {//每一门课都要到达大于等于目标的理解程度 
//		if (a[i].self > a[i].teach)//自学比上课好，就绝对去自学 
//			num += lesson_num(target, a[i].self);
//		else {//上课理解程度高，优先选择上课 
//			if (m * a[i].teach >= target)//仅靠上课，就可以达到目标：）
//				num += lesson_num(target, a[i].teach);
//			else {//不仅要上课，还要去自学才能达成目标qwq 
//				num += m;//上课的次数先计算上； 
//				num += lesson_num(target - m * a[i].teach, a[i].self);
//				//剩下的全部自学。 
//			}
//		}
//		//一轮下来，我们就得出所需的上课次数了！
//		//如果此时总上课次数已经超标，就不必计算直接返回假值。 
//		if (num > n * m)return 0;
//	}
//	return 1;
//
//}
//int main() {
//	scanf("%lld%lld", &n, &m);
//	for (int i = 1; i <= n; i++)scanf("%lld", &a[i].teach);
//	for (int i = 1; i <= n; i++)scanf("%lld", &a[i].self);
//	l = 1; r = 1000000000000000010;
//	while (l <= r) {//二分答案加验证 
//		mid = (l + r) >> 1;
//		if (check(mid) == true) {
//			ans = mid;
//			l = mid + 1;
//		}
//		else r = mid - 1;
//	}
//	printf("%lld\n", ans);
//	return 0;
//}