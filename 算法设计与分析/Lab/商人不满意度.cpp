#include <iostream>
#include <cmath>
using namespace std;
typedef long long ll;
//���㲻�����
int cal(long long x) {
    int tmp = 0;
    while (x % 10 == 0) x /= 10;
    if (x % 10 == 5) tmp--;
    while (x) {
        x /= 10;
        tmp += 2;
    }
    return tmp;
}

//int main() {
//    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//    int t;cin >> t;
//    while (t--) {
//        ll l, r;
//        cin >> l >> r;
//        ll ans1 = l, ans2 = cal(l);
//       //0������5������������ŵģ������ʮλ��ʼö��
//        for (int i = 1; i <= r; i *= 10) {
//            //ö��Ϊ0�����
//            ll tmp;
//            tmp = l / (i * 10) * (i * 10);
//            while (tmp < l) tmp += i * 10;
//            if (tmp <= r && cal(tmp) < ans2) {
//                ans1 = tmp;
//                ans2 = cal(tmp);
//            }
//            //ö��Ϊ5�����
//            tmp = l / (i * 10) * (i * 10) + 5 * i;
//            while (tmp < l) tmp += i * 10;
//            if (tmp <= r && cal(tmp) < ans2) {
//                ans1 = tmp;
//                ans2 = cal(tmp);
//            }
//        }
//        cout << ans1 << endl;
//    }
//    return 0;
//}
