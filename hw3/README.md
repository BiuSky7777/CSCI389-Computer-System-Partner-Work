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
| Del 1 | Fail |
| Del 2 | Fail |
| Del 3 | Fail |
| Space_used 1 | Fail |
| Space_used 2 | Pass |
| Space_used 3 | Pass |
| Evict 1 | Fail |
| Evict 2 | Fail |
| Reset 1 | Pass |
| Reset 2 | Fail |

### Analysis
Failures are caused by the following problems:
+ Compliation Failure: included <map> instead of <unordered map>
+ Space_used seems not to be initialized as 0 (the initialized space_used is 2 by `Test Space_used 1`)
+ Second input of `get` is not modified to the correct rectuned val's size (most failures were caused by this problem)
     
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
+ When deleting a nonexisted key, `del` retuned `true`

## 2ed Project: Mason Koch & Sebastian Simons

| Test Name | Test Status |
| :--- |:---: |
| Set/Get 1 | Pass |
| Set/Get 2 | Fail |
| Set/Get 3 | Pass |
| Set/Get 4 | Fail |
| Del 1 | Fail |
| Del 2 | Fail |
| Del 3 | Fail |
| Space_used 1 | Fail |
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
+ Space_used seems not to be initialized as 0 (the initialized space_used is 2 by `Test Space_used 1`)
+ Second input of `get` is not modified to the correct rectuned val's size (most failures were caused by this problem)
  
+ When deleting a nonexisted key, `del` retuned `true`.
## 3rd Project: Hrishee & Prasun


Test name.
A description of what is tested (not how--document your code instead).
Test status (fail/pass).


mason: 
      
      
Reily: included <map> instead of <unordered map>
        space_used->initial size = 2 != 0
        get did not return correct size
        delition did not return true when something get deleted
  
