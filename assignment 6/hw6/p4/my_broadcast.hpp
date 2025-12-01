#ifndef MY_BROADCAST_HPP
#define MY_BROADCAST_HPP

#include <mpi.h>

// Professor’s naive broadcast: root sends data to all others using MPI_Send/MPI_Recv
//   T      : data type (treated as bytes)
//   count  : number of bytes to send
//   root   : rank of broadcasting process
//   comm   : MPI communicator
template <typename T>
void my_broadcast(T *data, int count, int root, MPI_Comm comm)
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; ++i) {
            if (i != root) {
                MPI_Send(data, count, MPI_BYTE, i, 0, comm);
            }
        }
    } else {
        MPI_Recv(data, count, MPI_BYTE, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

#endif // MY_BROADCAST_HPP
