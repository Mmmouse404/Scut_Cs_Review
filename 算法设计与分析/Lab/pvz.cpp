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

    //�ж���i��jΪ���ĵ�3*3�����Ƿ��н�ʬ
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
    //ը����i��jΪ���ĵ�3*3����Ľ�ʬ
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
        // ��ʼ����������
        vector<vector<int>> costGrid(4, vector<int>(7, 150));
        for (int i = 1; i <= 2; i++) {
            for (int j = 0; j <= 6; j++) {
                costGrid[i][j] = 175;
            }
        }
        //����grid�򻯷�������
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
        costGrid[i][j]��λ�ö�Ӧ����grid[i-1][j-1]��λ��
        */
        int ans = dfs(grid, costGrid, minCost, 0, 1, 1);
        return ans;
    }
    bool canReachEndOrBetter(const vector<vector<int>>& grid, int gi, int gj, int minCost, int currentCost)
    {
        //���currentCost�Ѿ�����minCost��ֱ�ӷ���
        if (currentCost >= minCost)
        {
            return false;
        }
        //�ж��Ƿ���Ե����յ�
        //���gi>=2����ô���ǰgi-2���Ƿ��н�ʬ
        //���gj>=2��gi>=1����ô���ǰgi-1�е�gj-2���Ƿ��н�ʬ
        //���gi==4����ô�ڼ���3����5�еĵ�0��gj-2���Ƿ��н�ʬ
        //�������������һ�����㣬��ô˵�����ܵ����յ�
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

    //�ж��Ƿ񵽴��յ�
    //���max(0,gi-1)����5���ж�û�н�ʬ����ô�����յ�
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
        //�ж��Ƿ���Ե����յ�����Ƿ���ܸ���
        if (not canReachEndOrBetter(grid, gi, gj, minCost, currentCost))
        {
            return minCost;
        }

        //�ж��Ƿ񵽴��յ�
        if (isEnd(grid, gi))
        {
            minCost = min(minCost, currentCost);
            return minCost;
        }

        //���û�е����յ㣬��ô�����ݹ�
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
                    vector<vector<int>> gridCopy = grid;//������ڻ���

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


                    //����
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