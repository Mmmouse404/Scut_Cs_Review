import copy

class PuzzleGrid:
    def __init__(self, state):
        self.previous = None  # 用于追踪父节点
        self.target_state = [[1, 2, 3], [8, 0, 4], [7, 6, 5]]
        self.state = state
        self.find_blank()
        self.calculate_costs()  # 计算当前状态的G、H、F值

    def calculate_costs(self):
        self.calculate_h()  # 计算H值（启发值）
        self.calculate_g()  # 计算G值（从起始状态到当前状态的步数）
        self.calculate_f()  # 计算F值（总成本）

    def calculate_g(self):
        if self.previous is not None:  # 如果有父节点，步数加一
            self.g = self.previous.g + 1
        else:
            self.g = 0  # 否则步数为0

    def calculate_h(self):
        self.h = 0  # H值初始化
        for i in range(3):
            for j in range(3):
                target_value = self.target_state[i][j]
                current_position = self.find_position(target_value)
                self.h += abs(current_position[0] - i) + abs(current_position[1] - j)  # 计算曼哈顿距离总和

    def calculate_f(self):
        self.f = self.g + self.h  # F值为G值和H值之和

    def display(self):
        for row in self.state:
            print(row)
        print(f"F={self.f}, G={self.g}, H={self.h}")
        print("-" * 10)

    def display_solution_path(self):
        path = []
        path.append(self)  # 添加当前状态到路径中
        node = self.previous  # 从父节点开始
        while node:
            path.append(node)  # 逐步向上添加父节点
            node = node.previous
        path.reverse()  # 反转路径以从起始到目标显示
        for state in path:
            state.display()  # 显示每个状态

    def find_position(self, value):
        for i in range(3):
            if value in self.state[i]:
                j = self.state[i].index(value)
                return [i, j]  # 返回行列坐标

    def find_blank(self):
        self.blank_position = self.find_position(0)  # 找到空白（0）的位置

    def expand_positions(self):
        i, j = self.blank_position
        expanded_states = []
        if j > 0:  # 向左移动
            expanded_states.append(self.move_blank(0, -1))
        if j < 2:  # 向右移动
            expanded_states.append(self.move_blank(0, 1))
        if i > 0:  # 向上移动
            expanded_states.append(self.move_blank(-1, 0))
        if i < 2:  # 向下移动
            expanded_states.append(self.move_blank(1, 0))
        return expanded_states

    def move_blank(self, row_offset, col_offset):
        new_state = copy.deepcopy(self.state)
        target_value = new_state[self.blank_position[0] + row_offset][self.blank_position[1] + col_offset]
        new_state[self.blank_position[0]][self.blank_position[1]] = target_value
        new_state[self.blank_position[0] + row_offset][self.blank_position[1] + col_offset] = 0
        return new_state  # 返回新的状态

def state_exists(state, state_list):
    # 检查给定状态是否在状态列表中
    for index, existing_state in enumerate(state_list):
        if existing_state.state == state.state:  # 若状态相同
            return [True, index]  # 返回存在标记和索引
    return [False, -1]

def count_inversions(array):
    inversions = 0
    for i in range(len(array)):
        if array[i] != 0:
            for j in range(i):
                if array[j] > array[i]:
                    inversions += 1
    return inversions

def is_solvable(initial_state, target_state):
    inv_count_initial = count_inversions([num for row in initial_state for num in row])
    inv_count_target = count_inversions([num for row in target_state for num in row])
    return (inv_count_initial % 2) == (inv_count_target % 2)  # 判断逆序数的奇偶性

def a_star_solver(initial_state):
    open_set = []  # 用于存储待扩展的状态
    closed_set = []  # 用于存储已扩展的状态

    puzzle = PuzzleGrid(initial_state)
    if not is_solvable(initial_state, puzzle.target_state):
        print("该八数码问题无解，请检查输入。")
        return

    open_set.append(puzzle)  # 将初始状态添加到open_set
    move_count = 0  # 记录移动次数
    expansion_count = 0  # 记录扩展次数

    while open_set:
        open_set.sort(key=lambda x: x.f)
        current_state = open_set.pop(0)

        print("当前状态：")
        current_state.display()

        if current_state.h == 0:
            print("找到解决方案，移动次数：", move_count)
            current_state.display_solution_path()
            return

        closed_set.append(current_state)
        for new_state in current_state.expand_positions():  # 扩展当前状态
            new_puzzle = PuzzleGrid(new_state)
            new_puzzle.previous = current_state
            new_puzzle.calculate_costs()

            # 只在open_set和closed_set中都不存在时才添加新状态
            if not state_exists(new_puzzle, open_set)[0] and not state_exists(new_puzzle, closed_set)[0]:
                expansion_count += 1  # 每扩展一个新状态，计数器加一
                print(f"扩展次数：{expansion_count}，扩展的新状态：")
                new_puzzle.display()

                open_set.append(new_puzzle)  # 添加到open_set

            in_closed = state_exists(new_puzzle, closed_set)
            if in_closed[0] and new_puzzle.f < closed_set[in_closed[1]].f:
                closed_set[in_closed[1]] = new_puzzle  # 更新closed_set
                open_set.append(new_puzzle)  # 添加到open_set
                move_count += 1

            in_open = state_exists(new_puzzle, open_set)
            if in_open[0] and new_puzzle.f < open_set[in_open[1]].f:
                open_set[in_open[1]] = new_puzzle  # 更新open_set
                move_count += 1

# 使用示例
initial_configuration = [[2, 8, 3], [1, 0, 4], [7, 6, 5]]
initial_configuration = [[2,7,0], [8, 4, 1], [3,5,6]]  # 初始状态
a_star_solver(initial_configuration)  # 调用A*算法求解
