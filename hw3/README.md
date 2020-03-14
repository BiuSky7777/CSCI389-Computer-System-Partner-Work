# Part 1
| Test Name | Desciption | Test Status |
| :--- |:---: | :---: |
| Set/Get 1 | `set` a key and check if the value of this existed key is correctly returned by calling `get`. | Pass |
| Set/Get 2 | `set` a key and check if the size of the existed key's value is correctly returned by calling `get`. | Pass |
| Set/Get 3 | Check if the value of a nonexisted key is correctly returned by calling `get`.  | Pass |
| Set/Get 4 | Check if the value of the size of a nonexisted key's value is correctly returned by calling `get`.  | Pass |
| Del 1 | Check if the selected key is successfully deleted from the cache by calling `del` and the used_space has decreases. | Pass |
| Del 2 | Check that the selected key cannot be got after the deletion. | Pass |
| Del 3 | Check that a nonexisted key cannot be deleted. | Pass |
| Space_used1 | Check if the initial Cache has 0 space used. | Pass |
| Space_used2 | Check if Cache has coreect space used after setting keys. | Pass |
| Space_used3 | Check if Cache has coreect space used after deleting keys. | Pass |
| Evict 1 | Check if Cache evicts enough space for newly added key when only one key need to be evicted, and the eviction follows the right order. |  Pass |
| Evict 2 | Check if Cache evicts enough space for a newly added key when multiple keys need to be evicted, and the eviction follows the right order. |  Pass |
| Reset 1 | Check if `reset` cleans Cache and gives back 0 used space | Pass |
| Reset 2 | Check that, after `reset` cleans Cache, no key can be retrieved | Pass |


# Part 2

## 1st Poject: Reilly Cannon & James McCaull
### Test Result
| Test Name | Test Status |
| :--- |:---: |
| Set/Get 1 | Pass |
| Set/Get 2 | Pass |
| Set/Get 3 | Pass |
| Set/Get 4 | Fail |
| Del 1 | Pass |
| Del 2 | Fail |
| Del 3 | Pass |
| Space_used 1 | Pass |
| Space_used 2 | Pass |
| Space_used 3 | Pass |
| Evict 1 | Pass |
| Evict 2 | Fail |
| Reset 1 | Pass |
| Reset 2 | Fail |

### Analysis
Failures are caused by the following problems:
+ Compliation Failure: included <map> instead of <unordered map>
+ Second input of `get` is not modified to the correct rectuned val's size (All failures were caused by this problem)
 
 For example (When we call `get` function, it should set the actual size of the returned value (in bytes) in val_size, the second input of `get`. However, this program didn't make the modification) 

 ```
 Set/Get 4
 The size of the nonexisted key's value is returned correctly.
 -------------------------------------------------------------------------------
 testy_cache.cc:69
 ...............................................................................

 testy_cache.cc:82: FAILED:
   REQUIRE( size == 0 )
 with expansion:
   2 == 0
```     

## 2ed Project: Mason Koch & Sebastian Simons

| Test Name | Test Status |
| :--- |:---: |
| Set/Get 1 | Pass |
| Set/Get 2 | Fail |
| Set/Get 3 | Pass |
| Set/Get 4 | Fail |
| Del 1 | Pass |
| Del 2 | Fail |
| Del 3 | Pass |
| Space_used 1 | Pass |
| Space_used 2 | Pass |
| Space_used 3 | Pass |
| Evict 1 | Fail |
| Evict 2 | Fail |
| Reset 1 | Pass |
| Reset 2 | Fail |

### Analysis
Failures are caused by the following problems:
+ Compliation Failure: Did not include <unordered map>
+ Compilation Failure: Did not define template  for Pair object
+ Second input of `get` is not modified to the correct rectuned val's size (most failures were caused by this problem)

## 3rd Project: Hrishee & Prasun

| Test Name | Test Status |
| :--- |:---: |
| Set/Get 1 | Pass |
| Set/Get 2 | Fail |
| Set/Get 3 | Pass |
| Set/Get 4 | Fail |
| Del 1 | Fail |
| Del 2 | Fail |
| Del 3 | Pass |
| Space_used 1 | Pass |
| Space_used 2 | Pass |
| Space_used 3 | Fail |
| Evict 1 | Fail |
| Evict 2 | Fail |
| Reset 1 | Pass |
| Reset 2 | Fail |

### Analysis
Failures are caused by the following problems:
+ Second input of `get` is not modified to the correct rectuned val's size (most failures were caused by this problem)
Test name.
+ Space_used was wong after deletion (`Test Del 1`, `Test Space_used 3`, `Test Evict 2` failed because of this problem)

For example (The original Cache has space 10, which is totally occupied by 5 pairs each with size of 2. Then, a new pair with size of 3 is added. In this case, 2 pairs should be evicted from the front and the new space_used should be 10-4+3=9. However, this program gives back 10): 

```
Evict 2
  Ensure that Cache evicts enough space for newly added key when multiple keys
  need to be evicted.
-------------------------------------------------------------------------------
  testy_cache.cc:216
...............................................................................

  testy_cache.cc:236: FAILED:
    REQUIRE( c.space_used() == 9 )
  with expansion:
    10 == 9
```


A description of what is tested (not how--document your code instead).
Test status (fail/pass).

