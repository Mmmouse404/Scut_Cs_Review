#include<iostream>
#include<algorithm>
using namespace std;
typedef long long ll;
ll pre[200005];
struct edge
{
	ll a, b, v;
	bool operator<(edge y)
	{
		return v < y.v;
	}
}e[500005];
ll findx(ll x)
{
	if (x == pre[x])
		return x;
	return pre[x] = findx(pre[x]);
}
//int main()
//{
//	ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//	ll n, m, i, sum = 0;
//	cin >> n >> m;
//	for (i = 1; i <= n; i++)
//		pre[i] = i;//并查集初始化 
//	for (i = 1; i <= m; i++)
//		cin >> e[i].a >> e[i].b >> e[i].v;
//	sort(e + 1, e + 1 + m);
//	for (i = 1; i <= m; i++)
//	{
//		ll x = findx(e[i].a);
//		ll y = findx(e[i].b);
//		if (x != y)
//		{
//			pre[x] = y;
//			sum += e[i].v;
//		}
//	}
//	cout << sum;
//	return 0;
//}
