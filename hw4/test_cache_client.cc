#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "cache.hh"
#include "fifo_evictor.hh"
#include "cache_client.cc"
#include "cache_server.cc"
using namespace std;

// Initialize cache and key/value variables/pointers
sting port = "11112";
string address = "0.0.0.0";
Cache c(port, address);


TEST_CASE("Set/Get")
{
    SECTION("The value of the existed key is returned correctly.")
    {
        c.reset();
        key_type key = "a";
      	Cache::val_type val = "z";
      	Cache::size_type size = strlen(val)+1;
      	c.set(key, val, size);
      	key_type key2 = "b";
      	Cache::val_type val2 = "xy";
      	Cache::size_type size2 = strlen(val2)+1;
      	c.set(key2, val2, size2);

        Cache::val_type get_val = c.get(key, size2);
        REQUIRE(strcmp(get_val,"z") == 0);
        c.reset();
    }

    SECTION("The size of the existed key's value is returned correctly.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);
        key_type key2 = "b";
        Cache::val_type val2 = "xy";
        Cache::size_type size2 = strlen(val2)+1;
        c.set(key2, val2, size2);

        Cache::val_type get_val = c.get(key, size2);
        REQUIRE(size2 == size);
        c.reset();
    }

    SECTION("The value of the nonexisted key is returned correctly.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);
        key_type key2 = "b";
        Cache::val_type val2 = "xy";
        Cache::size_type size2 = strlen(val2)+1;
        c.set(key2, val2, size2);

        key_type fake_key = "o";
        auto get_fake_val = c.get(fake_key, size);
        REQUIRE(get_fake_val == nullptr);
        c.reset();
    }

    SECTION("The size of the nonexisted key's value is returned correctly.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);
        key_type key2 = "b";
        Cache::val_type val2 = "xy";
        Cache::size_type size2 = strlen(val2)+1;
        c.set(key2, val2, size2);

        key_type fake_key = "o";
        auto get_fake_val = c.get(fake_key, size);
        REQUIRE(size == 0);
        c.reset();
    }
}

TEST_CASE("Del")
{
    SECTION("The selected key is deleted from the cache successfully.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);

        bool del_flag = c.del(key);
        REQUIRE(del_flag == true);
        REQUIRE(c.space_used() == 0);
        c.reset();
    }

    SECTION("The selected key cannot be got after the deletion.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);

        bool del_flag = c.del(key);
        auto get_deleted_val = c.get(key, size);
        REQUIRE(get_deleted_val == nullptr);
        REQUIRE(size == 0);
        c.reset();
    }

    SECTION("The value of the nonexisted key cannot be deleted.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);

        key_type key2 = "b";
        bool del_flag = c.del(key2);
        REQUIRE(del_flag == false);
        REQUIRE(c.space_used() == size);
        c.reset();
    }

}

TEST_CASE("Space_used")
{
    SECTION("Ensure that initial Cache has 0 space used.")
    {
        c.reset();
        REQUIRE(c.space_used() == 0);
        c.reset();
    }

    SECTION("Ensure that Cache has coreect space used after setting pairs.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);

        key_type key2 = "b";
        Cache::val_type val2 = "xy";
        Cache::size_type size2 = strlen(val2)+1;
        c.set(key2, val2, size2);

        REQUIRE(c.space_used() == size+size2);
        c.reset();
    }

    SECTION("Ensure that Cache has coreect space used after deleting pairs.")
    {
        c.reset();
        key_type key = "a";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c.set(key, val, size);

        key_type key2 = "b";
        Cache::val_type val2 = "xy";
        Cache::size_type size2 = strlen(val2)+1;
        c.set(key2, val2, size2);

        c.del(key2);
        REQUIRE(c.space_used() == size);

        c.del(key);
        REQUIRE(c.space_used() == 0);
        c.reset();
    }

}


TEST_CASE("Evict")
{
    SECTION("Check when evictor is a nullptr")
    {

        c1.reset();
        key_type key1 = "a";
        key_type key2 = "b";
        key_type key3 = "c";
        key_type key4 = "d";
        Cache::val_type val = "z";
        Cache::size_type size = strlen(val)+1;
        c1.set(key1, val, size);
        c1.set(key2, val, size);
        c1.set(key3, val, size);
        c1.set(key4, val, size);

        key_type key6 = "f";
        Cache::val_type val6 = "yz";
        Cache::size_type size6 = strlen(val6)+1;
        c1.set(key6, val6, size6);
        REQUIRE(c1.space_used() == 8);
        auto cannotadd = c1.get(key6, size6);
        REQUIRE(cannotadd == nullptr);
        REQUIRE(size6 == 0);
        c1.reset();
    }

    SECTION("Ensure that when only one key gets evicted, Cache has correct used space and the order of the eviction is correct.")
    {
        c.reset();
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

        key_type key6 = "f";
        c.set(key6, val, size);
        REQUIRE(c.space_used() == 10);
        auto evict_val = c.get(key1, size);
        REQUIRE(evict_val == nullptr);
        REQUIRE(size == 0);
        c.reset();
    }

    SECTION("Ensure that Cache evicts enough space for newly added key when multiple keys need to be evicted.")
    {
        c.reset();
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

        key_type key7 = "g";
      	Cache::val_type val7 = "yz";
      	Cache::size_type size7 = strlen(val7)+1;
      	c.set(key7, val7, size7);
      	REQUIRE(c.space_used() == 9);

      	auto evict_val1 = c.get(key1, size7);
      	REQUIRE(evict_val1 == nullptr);
      	REQUIRE(size7 == 0);
      	size7 = 10; //redefine size7 to check
      	auto evict_val2 = c.get(key2, size7);
      	REQUIRE(evict_val2 == nullptr);
      	REQUIRE(size7 == 0);
        c.reset();
    }

}


TEST_CASE("Reset")
{

    SECTION("Ensure that reset cleans the Cache and gives back 0 used space.")
    {
        c.reset();
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

      	REQUIRE(c.space_used() != 0);

      	c.reset();
      	REQUIRE(c.space_used() == 0);

        c.reset();
    }

    SECTION("Ensure that after reset cleans the Cache and no key can be retrieved.")
    {
        c.reset();
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

        c.reset();
        auto get_val = c.get(key1, size);
        REQUIRE(get_val == nullptr);
        REQUIRE(size == 0);

        c.reset();

    }

}
