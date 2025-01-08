using MPI
MPI.Init()
function jacobi_with_tol(n, tol)
    u = zeros(n + 2)  
    u[1] = -1 
    u[end] = 1 
    u_new = copy(u)  
    while true
        diff = 0.0 
        for i in 2:(n + 1) 
            ui_new = 0.5 * (u[i - 1] + u[i + 1]) 
            u_new[i] = ui_new 
            diff_i = abs(ui_new - u[i])  
            diff = max(diff_i, diff)  
        end
        if diff < tol 
            return u_new  
        end
        u, u_new = u_new, u  
    end
    u 
end
function jacobi_mpi_with_tol(n, niters, tol, comm)
    u, u_new = init(n, comm)  
    local_load = length(u) - 2  
    rank = MPI.Comm_rank(comm) 
    num_ranks = MPI.Comm_size(comm)  
    nreqs = 2 * ((rank != 0) + (rank != (num_ranks - 1)))
    reqs = MPI.MultiRequest(nreqs) 

    for t in 1:niters 
        ireq = 0 
        # 处理与左侧邻居的通信
        if rank != 0
            neig_rank = rank - 1  
            u_snd = view(u, 2:2)  
            u_rcv = view(u, 1:1)  
            dest = neig_rank 
            source = neig_rank  
            ireq += 1
            MPI.Isend(u_snd, comm, reqs[ireq]; dest)  # 异步发送
            ireq += 1
            MPI.Irecv!(u_rcv, comm, reqs[ireq]; source)  # 异步接收
        end
        # 处理与右侧邻居的通信
        if rank != (num_ranks - 1)
            neig_rank = rank + 1  
            u_snd = view(u, (local_load + 1):(local_load + 1)) 
            u_rcv = view(u, (local_load + 2):(local_load + 2)) 
            dest = neig_rank 
            source = neig_rank  
            ireq += 1
            MPI.Isend(u_snd, comm, reqs[ireq]; dest)  # 异步发送
            ireq += 1
            MPI.Irecv!(u_rcv, comm, reqs[ireq]; source)  # 异步接收
        end
        # 等待所有通信完成
        MPI.Waitall(reqs)
        mydiff = 0.0  
        for i in 2:local_load + 1
            u_new[i] = 0.5 * (u[i - 1] + u[i + 1]) 
            diff_i = abs(u_new[i] - u[i])  
            mydiff = max(mydiff, diff_i)  # 更新当前进程的最大差值
        end
        diff_ref = Ref(mydiff) 
        MPI.Allreduce!(diff_ref, max, comm)  # 全局归约，计算最大差值
        diff = diff_ref[]  
        if diff < tol
            return u_new  
        end
        # 交换 u 和 u_new
        u, u_new = u_new, u
    end
    return u
end
function init(n, comm)
    nranks = MPI.Comm_size(comm)  
    rank = MPI.Comm_rank(comm) 
    if mod(n, nranks) != 0  
        println("n must be a multiple of nranks")
        MPI.Abort(comm, 1) 
    end
    load = div(n, nranks)  
    u = zeros(load + 2)  
    if rank == 0  
        u[1] = -1 
    end
    if rank == nranks - 1 
        u[end] = 1  
    end
    u_new = copy(u)  
    u, u_new  
end
function gather_final_result(u, comm)
    load = length(u) - 2  
    rank = MPI.Comm_rank(comm)  
    if rank != 0  
        u_snd = view(u, 2:(load + 1))  
        MPI.Send(u_snd, 0, comm)  
        u_root = zeros(0)  
    else  
        nranks = MPI.Comm_size(comm)  
        n = load * nranks  
        u_root = zeros(n + 2) 
        u_root[1] = -1  
        u_root[end] = 1  
        lb = 2  
        ub = load + 1  
        u_root[lb:ub] = view(u, lb:ub) 
        for other_rank in 1:(nranks - 1)  
            lb += load  
            ub += load  
            u_rcv = view(u_root, lb:ub)  
            MPI.Recv!(u_rcv, other_rank, comm) 
        end
    end
    return u_root 
end

function jacobi(n, niters)
    u = zeros(n + 2)  
    u[1] = -1  
    u[end] = 1  
    u_new = copy(u)  
    for t in 1:niters  
        for i in 2:(n + 1) 
            u_new[i] = 0.5 * (u[i - 1] + u[i + 1]) 
        end
        u, u_new = u_new, u  
    end
    u 
end
n = 12  
tol = 1e-6  
niters = 1000  
comm = MPI.Comm_dup(MPI.COMM_WORLD) 
u = jacobi_mpi_with_tol(n, niters, tol, comm)  
u_root = gather_final_result(u, comm) 
rank = MPI.Comm_rank(comm)  
if rank == 0  
    u_seq = jacobi_with_tol(n, tol)  
    if isapprox(u_root, u_seq)  
        println("Test passed 🥳")  
    else
        println("Test failed 😢")  
    end
end