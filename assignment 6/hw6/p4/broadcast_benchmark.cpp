#include <iostream>
#include <mpi.h>
#include <vector>
#include <chrono>
#include "my_broadcast.hpp"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int root = 0;

    if (rank == root) {
        cout << "size,my_bcast_bw,mpi_bcast_bw\n";
    }

    // Test message sizes: 8 bytes up to 256MB (doubling each time)
    const int num_sizes = 26;
    const int base_size = 8;  // starting at 8 bytes

    for (int i = 0; i < num_sizes; ++i) {
        int count = base_size * (1 << i);
        vector<char> data(count, 'a');

        // Warmup barrier
        MPI_Barrier(MPI_COMM_WORLD);
        // Time custom my_broadcast
        auto t0 = high_resolution_clock::now();
        my_broadcast(data.data(), count, root, MPI_COMM_WORLD);
        auto t1 = high_resolution_clock::now();
        MPI_Barrier(MPI_COMM_WORLD);
        double my_time = duration_cast<duration<double>>(t1 - t0).count();

        // Time MPI_Bcast
        MPI_Barrier(MPI_COMM_WORLD);
        t0 = high_resolution_clock::now();
        MPI_Bcast(data.data(), count, MPI_BYTE, root, MPI_COMM_WORLD);
        t1 = high_resolution_clock::now();
        MPI_Barrier(MPI_COMM_WORLD);
        double mpi_time = duration_cast<duration<double>>(t1 - t0).count();

        // Compute bandwidth (MB/s)
        double my_bw  = (count / (1024.0*1024.0)) / my_time;
        double mpi_bw = (count / (1024.0*1024.0)) / mpi_time;

        if (rank == root) {
            cout << count << "," << my_bw << "," << mpi_bw << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
