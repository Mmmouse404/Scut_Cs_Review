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
//    // 定义一个数组 diff 存储每门课选择上课和自学的熟练度差值
//    vector<long long> diff(N);
//    for (long long i = 0; i < N; i++) {
//        A[i] = read();
//        total += A[i];
//    }
//    for (long long i = 0; i < N; i++) {
//        B[i] = read(); diff[i] =  B[i]-A[i];
//    }
//    // 对 diff 数组进行排序，按照差值从大到小排序
//    std::nth_element(diff.begin(), diff.begin() + k, diff.end(), greater<long long>());
//
//    
//    // 对于前 k 个差值，选择上课，其余选择自学
//    for (long long i = 0; i < k; i++) {
//        total += diff[i];
//    }
//
//    cout << total ;
//
//    return 0;
//}
