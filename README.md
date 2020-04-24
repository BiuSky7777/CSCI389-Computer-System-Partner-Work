# HW 6 Crank it up
Collaboration: Kaiyan Shi, Jiarong Li

## Part 1: multi-threaded benchmark
We adopted pthread to acccomplish multi-threading clients.
`run_thread()` function encapsulates our benchmarking function (baseline_latencies) in hw 5 in an independent thread.
`threaded_performance()` function launches `nthreads` worker threads and each of them sends `nreq` and `initial_size`(warm-up) to the cache server and returns the resulting vector of latencies. After combining all generated latencies, the main thread finds the 95th-percentile latency and throughput of all the threads.

95th-percentile Latency:

![13831587694431_ pic_hd](https://user-images.githubusercontent.com/43858610/80168035-c383ba00-8596-11ea-9e7c-fd8ab502c43e.jpg)

Mean Throughput:

![13841587694440_ pic_hd](https://user-images.githubusercontent.com/43858610/80168042-c54d7d80-8596-11ea-94c8-64ec714270d4.jpg)

## Part 2: multithreaded server
We utilized the built-in function in crow library to create a multithreaded server. 

## To Run the Code
The default size of server is 10000
First imput the following commmand line to run the server:

`g++ -std=c++17 -I../crow/include -Wall -Wextra cache_lib.cc cache_server.cc fifo_evictor.cc -o server -pthread -lboost_system`

Then, open another terminal window and input the following command line to run multithread clients:

`g++ -std=c++17 workload_generator.cc cache_client.cc -o main.exe -lcurl -pthread`
