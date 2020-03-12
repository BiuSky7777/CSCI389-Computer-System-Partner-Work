#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include "cache.hh"
#include "fifo_evictor.hh"

using namespace std;

// Initialize cache and key/value variables/pointers
FIFO_Evictor* e = new FIFO_Evictor();
Cache c(10,0.75,e,std::hash<key_type>());

TEST_CASE("Set/Get")
{
    SECTION("The value of the existed key is returned correctly.")
    {
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

    SECTION("The value of the deleted key is returned correctly.")
    {
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
        REQUIRE(c.space_used() == 0);
        c.reset();
    }

    SECTION("Ensure that Cache has coreect space used after setting pairs.")
    {
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

    SECTION("Ensure that when only one key gets evicted, Cache has correct used space and the order of the eviction is correct.")
    {
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

    SECTION("Ensure that when two keys get evicted, Cache has correct used space and the order of the eviction is correct.")
    {
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

    SECTION("Ensure that reset cleans the Cache and gives back correct space used.")
    {
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

    SECTION("Ensure that reset cleans the Cache and gives back correct space used.")
    {
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
