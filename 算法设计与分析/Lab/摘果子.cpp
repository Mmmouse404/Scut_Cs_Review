//#include<iostream >
//#include<algorithm>
//#include<cstring>
//using namespace std;
//int n, h;
//int a[1000], b[1000], c[1000];
//int a1[1000]; //��a1[i]�洢ԭʼ����
//int main()
//{
//	int k, m, sum = 0, max = 0;
//	int i, j, h1;
//	cin >> n >> h;
//	h *= 12;
//	h1 = h;
//	for (i = 1; i <= n; i++) {
//		cin >> a[i]; a1[i] = a[i];
//	}
//
//	for (i = 1; i <= n; i++) {
//		cin >> b[i];
//	}
//
//	c[1] = 0;
//	for (i = 2; i <= n; i++) {
//		cin >> k;
//		c[i] = c[i - 1] + k;
//	}
//
//	for (i = 1; i <= n; i++) { //�Ѿ������ֻ������i����԰ 
//		h = h - c[i];
//		sum = 0;
//		int Max = 1;
//		while (h > 0) {
//			for (j = 1; j <= i; j++) {
//				if (a[Max] < a[j])
//					Max = j;
//			}
//			if (a[Max] == 0)
//				break;
//			sum += a[Max];
//			a[Max] -= b[Max];
//			if (a[Max] < 0) {
//				a[Max] = 0;
//			}
//			h--;
//
//		}
//		if (max < sum)
//			max = sum;
//		h = h1;  //��ԭh 
//		for (j = 1; j <= n; j++) { //��ԭa���� 
//			a[j] = a1[j];
//		}
//	}
//	cout << max;
//	return 0;
//}
