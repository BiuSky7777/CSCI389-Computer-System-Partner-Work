# HW 6 Crank it up
Collaboration: Kaiyan Shi, Jiarong Li

## Part 1: multi-threaded benchmark
We adopted pthread to acccomplish multi-threading clients.
`run_thread()` function encapsulates our benchmarking function (baseline_latencies) in hw 5 in an independent thread.
`threaded_performance()` function launches `nthreads` worker threads and each of them sends `nreq` and `initial_size`(warm-up) to the cache server and returns the resulting vector of latencies. After combining all generated latencies, the main thread finds the 95th-percentile latency and throughput of all the threads.

We set `nthread` from 1 to 20 in order to find the actual limits of the cache. The results generated are shown below.

95th-percentile Latency:

![13831587694431_ pic_hd](https://user-images.githubusercontent.com/43858610/80168035-c383ba00-8596-11ea-9e7c-fd8ab502c43e.jpg)

Mean Throughput:

![13841587694440_ pic_hd](https://user-images.githubusercontent.com/43858610/80168042-c54d7d80-8596-11ea-94c8-64ec714270d4.jpg)

**Analysis:**

+ 95 percentile latency: We got an obvious jump at number of threads = 13. The maximum unsaturated latency is around 6.8 milliseconds.

+ Mean throughput: The saturation point seems to occur at number of threads = 8. The maximum unsaturated throughput is around 5.6 milliseconds.



## Part 2: multithreaded server
We utilized the built-in function in crow library to create a multithreaded server. We modified `cache_lib.cc` by adding `pthread_mutex_lock` at the beginning and `pthread_mutex_unlock` in the end of every called cache function so that no changes will be made to the cache by multiple threads at the same time, i.e. multi-threaded server can access cache.

We set `nthread` from 1 to 20 in order to find the actual limits of the cache. The results generated are shown below.

95th-percentile Latency:

![13871587695726_ pic_hd](https://user-images.githubusercontent.com/43858610/80169175-9edd1180-8599-11ea-80bb-09bc46ce6626.jpg)

Mean Throughput:

![13881587695736_ pic_hd](https://user-images.githubusercontent.com/43858610/80169187-a7354c80-8599-11ea-9f29-3710558a43ec.jpg)

**Analysis:**

+ 95 percentile latency: We got the first jump at number of threads = 6, and the second jump at number of threads = 13. The maximum unsaturated latency is around 5.8 milliseconds and the first jump, and 5.93 milliseconds at the second jump.

+ Mean throughput: The saturation point seems to occur at number of threads = 5. The maximum unsaturated throughput is around 5.3 milliseconds.

## To Run the Code
The default size of server is 10000
First imput the following commmand line to run the server:

`g++ -std=c++17 -I../crow/include -Wall -Wextra cache_lib.cc cache_server.cc fifo_evictor.cc -o server -pthread -lboost_system`

Then, open another terminal window and input the following command line to run multithread clients:

`g++ -std=c++17 workload_generator.cc cache_client.cc -o main.exe -lcurl -pthread`
