//#include <iostream>
//#include <vector>
//#include <algorithm>
//using namespace std;
//int minSubArraySum(std::vector<int>& nums, int k) {
//    int left = 0, right = 0;
//    int min_sum = 1000000;
//    int current_sum = 0;
//
//    while (right < nums.size()) {
//        current_sum += nums[right];
//        while (current_sum >= k) {
//            min_sum = std::min(min_sum, current_sum);
//            current_sum -= nums[left];
//            left++;
//        }
//        right++;
//    }
//
//    return min_sum != 1000000 ? min_sum : 0;
//}
//int arraySum(vector<int>nums) {
//    int sum = 0;
//    for (int i = 0; i < nums.size(); i++) {
//        sum += nums[i];
//    }
//    return sum;
//}
//int main() {
//    int n, m;
//    std::cin >> n >> m;
//    std::vector<int> cards(n);
//    for (int i = 0; i < n; ++i) {
//        std::string card;
//        std::cin >> card;
//        if (card == "A") {
//            cards[i] = 1;
//        }
//        else if (card == "J") {
//            cards[i] = 11;
//        }
//        else if (card == "Q") {
//            cards[i] = 12;
//        }
//        else if (card == "K") {
//            cards[i] = 13;
//        }
//        else {
//            cards[i] = atoi(card.c_str());;
//        }
//    }
//
//    sort(cards.begin(), cards.end());
//    int sum = arraySum(cards);
//    int ans = minSubArraySum(cards, m);
//    if (sum>=m)
//        cout << sum - ans << endl;
//    if(sum<m) cout << -1 << endl;
//        return 0;
//    
//
//}
