
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <string.h>
#include <stdio.h>
#include "cache.hh"
#include "fifo_evictor.hh"
using namespace std;


class Cache::Impl {
  size_type maxmem_;
  float max_load_factor_;
  Evictor* fifo_evictor;
  hash_func hasher_;

  size_type memused_;

  struct Element {
    val_type val;
    size_type size;
  };

  unordered_map<key_type,Element,hash_func> umap_;

public:

  Impl(size_type maxmem, float max_load_factor, Evictor* evictor, hash_func hasher) :
   maxmem_(maxmem), fifo_evictor(evictor), memused_(0), umap_(unordered_map<key_type,Element,hash_func>(0,hasher_)), max_load_factor_(max_load_factor), hasher_(hasher) {
    umap_.max_load_factor(max_load_factor_);
  }

  void set(key_type key, val_type val, size_type size)
  {
    if (fifo_evictor == nullptr) {
      cout << "Evictior is a nullptr. Crash!!!" << endl;
      return;
    }

    auto valcopy = new byte_type[size]; //memory copy of the value
    std::copy(val, val + size, valcopy);

    Element key_pair;
    key_pair.val = val;
    key_pair.size = size;

    if(size <= maxmem_){
        while (memused_ + size > maxmem_)
        {
          key_type retkey = fifo_evictor->evict();
          del(retkey);
        }
        // Find if key already exists in the Cache
        auto search = umap_.find(key);
        if (search != umap_.end()) {
            //overwrite
            memused_ -= (search -> second).size;
            (search -> second).val = val;
            (search -> second).size = size;
            memused_ += size;
            fifo_evictor->touch_key(key);
        }else{
            // insert
            umap_.insert({key, key_pair});
            memused_ += size;
            fifo_evictor->touch_key(key);
        }
    } else {
      cout << "Cache is not big enough to store this key-val pair.";
    }
  }

  val_type get(key_type key, size_type& val_size)
  {
    auto search = umap_.find(key);
    if (search != umap_.end()) {
        val_type retval = (search -> second).val;
        val_size = (search -> second).size;
        return retval;
    } else {
        val_size = 0;
        return nullptr;
    }
    if (fifo_evictor == nullptr) {
      cout << "Evictior is a nullptr. Crash!!!" << endl;
      return nullptr;
    } else {
        fifo_evictor->touch_key(key);
    }

  }

  bool del(key_type key)
  {
    auto search = umap_.find(key);
    if (search != umap_.end()) {
        val_type val = (search -> second).val;
        size_type size = (search -> second).size;
        delete[] val;
        umap_.erase(key);
        memused_ = memused_ - size;
        return true;
      }
    return false;
  }


  size_type space_used() const{
		return memused_;
	}

  void reset() {
    while (memused_ > 0)
    {
      key_type retkey = fifo_evictor->evict();
      del(retkey);
    }
  }

};

Cache::Cache(size_type maxmem,float max_load_factor, Evictor* evictor, hash_func hasher)
 : pImpl_(new Impl(maxmem, max_load_factor, evictor, hasher)) {}

Cache::~Cache() = default;

void Cache::set(key_type key, val_type val, size_type size){
  pImpl_-> set(key, val, size);
};

Cache::val_type Cache::get(key_type key, size_type& val_size) const {
  return pImpl_ -> get(key, val_size);
};

bool Cache::del(key_type key){
  return pImpl_ -> del(key);
};

Cache::size_type Cache::space_used() const {
  return pImpl_-> space_used();
}

void Cache::reset() {
  return pImpl_-> reset();
}
