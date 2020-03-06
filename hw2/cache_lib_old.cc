
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include "cache.hh"
using namespace std;

class Cache::Impl {
  size_type maxmem_;
  size_type memused_;

  struct Element {
    key_type key;
    val_type val;
    size_type size;
  };
  vector<Element> vect;

public:
  Impl(size_type maxmem) : maxmem_(maxmem), memused_(0) {}

  void set(key_type key, val_type val, size_type size)
  {
    if(size <= maxmem_){
        auto iter = find_if(vect.begin(), vect.end(),
            [key](const Element& elem){ return elem.key == key; });
        if (iter == vect.end()) {
            while ((memused_ + size)> maxmem_)
            {
              memused_ -= vect.back().size;
              vect.pop_back();
            }
            vect.push_back( {key, val, size} );
            memused_ += size;
        }else{
            (*iter).val = val;
        }
    } else {
      cout << "Cache is not big enough to store this key-val pair.";
    }
  }

  val_type get(key_type key, size_type& val_size)
  {
    auto iter = find_if(vect.begin(), vect.end(),
        [key](const Element& elem){ return elem.key == key; });
    if (iter != vect.end()) {
        retval = *iter.val;
        val_size = *iter.size;
        return retval;
      } else {
        val_size = 0;
        return nullptr;
      }
  }

  bool del(key_type key)
  {
    auto iter = std::find_if(vect.begin(), vect.end(),
          [key](const Element& elem){ return elem.key == key; });
    if (iter != vect.end()) {
        vect.erase(iter);
        memused_ -= *iter.size;
        return true;
      }
    return false;
  }

  size_type space_used() const{
		return memused_;
	}


};

Cache::Cache(size_type maxmem,float max_load_factor, Evictor* evictor, hash_func hasher)
 : pImpl_(new Impl(maxmem)) {}

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
