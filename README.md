# HW 6 Crank it up
Collaboration: Kaiyan Shi, Jiarong Li

## Part 1: multi-threaded benchmark
We adopted pthread to acccomplish multi-threading clients.
`run_thread()` function encapsulates our benchmarking function (baseline_latencies) in hw 5 in an independent thread.
`threaded_performance()` function launches `nthreads` worker threads and each of them sends `nreq` and `initial_size`(warm-up) to the cache server and returns the resulting vector of latencies.

## Part 2: multithreaded server
We utilized the built-in function in crow library to create a multithreaded server. 

## To Run the Code
The default size of server is 10000
First imput the following commmand line to run the server:

`g++ -std=c++17 -I../crow/include -Wall -Wextra cache_lib.cc cache_server.cc fifo_evictor.cc -o server -pthread -lboost_system`

Then, open another terminal window and input the following command line to run multithread clients:

`g++ -std=c++17 workload_generator.cc cache_client.cc -o main.exe -lcurl -pthread`
