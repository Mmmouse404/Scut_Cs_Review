using MPI
MPI.Init()

function jacobi_mpi_latency_hiding(n, niters, comm)
    u, u_new = init(n, comm)
    load = length(u) - 2
    rank = MPI.Comm_rank(comm)
    nranks = MPI.Comm_size(comm)
    nreqs = 2 * ((rank != 0) + (rank != (nranks - 1)))
    reqs = MPI.Request[]
    for t in 1:niters
        ireq = 0
        if rank != 0
            neig_rank = rank - 1
            u_snd = view(u, 2:2)
            u_rcv = view(u, 1:1)
            dest = neig_rank
            source = neig_rank
            MPI.Isend(u_snd, dest, comm)
            MPI.Irecv!(u_rcv, source, comm)
        end
        if rank != (nranks - 1)
            neig_rank = rank + 1
            u_snd = view(u, (load + 1):(load + 1))
            u_rcv = view(u, (load + 2):(load + 2))
            dest = neig_rank
            source = neig_rank
            MPI.Isend(u_snd, dest, comm)
            MPI.Irecv!(u_rcv, source, comm)
        end
        for i in 2:load + 1
            u_new[i] = 0.5 * (u[i - 1] + u[i + 1])
        end
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
niters = 100
comm = MPI.Comm_dup(MPI.COMM_WORLD)
u = jacobi_mpi_latency_hiding(n, niters, comm)
u_root = gather_final_result(u, comm)
rank = MPI.Comm_rank(comm)
if rank == 0
    u_seq = jacobi(n, niters)
    if isapprox(u_root, u_seq)
        println("Test passed 🥳")
    else
        println("Test failed 😢")
    end
end