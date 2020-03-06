#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "cache.hh"
#include "fifo_evictor.hh"

using namespace std;

int test_set_and_space_used()
{
	cout << "TEST: Create cache and check for set and space used..." << '\n';
	FIFO_Evictor* e = new FIFO_Evictor();
	Cache c(1000,0.75,e,std::hash<key_type>());

	// check insert
  key_type key = "a";
	Cache::val_type val = "z";
	Cache::size_type size = strlen(val)+1;
	c.set(key, val, size);
	key_type key2 = "b";
	Cache::val_type val2 = "xy";
	Cache::size_type size2 = strlen(val2)+1;
	c.set(key2, val2, size2);
	key_type key3 = "c";
	Cache::val_type val3 = "xyz";
	Cache::size_type size3 = strlen(val3)+1;
	c.set(key3, val3, size3);
	assert(c.space_used() == size+size2+size3);

	// check overwrite
	key_type key4 = "c";
	Cache::val_type val4 = "abcd";
	Cache::size_type size4 = strlen(val4)+1;
	c.set(key4, val4, size4);
	assert(c.space_used() == size+size2+size4);

	// unsigned n = c.umap_.bucket_count();
	//
	// std::cout << "my map has " << n << " buckets.\n";
	//
	// for (unsigned i=0; i<n; ++i) {
	// 	std::cout << "bucket #" << i << " contains: ";
	// 	for (auto it = c.umap_.begin(i); it!=c.umap_.end(i); ++it)
	// 		std::cout << "[" << it->first << ":" << it->second << "] ";
	// 	std::cout << "\n";
	// }

	cout << "Set Test Passed." << "\n";
	cout << '\n';
	return 0;
}


int test_get()
{
	cout << "TEST: Create cache and check for get..." << '\n';
	FIFO_Evictor* e = new FIFO_Evictor();
	Cache c(1000,0.75,e,std::hash<key_type>());

  key_type key = "a";
	Cache::val_type val = "z";
	Cache::size_type size = strlen(val)+1;
	c.set(key, val, size);
	key_type key2 = "b";
	Cache::val_type val2 = "xy";
	Cache::size_type size2 = strlen(val2)+1;
	c.set(key2, val2, size2);

	// check get for existed key
	// check both returned value and size
	key_type get_val = c.get(key, size2);
	assert(get_val == "z");
	assert(size2 == size);

	// check get for nonexisted key
	// check both returned value and size
	key_type fake_key = "o";
	auto get_fake_val = c.get(fake_key, size);
	assert(get_fake_val == nullptr);
	assert(size == 0);

	cout << "Get Test Passed." << "\n";
	cout << '\n';
	return 0;
}



int test_del()
{
	cout << "TEST: Create cache and check for del..." << '\n';
	FIFO_Evictor* e = new FIFO_Evictor();
	Cache c(1000,0.75,e,std::hash<key_type>());

	key_type key = "a";
	Cache::val_type val = "z";
	Cache::size_type size = strlen(val)+1;
	c.set(key, val, size);
	c.get(key, size);

	key_type key2 = "b";
	Cache::val_type val2 = "xy";
	Cache::size_type size2 = strlen(val2)+1;
	c.set(key2, val2, size2);

	// check del for existed key
	bool del_flag = c.del(key);
	assert(del_flag == true);
	assert(c.space_used() == size2);

	// check get for nonexisted key
	bool del_flag2 = c.del(key);
	assert(del_flag2 == false);
	assert(c.space_used() == size2);

	// check get for left space after deletion
	bool del_flag3 = c.del(key2);
	assert(del_flag3 == true);
	assert(c.space_used() == 0);

	cout << "Del Test Passed." << "\n";
	cout << '\n';
	return 0;
}

int test_space_used()
{
	cout << "TEST: Create cache and check for space used..." << '\n';
	FIFO_Evictor* e = new FIFO_Evictor();
	Cache c(1000,0.75,e,std::hash<key_type>());

  key_type key = "a";
	Cache::val_type val = "z";
	Cache::size_type size = strlen(val)+1;
	c.set(key, val, size);

	key_type key2 = "b";
	Cache::val_type val2 = "xy";
	Cache::size_type size2 = strlen(val2)+1;
	c.set(key2, val2, size2);

	assert(c.space_used() == size+size2);

	c.get(key, size);
	assert(c.space_used() == size+size2);

	c.del(key2);
	assert(c.space_used() == size);

	c.del(key);
	assert(c.space_used() == 0);
	cout << "Space-used Test Passed." << "\n";
	cout << '\n';
	return 0;
}

int test_evict()
{
	cout << "TEST: Create cache and check for evict..." << '\n';
	FIFO_Evictor* e = new FIFO_Evictor();
	Cache c(10,0.75,e,std::hash<key_type>());

	key_type key1 = "a";
	key_type key2 = "b";
	key_type key3 = "c";
	key_type key4 = "d";
	key_type key5 = "e";
	Cache::val_type val = "z";
	Cache::size_type size = strlen(val)+1;
	c.set(key1, val, size);
	c.set(key2, val, size);
	c.set(key3, val, size);
	c.set(key4, val, size);
	c.set(key5, val, size);

	// check correctness when only one key gets evicted
	// order being evicted also checked here
	key_type key6 = "f";
	c.set(key6, val, size);
	Cache::val_type key6val = c.get(key6,size);
	assert(key6val == "z");
	assert(c.space_used() == 10);

	auto evict_val = c.get(key1, size);
	assert(evict_val == nullptr);
	assert(size == 0);

	// check correctness when more than one key needs to be evicted
	// order being evicted also checked here
	key_type key7 = "g";
	Cache::val_type val7 = "yz";
	Cache::size_type size7 = strlen(val7)+1;
	c.set(key7, val7, size7);
	assert(c.space_used() == 9);

	auto evict_val2 = c.get(key2, size7);
	assert(evict_val2 == nullptr);
	assert(size7 == 0);
	size7 = 10;
	auto evict_val3 = c.get(key3, size7);
	assert(evict_val3 == nullptr);
	assert(size7 == 0);

	cout << "Eviction Policy Test Passed." << "\n";
	cout << '\n';
	return 0;

}

int test_reset()
{
	cout << "TEST: Create cache and check for reset..." << '\n';
	FIFO_Evictor* e = new FIFO_Evictor();
	Cache c(1000,0.75,e,std::hash<key_type>());

	key_type key1 = "a";
	key_type key2 = "b";
	key_type key3 = "c";
	key_type key4 = "d";
	key_type key5 = "e";
	Cache::val_type val = "z";
	Cache::size_type size = strlen(val)+1;
	c.set(key1, val, size);
	c.set(key2, val, size);
	c.set(key3, val, size);
	c.set(key4, val, size);
	c.set(key5, val, size);

	assert(c.space_used() != 0);

	c.reset();
	assert(c.space_used() == 0);

	cout << "Reset Test Passed." << "\n";
	cout << '\n';
	return 0;
}

int main(){
	test_set_and_space_used();
	test_get();
	test_del();
	test_space_used();
	test_evict();
	test_reset();
	cout << "All Tests Passed." << "\n";
	return 0;
}
