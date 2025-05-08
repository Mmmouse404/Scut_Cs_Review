#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

long long read() {
    long long x = 0;
    char c = getchar();
    while (c < '0' || c > '9') {
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    return x;
}

//int main() {
//    long long N = read();
//    long long k = read();
//    long long total = 0;
//    vector<long long> A(N), B(N);
//    // ����һ������ diff �洢ÿ�ſ�ѡ���Ͽκ���ѧ�������Ȳ�ֵ
//    vector<long long> diff(N);
//    for (long long i = 0; i < N; i++) {
//        A[i] = read();
//        total += A[i];
//    }
//    for (long long i = 0; i < N; i++) {
//        B[i] = read(); diff[i] =  B[i]-A[i];
//    }
//    // �� diff ����������򣬰��ղ�ֵ�Ӵ�С����
//    std::nth_element(diff.begin(), diff.begin() + k, diff.end(), greater<long long>());
//
//    
//    // ����ǰ k ����ֵ��ѡ���ϿΣ�����ѡ����ѧ
//    for (long long i = 0; i < k; i++) {
//        total += diff[i];
//    }
//
//    cout << total ;
//
//    return 0;
//}
