//#include<iostream>
//
//using namespace std;
//int n, e[250], s[250][250], maxans = 0;
//int main() {
//    ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
//    cin >> n; 
//    for (int i = 1; i <= n; i++) { cin >> e[i]; e[i + n] = e[i]; }
//    //�ظ��洢һ�λ����������ӿ�ʼ��β��
//    for (int i = 2; i < 2 * n; i++) {
//        for (int j = i - 1; i - j < n && j >= 1; j--) {//dp�Ĺ��̣���i��ʼ��ǰ��,��Ϊ������
//
//            //   |    A   |   B     |    C 
//            //       j(k)     i
//            //  e[j]    e[k+1]    e[i+1]
//            for (int k = j; k < i; k++)//k�Ǵ�������������Ļ��ֵ�
//                s[j][i] = max(s[j][i], s[j][k] + s[k + 1][i] + e[j] * e[k + 1] * e[i + 1]);
//            //״̬ת�Ʒ��̣�max(ԭ������������������+����������+�ϲ�������������  
//            if (s[j][i] > maxans)maxans = s[j][i];//�����ֵ 
//        }
//    }
//    cout << maxans;
//    return 0;
//}