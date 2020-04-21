# CSCI389-Computer-System-Partner-Work
Collaboration: Kaiyan Shi, Jiarong Li

## Part 1: Workload generation
`workload_generator.cc`: This file inludes functions that generate random cache requests, keys, and values. 
We generate key size and val size values from extreme value distribution with coefficients given in the paper. Given the size, we generate key and val by randomly choose the size of characters from all lower, upper letters and Arabic numbers.
We set get-(setdel) ratio being 1/30 as described in the paper, and generate random requests based on the ratio. 
We have a set_storage_vec to store all the keys set with those deleted get deleted. But we keep evicted keys to calculate the hit rate. We randomly get a key from this set and try to GET this key. It the value of the key can be retrieved, we increment the hit. Finally we can calcualte hit rate.

Here are the parameters we choose to achieve 80% hit rate:

+ cache's maxmem size: 30000
+ warm-up set number to fill the cache= 1000
+ number of requests = 500
+ load factor: 0.75
+ compiler: -o

hit rate = 0.809859

## Part 2: Baseline latency and throughput

Given the above parameters, we get 95 percentile timing: 5.75347ms and mean thoughput: 5.12167ms. Here is our histogram plot:

put plot 1


## Part 3: Sensitivity testing

We choose to change cache's maxmem size, warm-up set number to fill the cache,  load factor and compiler. We call the previous setup Case0, and we have the following four cases with one of these parameters get changed:

+ Case1:
cache's maxmem size: 30000
warm-up set number to fill the cache= 1000
number of requests = 500
load factor: 0.75
**compiler: -O3**

95 peercentile: 5.79399
mean: 5.15219

put plot 2

+ Case2:
cache's maxmem size: 30000
**warm-up set number to fill the cache= 3000**
number of requests = 500
load factor: 0.75
compiler: -o

95 peercentile: 5.72499
mean: 5.24483

put plot 3

+ Case3:
cache's maxmem size: 30000
warm-up set number to fill the cache= 1000
number of requests = 500
**load factor: 0.9**
compiler: -o

95 peercentile: 5.76967
meean: 5.22831

put plot 4

+ Case4:
**cache's maxmem size: 300000**
warm-up set number to fill the cache= 1000
number of requests = 500
load factor: 0.75
compiler: -o

95 peercentile: 5.79056
meean: 5.36057

put plot 5


