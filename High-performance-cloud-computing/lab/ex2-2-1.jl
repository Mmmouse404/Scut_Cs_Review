using MPI
MPI.Init()
function matmul_mpi_3!(C, A, B)
    comm = MPI.COMM_WORLD  # 获取 MPI 通信器
    rank = MPI.Comm_rank(comm)  # 获取当前进程的 rank
    P = MPI.Comm_size(comm)  # 获取进程总数
    root = 0  # 定义主进程的 rank
    if rank == root
        N = size(A, 1)  # 获取矩阵 A 的行数，即矩阵 C 的行数
        Nref = Ref(N)  # 创建一个引用对象，用于广播 N
    else
        Nref = Ref(0)  # 其他进程初始化 Nref 为 0
    end
    MPI.Bcast!(Nref, comm; root) 
    N = Nref[] 
    if rank == root
        B_local = B  # 主进程直接使用全局矩阵 B
    else
        B_local = zeros(N, N)  # 其他进程初始化局部矩阵 B
    end
    MPI.Bcast!(B_local, comm; root)  # 广播矩阵 B 到所有进程
    # 计算每个进程处理的列数
    L = div(N, P) 
    A_local_t = zeros(N, L) 
    A_t = collect(transpose(A)) 
    MPI.Scatter!(A_t, A_local_t, comm; root)  # 每个进程接收 A_t 的一部分
    # 每个进程计算局部矩阵乘法（转置形式）
    C_local_t = transpose(B_local) * A_local_t  # 计算局部结果矩阵 C_local（转置形式）
    C_t = similar(C)  # 初始化全局矩阵 C 的转置形式
    MPI.Gather!(C_local_t, C_t, comm; root)  # 收集所有进程的局部结果
    C .= transpose(C_t)  # 将转置后的结果还原为全局矩阵 C
    C
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
    println("Rank $rank: A = $A")
    # 同步所有进程，确保 rank 0 在检查 C 之前已经收集到所有结果
    MPI.Barrier(comm)
    if rank == 0
        if !(C ≈ A * B)
            println("Test failed 😢")
            println("A * B = ", A * B)
            println("C = ", C)
        else
            println("Test passed 🥳")
        end
    end
end
testit(100)
MPI.Finalize()
