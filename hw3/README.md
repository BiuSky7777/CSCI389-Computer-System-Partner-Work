| Test Name | Desciption | Test Status |
| :--- |:---: | :---: |
| Set/Get | `set` a key and check if the value of this existed key is correctly returned by calling `get`. | Pass |
| Set/Get | `set` a key and check if the size of the existed key's value is correctly returned by calling `get`. | Pass |
| Set/Get | Check if the value of a nonexisted key is correctly returned by calling `get`.  | Pass |
| Set/Get | Check if the value of the size of a nonexisted key's value is correctly returned by calling `get`.  | Pass |
| Del | Check if the selected key is successfully deleted from the cache by calling `del` and the used_space has decreases. | Pass |
| Del | Check that the selected key cannot be got after the deletion. | Pass |
| Space_used |  |  |
| Evict | 1.51957 | |
| Reset | 1.51957 | |

| Test Name | Desciption | Test Status |
| :--- |:---: | :---: |
| Set/Get | `set` a key and check if the value of this existed key is correctly returned by calling `get`. | Pass |
| Set/Get | `set` a key and check if the size of the existed key's value is correctly returned by calling `get`. | Pass |
| Set/Get | Check if the value of a nonexisted key is correctly returned by calling `get`.  | Pass |
| Set/Get | Check if the value of the size of a nonexisted key's value is correctly returned by calling `get`.  | Pass |
| Del | Check if the selected key is successfully deleted from the cache by calling `del` and the used_space has decreases. | Pass |
| Del | Check that the selected key cannot be got after the deletion. | Pass |
| Space_used |  |  |
| Evict | 1.51957 | |
| Reset | 1.51957 | |


Test name.
A description of what is tested (not how--document your code instead).
Test status (fail/pass).


mason: did not include unordered map 
      pair didn't define template 
      
Reily: included <map> instead of <unordered map>
        space_used->initial size = 2 != 0
        get did not return correct size
        delition did not return true when something get deleted
  
