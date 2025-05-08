using MPI
function main()
    MPI.Init()
    rank = MPI.Comm_rank(MPI.COMM_WORLD)
    size = MPI.Comm_size(MPI.COMM_WORLD)
    # 初始化计数器
    counter = 0
    if rank == 0
        # 等级 0 生成一条信息（整数）
        msg = 0
        println("Rank $rank sending initial message: $msg")
        MPI.Send(msg, mod(rank + 1, size), 0, MPI.COMM_WORLD)
    end
    while true
        # 接收信息
        status = MPI.Probe(MPI.ANY_SOURCE, MPI.ANY_TAG, MPI.COMM_WORLD)
        (msg, _) = MPI.Recv(Int, status.source, status.tag, MPI.COMM_WORLD)
        println("Rank $rank received message: $msg")
        # 增加计数器
        counter += 1
        if rank == 0 && status.tag == 1
            # 如果等级 0 收到带有结束标记的消息，结束循环
            println("Rank $rank received final message: $msg")
            break
        end
        # 将信息递增 1
        msg += 1
        # 发送信息给下一个等级
        next_rank = mod(rank + 1, size)
        println("Rank $rank sending message: $msg to rank $next_rank")
        MPI.Send(msg, next_rank, 0, MPI.COMM_WORLD)
        # 检查是否达到 20 次通信
        if counter >= 20
            if rank == 0
                println("Rank $rank: Stopping communication after $counter communications.")
                # 发送结束标记给下一个等级
                MPI.Send(msg, next_rank, 1, MPI.COMM_WORLD)
            end 
            break  # 停止通信
        end
    end
    MPI.Finalize()
end
main()