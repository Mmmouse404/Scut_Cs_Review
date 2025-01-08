using MPI

MPI.Init()

function matmul_mpi_3!(C, A, B)
    comm = MPI.COMM_WORLD
    rank = MPI.Comm_rank(comm)
    size1 = MPI.Comm_size(comm)

    # 获取矩阵的维度
    N = size(A, 1)  # 假设 A 和 B 是正方形矩阵

    # 计算每个进程应该处理的行数
    rows_per_proc = N ÷ size1
    println("Rank $rank: rows_per_proc = $rows_per_proc")

    # 每个进程的局部 A 和 C
    local_A = zeros(eltype(A), rows_per_proc, N)
    local_C = zeros(eltype(C), rows_per_proc, N)

    # 每个进程接收完整的 B
    local_B = B

    # 点对点通信：rank 0 将 A 的行分发给其他进程
    if rank == 0
        for r in 1:size1
            start_row = (r - 1) * rows_per_proc + 1
            end_row = r * rows_per_proc
            if r == 1
                # rank 0 处理自己的部分
                local_A = view(A, start_row:end_row, :)
            else
                # 发送数据到其他进程
                MPI.Send(view(A, start_row:end_row, :), r - 1, 0, comm)
            end
        end
    else
        # 其他进程接收数据
        MPI.Recv!(local_A, 0, 0, comm)
    end
    # 执行局部矩阵乘法
    for j in 1:size(local_B, 2)
        for k in 1:size(local_A, 2)
            for i in 1:rows_per_proc
                local_C[i, j] += local_A[i, k] * local_B[k, j]
            end
        end
    end

    # 点对点通信：将结果发送回 rank 0
    if rank == 0
        # rank 0 收集所有结果
        C = zeros(eltype(C), N, N)
        for r in 1:size1
            start_row = (r - 1) * rows_per_proc + 1
            end_row = r * rows_per_proc
            if r == 1
                # rank 0 处理自己的部分
                C[start_row:end_row, :] = local_C
            else
                # 接收其他进程的结果
                MPI.Recv!(view(C, start_row:end_row, :), r - 1, 1, comm)
            end
        end
    else
        # 其他进程发送结果
        MPI.Send(local_C, 0, 1, comm)
    end

    # 同步所有进程，确保 rank 0 在检查 C 之前已经收集到所有结果
    MPI.Barrier(comm)

    return C  # 返回修改后的 C
end

function testit(load)
    comm = MPI.COMM_WORLD
    rank = MPI.Comm_rank(comm)
    P = MPI.Comm_size(comm)
    N = load * P
    println("Matrix size: $N")

    # 在 rank 0 生成矩阵 A 和 B，并通过广播发送给所有进程
    if rank == 0
        A = rand(Int64, N, N)  # 生成 Int64 矩阵
        B = rand(Int64, N, N)  # 生成 Int64 矩阵
    else
        A = zeros(Int64, N, N)  # 其他进程初始化空矩阵
        B = zeros(Int64, N, N)  # 其他进程初始化空矩阵
    end

    # 广播 A 和 B 到所有进程
    MPI.Bcast!(A, 0, comm)
    MPI.Bcast!(B, 0, comm)

    # 分配 C 为 Int64 矩阵
    C = zeros(eltype(A), N, N)
    C = matmul_mpi_3!(C, A, B)  # 调用函数并更新 C

    # 同步所有进程，确保 rank 0 在检查 C 之前已经收集到所有结果
    MPI.Barrier(comm)

    if rank == 0

        if !(C ≈ A*B)
            println("Test failed 😢")
        else
            println("Test passed 🥳")
        end
    end
end

testit(100)  
MPI.Finalize()