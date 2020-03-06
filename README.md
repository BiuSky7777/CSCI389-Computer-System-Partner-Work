# CSCI389-Computer-System-Partner-Work
Collaboration: Kaiyan Shi, Jiarong Li

## File Descriptions
+ cache.hh: Header file for cache_lib.cc.
+ cache_lib.cc: The implementations of cache, including set, get, del, space_used, reset functions.
+ test_cache_lib.cc: Tests for inplementations included in cache_lib.cc.
+ fifo_evictor.hh: Header file for fifo_evictor.cc.
+ fifo_evictor.cc: The implementations of evictor, which regulates the perfermance of cache. 

## Cache Structure
We built a cache with the data structure of an unordered map `{key: (value, size)}`. The Cache obect will be initialized by taking in 4 variables, `maxmem`, `max_load_factor`, `evictor`, `hasher`. We set the mamximal memoery to `maxmem` (sum of the size of all keys < `maxmem`), overwrite the maximal ratio of the size of the map and its number of buckets to `max_load_factor`, use evictor object as our eviction policy, and replace the default hasch function for the unordered map by `hasher`.

## Testing


## Collision Resolution
We handled the collision detection by building a queue 
## Dynamic Resizing

## Run the Test
```
g++ test_cache_lib.cc cache_lib.cc fifo_evictor.cc -std=c++17 -o main
```
