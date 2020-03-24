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
       port = 11111;
  }

  SimpleApp app;

  FIFO_Evictor* e = new FIFO_Evictor();
  Cache c(maxmem,0.75,e,std::hash<key_type>());

  CROW_ROUTE(app, "/key/<string>").methods("GET"_method)
  ([&](key_type key){
      uint32_t size = 1;
      if (c.get(key,size) == nullptr){
        string err = "The key is not in the cache.";
        return crow::response(500,err);
      } else {
        auto ptr_val = c.get(key,size);
        auto val_val = *ptr_val;

        crow::json::wvalue jspair;
        jspair["key"] = key;
        jspair["value"] = val_val;
        return crow::response(200, jspair);
      }
  });
    
    
    CROW_ROUTE(app, "/key/<string>/<string>")
           .methods("PUT"_method)
           ([&](const request& req, std::string k, std::string val) {


          if (req.method == "PUT"_method)
         {

             uint32_t size = sizeof(val);
             Cache::val_type val_pointer = val.c_str();
             c.set(k, val_pointer, size);
           }
          return response(200, "Successfully inserted/updated key and value.");
       });
/*

  CROW_ROUTE(app, "/keyval/<string>/<string>").methods("PUT"_method)
  ([&](key_type key, Cache::val_type val){
      Cache::size_type size = sizeof(val); //strlen(val)+1
      c.set(key, val, size);

      return response(200, "Successfully created/replaced a k,v pair in the cache");
  });
*/
  CROW_ROUTE(app, "/key/<string>").methods("DELETE"_method)
  ([&](key_type key){
    c.del(key);
    return response(200, "Successfully deleted the k,v pair from the cache");
  });

  CROW_ROUTE(app, "/head").methods("HEAD"_method)
  ([&](){
    crow::json::wvalue header;
    header["HTTP Version"] = "1.1";
    header["Accept"] = "application/json";
    header["Content-Type"] = "application/json";
    header["Space-used"] = c.space_used();

    return response(200, header);

  });

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
