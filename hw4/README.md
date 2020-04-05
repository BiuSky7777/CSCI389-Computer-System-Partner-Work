# HW 4 Let's Network

## Compile and Run

### Server
We use crow(github address: https://github.com/ipkn/crow) to construct the cache_server.cc.
1. We followed its instruction to download boost.
2. Due to the version of boost used in crow, we modified the "CSCI389-Computer-System-Partner-Work\crow\include\crow\include\crow\socket.adptors.hh" according to https://git.xmr.pm/wownero/onion-wownero-blockchain-explorer/commit/444effc2878ae714feb9c3087c81b4f2f25975be. (We have included the new crow folders and so there is no need for you to implement this step.)
3. To run the server, we use the command line:
```
g++ -std=c++17 -I../crow/include -Wall -Wextra cache_lib.cc cache_server.cc fifo_evictor.cc -o server -pthread -lboost_system
./server 5 11112
```
With one window running, we open another window and give instructions like:
```
curl -X Put localhost:11112/keyval/a/apple
curl -I localhost:11112/head
```

### Client

We use libcurl (https://curl.haxx.se/libcurl/) to construct the cache_client.cc.
1. Since there is no explicit way to retrieve data in libcurl, we write a callback function according to https://curl.haxx.se/mail/lib-2012-06/0308.html. Then we use this callback function, CURLOPT_WRITEFUNCTION and CURLOPT_WRITEDATA in libcurl to implement the retrieval.
2. To run the client and test it, we keep the server running (with ./server 5 11112), and then use the command line:
```
client: g++ -std=c++17 -o client cache_client.cc test_cache_client.cc  -lcurl
./client
```

## Test
| Test Name | Desciption | Test Status |
| :--- |:---: | :---: |
| Set/Get 1 | `set` a key and check if the value of this existed key is correctly returned by calling `get`. | Pass |
| Set/Get 2 | `set` a key and check if the size of the existed key's value is correctly returned by calling `get`. | Pass |
| Set/Get 3 | Check if the value of a nonexisted key is correctly returned by calling `get`.  | Pass |
| Set/Get 4 | Check if the value of the size of a nonexisted key's value is correctly returned by calling `get`.  | Pass |
| Del 1 | Check if the selected key is successfully deleted from the cache by calling `del` and the used_space has decreases. | Pass |
| Del 2 | Check that the selected key cannot be got after the deletion. | Fail |
| Del 3 | Check that a nonexisted key cannot be deleted. | Fail |
| Space_used1 | Check if the initial Cache has 0 space used. | Pass |
| Space_used2 | Check if Cache has correct space used after setting keys. | Pass |
| Space_used3 | Check if Cache has correct space used after deleting keys. | Fail |
| Evict 1 | Check if Cache evicts enough space for newly added key when only one key need to be evicted, and the eviction follows the right order. |  Pass |
| Evict 2 | Check if Cache evicts enough space for a newly added key when multiple keys need to be evicted, and the eviction follows the right order. |  Pass |
| Reset 1 | Check if `reset` cleans Cache and gives back 0 used space | Pass |
| Reset 2 | Check that, after `reset` cleans Cache, no key can be retrieved | Pass |

Reason:
We have those failures every time we directly call del/key/<string> (from cache_server.cc) for nonexisted key.
It always gives us "bus error" and "Segmentation fault", and so we thought the failures are due memory allocation problem which we might not handle in DELETE method in cache_server.cc.
