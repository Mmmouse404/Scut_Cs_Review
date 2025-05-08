#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
#include <cstring>

using namespace std;

class Solution
{
public:
    int dx[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
    int dy[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

    //判断以i，j为中心的3*3网格是否有僵尸
    bool isExistZombie(const vector<vector<int>>& grid, int i, int j)
    {
        for (int k = 0; k <= 8; k++)
        {
            if (grid[i + dx[k]][j + dy[k]] == 1)
            {
                return true;
            }
        }
        return false;
    }
    //炸掉以i，j为中心的3*3网格的僵尸
    void bombZombie(vector<vector<int>>& grid, int i, int j)
    {
        for (int k = 0; k <= 8; k++)
        {
            grid[i + dx[k]][j + dy[k]] = 0;
        }
    }
    int minSunCost(vector<vector<int>> grid)
    {
        int minCost = INT_MAX;
        // 初始化费用网格
        vector<vector<int>> costGrid(4, vector<int>(7, 150));
        for (int i = 1; i <= 2; i++) {
            for (int j = 0; j <= 6; j++) {
                costGrid[i][j] = 175;
            }
        }
        //遍历grid简化费用网格
        for (int i = 1; i <= 4; i++)
        {
            for (int j = 1; j <= 7; j++)
            {
                if (not isExistZombie(grid, i, j))
                {
                    costGrid[i - 1][j - 1] = 0;
                }
            }
        }
        /*
        costGrid[i][j]的位置对应的是grid[i-1][j-1]的位置
        */
        int ans = dfs(grid, costGrid, minCost, 0, 1, 1);
        return ans;
    }
    bool canReachEndOrBetter(const vector<vector<int>>& grid, int gi, int gj, int minCost, int currentCost)
    {
        //如果currentCost已经大于minCost，直接返回
        if (currentCost >= minCost)
        {
            return false;
        }
        //判断是否可以到达终点
        //如果gi>=2，那么检查前gi-2行是否有僵尸
        //如果gj>=2且gi>=1，那么检查前gi-1行的gj-2列是否有僵尸
        //如果gi==4，那么第检查第3到第5行的第0到gj-2列是否有僵尸
        //如果上述条件有一个满足，那么说明不能到达终点
        for (int i = 0; i <= gi - 2; i++)
        {
            for (int j = 0; j <= 8; j++)
            {
                if (grid[i][j] == 1)
                {
                    return false;
                }
            }
        }
        for (int i = 0; i <= gi - 1; i++)
        {
            for (int j = 0; j <= gj - 2; j++)
            {
                if (grid[i][j] == 1)
                {
                    return false;
                }
            }
        }
        if (gi == 4 and gj >= 2)
        {
            for (int i = 3; i <= 5; i++)
            {
                for (int j = 0; j <= gj - 2; j++)
                {
                    if (grid[i][j] == 1)
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    //判断是否到达终点
    //如果max(0,gi-1)到第5行中都没有僵尸，那么到达终点
    bool isEnd(const vector<vector<int>>& grid, int gi)
    {
        for (int i = max(0, gi - 1); i <= 5; i++)
        {
            for (int j = 0; j <= 8; j++)
            {
                if (grid[i][j] == 1)
                {
                    return false;
                }
            }
        }
        return true;
    }
    int dfs(vector<vector<int>>grid, const vector<vector<int>>& costGrid, int& minCost, int currentCost, int gi, int gj)
    {
        //判断是否可以到达终点或者是否可能更优
        if (not canReachEndOrBetter(grid, gi, gj, minCost, currentCost))
        {
            return minCost;
        }

        //判断是否到达终点
        if (isEnd(grid, gi))
        {
            minCost = min(minCost, currentCost);
            return minCost;
        }

        //如果没有到达终点，那么继续递归
        for (int i = gi; i <= 4; i++)
        {
            for (int j = (i == gi ? gj : 1); j <= 7; j++)
            {
                if (not canReachEndOrBetter(grid, i, j, minCost, currentCost))
                {
                    return minCost;
                }

                else if (isExistZombie(grid, i, j))
                {
                    vector<vector<int>> gridCopy = grid;//深拷贝便于回溯

                    currentCost += costGrid[i - 1][j - 1];

                    bombZombie(grid, i, j);

                    if (i == 4 and j == 7)
                    {
                        minCost = min(minCost, currentCost);
                        return minCost;
                    }
                    else if (i <= 3 and j == 7)
                    {
                        dfs(grid, costGrid, minCost, currentCost, i + 1, 1);
                    }
                    else
                    {
                        dfs(grid, costGrid, minCost, currentCost, i, min(j + 3, 7));
                    }


                    //回溯
                    grid = gridCopy;
                    currentCost -= costGrid[i - 1][j - 1];
                    if (not canReachEndOrBetter(grid, i, j, minCost, currentCost))
                    {
                        return minCost;
                    }
                }
                else if (isEnd(grid, i))
                {
                    minCost = min(minCost, currentCost);
                    return minCost;
                }
            }
        }

    }
};




int main() {
    vector<vector<int>> grid(6, vector<int>(9));
    for (int i = 0; i <= 5; i++) {
        for (int j = 0; j <= 8; j++) {
            cin >> grid[i][j];
        }
    }
    Solution s;
    cout << s.minSunCost(grid) << endl;
    return 0;
}