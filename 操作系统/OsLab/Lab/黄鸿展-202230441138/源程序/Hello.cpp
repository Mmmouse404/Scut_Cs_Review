#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

const int N = 10; // 页面数量
const int M = 3;  // 物理块数量

// 页表项结构体
struct PageTableEntry {
    int pageNumber;   // 页号
    int frameNumber;  // 物理块号
    bool valid;       // 有效位（是否在内存中）
    bool referenced;  // 引用位（是否被访问过）
    bool modified;    // 修改位（是否被修改过）
};

vector<pair<int, char>> pageAccessSequence; // 页面访问序列（页号, 访问方式）
vector<PageTableEntry> pageTable(N);        // 页表
vector<int> frames(M, -1);                  // 物理块，初始为空

// 初始化页表
void initializePageTable() {
    for (int i = 0; i < N; i++) {
        pageTable[i].pageNumber = i;
        pageTable[i].frameNumber = -1;
        pageTable[i].valid = false;
        pageTable[i].referenced = false;
        pageTable[i].modified = false;
    }
}

// 生成页面访问序列
void generatePageAccessSequence() {
    srand(time(0)); // 设置随机种子
    for (int i = 0; i < 20; i++) {
        int page = rand() % N; // 随机生成页面号
        char accessType = (rand() % 2 == 0) ? 'R' : 'W'; // 随机生成访问方式（读/写）
        pageAccessSequence.emplace_back(page, accessType);
    }
}

// 打印页表
void printPageTable() {
    cout << "页表内容：" << endl;
    cout << "页号\t物理块号\t有效位\t引用位\t修改位" << endl;
    for (int i = 0; i < N; i++) {
        cout << pageTable[i].pageNumber << "\t\t" << pageTable[i].frameNumber << "\t\t"
             << pageTable[i].valid << "\t" << pageTable[i].referenced << "\t\t"
             << pageTable[i].modified << endl;
    }
}

// 打印物理块状态
void printFrames() {
    cout << "物理块状态: ";
    for (int i = 0; i < M; i++) {
        if (frames[i] != -1) {
            cout << frames[i] << " ";
        } else {
            cout << "- ";
        }
    }
    cout << endl;
}

// 查找页面是否在物理块中
int findPageInFrames(int page) {
    for (int i = 0; i < M; i++) {
        if (frames[i] == page) {
            return i; // 返回物理块索引
        }
    }
    return -1; // 未找到
}

// OPT算法（最佳替换算法）
int optAlgorithm() {
    cout << "OPT算法：" << endl;
    int pageFaults = 0; // 缺页次数
    for (int i = 0; i < pageAccessSequence.size(); i++) {
        int page = pageAccessSequence[i].first; // 当前访问的页面
        char accessType = pageAccessSequence[i].second; // 访问方式
        cout << "访问页面 " << page << " (" << accessType << ")" << endl;
        printPageTable();
        printFrames();
        if (findPageInFrames(page) != -1) {
            cout << "页面 " << page << " 已在内存中。" << endl;
            pageTable[page].referenced = true; // 更新引用位
            if (accessType == 'W') {
                pageTable[page].modified = true; // 如果是写操作，更新修改位
            }
        } else {
            pageFaults++;
            if (frames[M - 1] == -1) { // 如果还有空闲物理块
                for (int j = 0; j < M; j++) {
                    if (frames[j] == -1) {
                        frames[j] = page;
                        pageTable[page].frameNumber = j;
                        pageTable[page].valid = true;
                        pageTable[page].referenced = true;
                        if (accessType == 'W') {
                            pageTable[page].modified = true;
                        }
                        break;
                    }
                }
            } else { // 没有空闲物理块，需要替换
                vector<int> futureAccess(M, pageAccessSequence.size()); // 记录每个页面未来被访问的时间
                for (int j = 0; j < M; j++) {
                    for (int k = i + 1; k < pageAccessSequence.size(); k++) {
                        if (pageAccessSequence[k].first == frames[j]) {
                            futureAccess[j] = k;
                            break;
                        }
                    }
                }
                int replaceIndex = 0;
                for (int j = 1; j < M; j++) {
                    if (futureAccess[j] > futureAccess[replaceIndex]) {
                        replaceIndex = j;
                    }
                }
                int replacedPage = frames[replaceIndex];
                frames[replaceIndex] = page;
                pageTable[replacedPage].frameNumber = -1;
                pageTable[replacedPage].valid = false;
                pageTable[page].frameNumber = replaceIndex;
                pageTable[page].valid = true;
                pageTable[page].referenced = true;
                if (accessType == 'W') {
                    pageTable[page].modified = true;
                }
            }
            cout << "缺页：页面 " << page << " 被加载到物理块 " << pageTable[page].frameNumber << endl;
        }
    }
    cout << "总缺页次数: " << pageFaults << endl;
    return pageFaults;
}

// FIFO算法（先进先出算法）
int fifoAlgorithm() {
    cout << "FIFO算法：" << endl;
    int pageFaults = 0;
    queue<int> fifoQueue; // FIFO队列
    for (int i = 0; i < pageAccessSequence.size(); i++) {
        int page = pageAccessSequence[i].first;
        char accessType = pageAccessSequence[i].second;
        cout << "访问页面 " << page << " (" << accessType << ")" << endl;
        printPageTable();
        printFrames();

        if (findPageInFrames(page) != -1) {
            cout << "页面 " << page << " 已在内存中。" << endl;
            pageTable[page].referenced = true;
            if (accessType == 'W') {
                pageTable[page].modified = true;
            }
        } else {
            pageFaults++;
            if (frames[M - 1] == -1) { // 如果还有空闲物理块
                for (int j = 0; j < M; j++) {
                    if (frames[j] == -1) {
                        frames[j] = page;
                        pageTable[page].frameNumber = j;
                        pageTable[page].valid = true;
                        pageTable[page].referenced = true;
                        if (accessType == 'W') {
                            pageTable[page].modified = true;
                        }
                        fifoQueue.push(j); // 将物理块索引加入FIFO队列
                        break;
                    }
                }
            } else { // 没有空闲物理块，需要替换
                if (fifoQueue.empty()) {
                    cerr << "错误：FIFO队列为空，无法进行页面替换。" << endl;
                    exit(1);
                }
                int replaceIndex = fifoQueue.front();
                fifoQueue.pop();
                int replacedPage = frames[replaceIndex];
                frames[replaceIndex] = page;
                pageTable[replacedPage].frameNumber = -1;
                pageTable[replacedPage].valid = false;
                pageTable[page].frameNumber = replaceIndex;
                pageTable[page].valid = true;
                pageTable[page].referenced = true;
                if (accessType == 'W') {
                    pageTable[page].modified = true;
                }
                fifoQueue.push(replaceIndex); // 将替换后的物理块索引重新加入FIFO队列
            }
            cout << "缺页：页面 " << page << " 被加载到物理块 " << pageTable[page].frameNumber << endl;
        }
    }
    cout << "总缺页次数: " << pageFaults << endl;
    return pageFaults;
}

// LRU算法（最近最少使用算法）
int lruAlgorithm() {
    cout << "LRU算法：" << endl;
    int pageFaults = 0;
    vector<int> lruList; // LRU列表
    for (int i = 0; i < pageAccessSequence.size(); i++) {
        int page = pageAccessSequence[i].first;
        char accessType = pageAccessSequence[i].second;
        cout << "访问页面 " << page << " (" << accessType << ")" << endl;
        printPageTable();
        printFrames();

        if (findPageInFrames(page) != -1) {
            cout << "页面 " << page << " 已在内存中。" << endl;
            pageTable[page].referenced = true;
            if (accessType == 'W') {
                pageTable[page].modified = true;
            }
            // 更新LRU列表，将访问的页面移到最前面
            lruList.erase(remove(lruList.begin(), lruList.end(), page), lruList.end());
            lruList.insert(lruList.begin(), page);
        } else {
            pageFaults++;
            if (frames[M - 1] == -1) { // 如果还有空闲物理块
                for (int j = 0; j < M; j++) {
                    if (frames[j] == -1) {
                        frames[j] = page;
                        pageTable[page].frameNumber = j;
                        pageTable[page].valid = true;
                        pageTable[page].referenced = true;
                        if (accessType == 'W') {
                            pageTable[page].modified = true;
                        }
                        lruList.insert(lruList.begin(), page); // 将新页面插入LRU列表
                        break;
                    }
                }
            } else { // 没有空闲物理块，需要替换
                int replacePage = lruList.back(); // 选择LRU列表中最后一个页面（最近最少使用的页面）
                lruList.pop_back(); // 从LRU列表中移除
                int replaceIndex = pageTable[replacePage].frameNumber;
                frames[replaceIndex] = page;
                pageTable[replacePage].frameNumber = -1;
                pageTable[replacePage].valid = false;
                pageTable[page].frameNumber = replaceIndex;
                pageTable[page].valid = true;
                pageTable[page].referenced = true;
                if (accessType == 'W') {
                    pageTable[page].modified = true;
                }
                lruList.insert(lruList.begin(), page); // 将新页面插入LRU列表
            }
            cout << "缺页：页面 " << page << " 被加载到物理块 " << pageTable[page].frameNumber << endl;
        }
    }
    cout << "总缺页次数: " << pageFaults << endl;
    return pageFaults;
}

// Clock算法（简单时钟算法）
int clockAlgorithm() {
    cout << "Clock算法：" << endl;
    int pageFaults = 0;
    int clockHand = 0; // 时钟指针
    for (int i = 0; i < pageAccessSequence.size(); i++) {
        int page = pageAccessSequence[i].first;
        char accessType = pageAccessSequence[i].second;
        cout << "访问页面 " << page << " (" << accessType << ")" << endl;
        printPageTable();
        printFrames();

        if (findPageInFrames(page) != -1) {
            cout << "页面 " << page << " 已在内存中。" << endl;
            pageTable[page].referenced = true; // 更新引用位
            if (accessType == 'W') {
                pageTable[page].modified = true; // 如果是写操作，更新修改位
            }
        } else {
            pageFaults++;
            while (true) {
                if (!pageTable[frames[clockHand]].referenced) { // 找到未被引用的页面
                    int replacedPage = frames[clockHand];
                    frames[clockHand] = page;
                    pageTable[replacedPage].frameNumber = -1;
                    pageTable[replacedPage].valid = false;
                    pageTable[page].frameNumber = clockHand;
                    pageTable[page].valid = true;
                    pageTable[page].referenced = true;
                    if (accessType == 'W') {
                        pageTable[page].modified = true;
                    }
                    clockHand = (clockHand + 1) % M; // 更新时钟指针
                    break;
                } else {
                    pageTable[frames[clockHand]].referenced = false; // 清除引用位
                    clockHand = (clockHand + 1) % M;
                }
            }
            cout << "缺页：页面 " << page << " 被加载到物理块 " << pageTable[page].frameNumber << endl;
        }
    }
    cout << "总缺页次数: " << pageFaults << endl;
    return pageFaults;
}

// 改进Clock算法
int improvedClockAlgorithm() {
    cout << "改进Clock算法：" << endl;
    int pageFaults = 0;
    int clockHand = 0;
    for (int i = 0; i < pageAccessSequence.size(); i++) {
        int page = pageAccessSequence[i].first;
        char accessType = pageAccessSequence[i].second;
        cout << "访问页面 " << page << " (" << accessType << ")" << endl;
        printPageTable();
        printFrames();

        if (findPageInFrames(page) != -1) {
            cout << "页面 " << page << " 已在内存中。" << endl;
            pageTable[page].referenced = true; // 更新引用位
            if (accessType == 'W') {
                pageTable[page].modified = true; // 如果是写操作，更新修改位
            }
        } else {
            pageFaults++;
            bool replaced = false;
            while (!replaced) {
                if (!pageTable[frames[clockHand]].referenced && !pageTable[frames[clockHand]].modified) { // 找到未引用且未修改的页面
                    int replacedPage = frames[clockHand];
                    frames[clockHand] = page;
                    pageTable[replacedPage].frameNumber = -1;
                    pageTable[replacedPage].valid = false;
                    pageTable[page].frameNumber = clockHand;
                    pageTable[page].valid = true;
                    pageTable[page].referenced = true;
                    if (accessType == 'W') {
                        pageTable[page].modified = true;
                    }
                    clockHand = (clockHand + 1) % M;
                    replaced = true;
                } else if (!pageTable[frames[clockHand]].referenced && pageTable[frames[clockHand]].modified) { // 找到未引用但已修改的页面
                    pageTable[frames[clockHand]].modified = false; // 清除修改位
                    clockHand = (clockHand + 1) % M;
                } else if (pageTable[frames[clockHand]].referenced) { // 找到已引用的页面
                    pageTable[frames[clockHand]].referenced = false; // 清除引用位
                    clockHand = (clockHand + 1) % M;
                }
            }
            cout << "缺页：页面 " << page << " 被加载到物理块 " << pageTable[page].frameNumber << endl;
        }
    }
    cout << "总缺页次数: " << pageFaults << endl;
    return pageFaults;
}

int main() {
    initializePageTable(); // 初始化页表
    generatePageAccessSequence(); // 生成页面访问序列
    int fifoFaults = fifoAlgorithm(); // 运行FIFO算法
    cout << endl;
    initializePageTable();
    int lruFaults = lruAlgorithm(); // 运行LRU算法
    cout << endl;

    initializePageTable();
    int optFaults = optAlgorithm(); // 运行OPT算法
    cout << endl;



    initializePageTable();
    int clockFaults = clockAlgorithm(); // 运行Clock算法
    cout << endl;

    initializePageTable();
    int improvedClockFaults = improvedClockAlgorithm(); // 运行改进Clock算法

    // 比较各算法的缺页率
    cout << "各算法的缺页率比较：" << endl;
    cout << "FIFO算法缺页率: " << (double)fifoFaults / pageAccessSequence.size() <<"缺页次数："<< fifoFaults << endl;
    cout << "LRU算法缺页率: " << (double)lruFaults / pageAccessSequence.size() <<"缺页次数"<< lruFaults << endl;
    cout << "OPT算法缺页率: " << (double)optFaults / pageAccessSequence.size() <<"缺页次数"<< optFaults << endl;
    cout << "Clock算法缺页率: " << (double)clockFaults / pageAccessSequence.size() <<"缺页次数"<<clockFaults<< endl;
    cout << "改进Clock算法缺页率: " << (double)improvedClockFaults / pageAccessSequence.size() <<"缺页次数"<< improvedClockFaults << endl;

    return 0;
}