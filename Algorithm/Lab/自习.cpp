//#include <bits/stdc++.h>
//using namespace std;
//long long n, m, l, r, ans, num, mid;
//struct study {
//	long long teach, self;
//}a[300010];
//long long lesson_num(long long target, long long eta) {//eta ������Ч��
//	//����ʱ��=��������/����Ч�ʣ�����ȡ��
//	if (target % eta == 0)return target / eta;
//	else return target / eta + 1;
//}
//bool check(long long target) {//target ����Ŀ�� 
//	num = 0;//num����ﵽĿ��������Ͽδ��� 
//	for (int i = 1; i <= n; i++) {//ÿһ�ſζ�Ҫ������ڵ���Ŀ������̶� 
//		if (a[i].self > a[i].teach)//��ѧ���Ͽκã��;���ȥ��ѧ 
//			num += lesson_num(target, a[i].self);
//		else {//�Ͽ����̶ȸߣ�����ѡ���Ͽ� 
//			if (m * a[i].teach >= target)//�����ϿΣ��Ϳ��ԴﵽĿ�꣺��
//				num += lesson_num(target, a[i].teach);
//			else {//����Ҫ�ϿΣ���Ҫȥ��ѧ���ܴ��Ŀ��qwq 
//				num += m;//�ϿεĴ����ȼ����ϣ� 
//				num += lesson_num(target - m * a[i].teach, a[i].self);
//				//ʣ�µ�ȫ����ѧ�� 
//			}
//		}
//		//һ�����������Ǿ͵ó�������Ͽδ����ˣ�
//		//�����ʱ���Ͽδ����Ѿ����꣬�Ͳ��ؼ���ֱ�ӷ��ؼ�ֵ�� 
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
//	while (l <= r) {//���ִ𰸼���֤ 
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