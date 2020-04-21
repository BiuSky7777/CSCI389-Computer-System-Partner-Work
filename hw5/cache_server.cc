#include "cache.hh"
#include "crow.h"
#include "fifo_evictor.hh"
#include <iostream>
#include <cstring>
using namespace std;
using namespace crow;

int main(int argc, char *argv[]){

    uint32_t maxmem;
    uint32_t port;

    if (argc != 1){
         maxmem = atoi(argv[1]);
         port = atoi(argv[2]);
    }else{
         maxmem = 1000;
         port = 11112;
    }

    SimpleApp app;
    // Initialize a cache
    FIFO_Evictor* e = new FIFO_Evictor();
    Cache c(maxmem,0.75,e,std::hash<key_type>());

    // Get function
    CROW_ROUTE(app, "/key/<string>").methods("GET"_method)
    ([&](key_type key){
        uint32_t size = 1;
        if (c.get(key,size) == nullptr){
            string err = "The key is not in the cache.";
            return response(404);
        } else {
            auto ptr_val = c.get(key,size);
            crow::json::wvalue jspair;
            jspair["key"] = key;
            jspair["value"] = ptr_val;
            return crow::response(200, jspair);
        }
    });

    // Put function
    CROW_ROUTE(app, "/keyval/<string>/<string>").methods("PUT"_method)
    ([&](const request& req, std::string k, std::string val) {
        if (req.method == "PUT"_method){
            uint32_t size = val.length();
            Cache::val_type val_pointer = val.c_str();
            c.set(k, val_pointer, size);
            return response(200, "Successfully inserted/updated key and value.");
        } else {
            return response(404);
        }
    });

  // Delete function
  CROW_ROUTE(app, "/key/<string>").methods("DELETE"_method)
  ([&](key_type key){
      bool exist = c.del(key);
      std::cout <<"exist: "<< exist << '\n';
      if (exist){
          return response(200, "Successfully deleted the k,v pair from the cache");
      } else {
          string err = "The key is not in the cache.";
          return response(404);
      }
  });

  // Head function
  CROW_ROUTE(app, "/head").methods("HEAD"_method)
  ([&](){
      crow::json::wvalue header;
      header["HTTP Version"] = "1.1";
      header["Accept"] = "application/json";
      header["Content-Type"] = "application/json";
      header["Space-used"] = c.space_used();

      return response(200, header);

  });

  // Space_used function
  CROW_ROUTE(app, "/space_used").methods("GET"_method)
  ([&](){
      std::string str = to_string(c.space_used());
      return response(200, str);

  });

  // Reset function
  CROW_ROUTE(app, "/reset").methods("POST"_method)
  ([&](const request& req) {
      if (req.method == "POST"_method){
          c.reset();
          return response(200, "Successfully reset the cache.");
      } else {
          return response(404);
      }
  });


  app.port(port).run();
  //app.port().multithreaded().run(); // for multithread

}
