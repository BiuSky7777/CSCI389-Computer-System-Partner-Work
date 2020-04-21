# CSCI389-Computer-System-Partner-Work
Collaboration: Kaiyan Shi, Jiarong Li

## Part 1: Workload generation
`workload_generator.cc`: This file inludes functions that generate random cache requests, keys, and values. 

We generate key size and val size values from extreme value distribution with coefficients given in the paper. Given the size, we generate key and val by randomly choose the size of characters from all lower, upper letters and Arabic numbers.

We set get-(setdel) ratio being 1/30 as described in the paper, and generate random requests based on the ratio. 

We have a set_storage_vec to store all the keys set with those deleted get deleted. But we keep evicted keys to calculate the hit rate.

We randomly get a key from this set and try to GET this key. It the value of the key can be retrieved, we increment the hit. Finally we can calcualte hit rate.

Here are the parameters we choose to achieve 80% hit rate:

+ cache's maxmem size: 30000
+ warm-up set number to fill the cache= 1000
+ number of requests = 500
+ load factor: 0.75
+ compiler: -o

hit rate = 0.809859

## Part 2: Baseline latency and throughput

Given the above parameters, we get **95 percentile timing: 5.75347ms** and **mean thoughput: 5.12167ms**. Here is our histogram plot:

![13511587437155_ pic_hd](https://user-images.githubusercontent.com/43858610/79820365-e74dc280-8340-11ea-8faf-c9ae001ca25f.jpg)


## Part 3: Sensitivity testing

We choose to change cache's maxmem size, warm-up set number to fill the cache,  load factor and compiler. We call the previous setup Case0, and we have the following four cases with one of these parameters get changed:

+ Variation 1:

cache's maxmem size: 30000

warm-up set number to fill the cache= 1000

number of requests = 500

load factor: 0.75

**compiler: -O3**

-------Result------------------

95 peercentile: 5.79399

mean: 5.15219


![13521587437155_ pic_hd](https://user-images.githubusercontent.com/43858610/79820397-f92f6580-8340-11ea-8318-c4007a586be3.jpg)

+ Variation 2:

cache's maxmem size: 30000

**warm-up set number to fill the cache= 3000**

number of requests = 500

load factor: 0.75

compiler: -o

-------Result------------------

95 percentile: 5.72499

mean: 5.24483

![13531587437156_ pic_hd](https://user-images.githubusercontent.com/43858610/79820404-ffbddd00-8340-11ea-8370-420406787971.jpg)

+ Variation 3:

cache's maxmem size: 30000

warm-up set number to fill the cache= 1000

number of requests = 500

**load factor: 0.9**

compiler: -o

-------Result------------------

95 percentile: 5.76967

mean: 5.22831

![13541587437157_ pic_hd](https://user-images.githubusercontent.com/43858610/79820420-08161800-8341-11ea-8701-bb574b4ab2d2.jpg)

+ Variation 4:

**cache's maxmem size: 300000**

warm-up set number to fill the cache= 1000

number of requests = 500

load factor: 0.75

compiler: -o

-------Result------------------

95 percentile: 5.79056

mean: 5.36057

![13551587437158_ pic_hd](https://user-images.githubusercontent.com/43858610/79820431-0ea48f80-8341-11ea-98bf-a58f3cea542f.jpg)

+ Conclusion: According to the data shown above, there is no significant effect caused by the chosen variations.
