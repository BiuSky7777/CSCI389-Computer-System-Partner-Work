# CSCI389-Computer-System-Partner-Work
Collaboration: Kaiyan Shi, Jiarong Li

## File Descriptions
+ cache.hh: Header file for cache_lib.cc.
+ cache_lib.cc: The implementations of cache, including set, get, del, space_used, reset functions.
+ test_cache_lib.cc: Tests for inplementations included in cache_lib.cc.
+ fifo_evictor.hh: Header file for fifo_evictor.cc.
+ fifo_evictor.cc: The implementations of evictor, which regulates the perfermance of cache. 

## Cache Structure
We built a cache object with the data structure of an unordered map `{key: (value, size)}`. The Cache obect will be initialized by taking in 4 variables, `maxmem`, `max_load_factor`, `evictor`, `hasher`. We set the mamximal memoery to `maxmem` (sum of the size of all keys < `maxmem`), overwrite the maximal ratio of the size of the map and its number of buckets to `max_load_factor`, use evictor object as our eviction policy, and replace the default hash function for the unordered map by `hasher`. We chose unordered map because unordered containers organize their elements using hash tables that allow for fast access to elements by their key. Without the extra runtime for probing, all operations run in asymptotic constant time: O(1).

An evictor object was given a data structure of queue, which contributes to a convinient implementation of out eviction policy: when the user wants to add a new key with size that cannot be contained in current cache, the cache automatically deletes from old keys in stead of recently added ones. Thus, we set `touch_key()` function to, whenever `set()` or `get()` is called, the corresponding key is added to the end of `myqueue`. And the function `evict()` pops out front elements of `myqueue`. 


## Testing
The test_cache_lib.cc file is wrote for testing `set()`, `get()`, `del()`, `space_used()`,`evict()`, and `reset()` functions of cache. In each test, we create a cache and set, get or delete some values and assert they are equal to what we expected. All these tests passed (more detailed comments are in the code file).

## Collision Resolution
By adopting unordered map, which allows us to include the customized functionality to utilize `umap_`'s properties. In this case, we overwrited its own hash function by using `hasher` so that, in the process of hashing, `umap_` will use buckets, which enable us to avoid collisions.

## Dynamic Resizing
Unordered map has its own way to modify the ratio between the amount of table rows and element buckets. By initializing `umap_.max_load_factor()`, we customized the max load factor of our `umap_` so that it will automatically keep its load_factor below assigned number, which helped us to achieve the dynamic resizing.

## Run the Test
```
g++ test_cache_lib.cc cache_lib.cc fifo_evictor.cc -std=c++17 -o main
```
```
\.main
```
