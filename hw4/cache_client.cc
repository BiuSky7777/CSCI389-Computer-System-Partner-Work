
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include "cache.hh"
#include "fifo_evictor.hh"
using namespace std;

const char* url_get_key = "http://0.0.0.0:11112/key/";
const char* url_put_key_val = "http://0.0.0.0:11112/keyval/";
const char* url_del_key = "http://0.0.0.0:11112/key/";
const char* url_head = "http://0.0.0.0:11112/head";
const char* url_post = "http://0.0.0.0:11112/reset";

struct Array_t{
  char* array;
  int array_size;
};
//https://curl.haxx.se/mail/lib-2012-06/0308.html
/* the function to invoke as the data recieved */
Cache::size_type static write_callback_func(void *buffer, Cache::size_type _ ,Cache::size_type buffer_size,void *userp)
{
    printf("\n\nIn write_callback_func()...\n");
    int bufferlen = sizeof(buffer);
    printf("In write_callback_func(): strlen(buffer) = [%d]\n", bufferlen);
    printf("In write_callback_func(): buffer = [%s]\n", buffer);
    char **response_ptr = (char**)userp;
    printf("In write_callback_func(): About to call strndup()...\n");
    /* assuming the response is a string */
    //*response_ptr = strndup(buffer, (size_t)(size *nmemb));
    //apr_cpystrn((const char *)userp, buffer, bufferlen);
    printf("In write_callback_func(): Finished setting *response_ptr, returning...\n");

    Array_t* arr_t = (Array_t*) userp;
    arr_t -> array_size = buffer_size;
    arr_t -> array = new char[arr_t-> array_size];

    for(int i = 0; i < buffer_size; i++){
        arr_t -> array[i] = buffer[i];
    }
    return buffer_size;
} // end write_callback_func()

class Cache::Impl {

  const string port_;
  const string address_;

  CURL* curl_;

  string url;
public:

  Impl(string port, string address) :
   port_(port), address_(address), curl_(curl_easy_init()) {
      url = "http://"+address_+":"+port_;
   }

  ~Impl(){}

  void set(key_type key, val_type val, size_type size)
  {
    std::string command = url+"/keyval/"+key+'/'+val;

    if (curl_){
        CURLcode res;
        curl_easy_setopt(curl_,CURLOPT_URL,command);
        res = curl_easy_perform(curl_);

        if (res != CURLE_OK){
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl_);
    }
  }

  val_type get(key_type key, size_type& val_size)
  {
      std::string command = url+"/key/"+key;

      Array_t* arr_t;

      if (curl_){
          CURLcode res;
          curl_easy_setopt(curl_,CURLOPT_URL,command);

          /* setting a callback function to return the data */
          curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback_func);
          /* passing the pointer to the response as the callback parameter */
          //setting the callback user data(userp).
          curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &arr_t);
          res = curl_easy_perform(curl_);


          if (res != CURLE_OK){
			      fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
          }

          curl_easy_cleanup(curl_);
      }

      return arr_t->array;
  }

  bool del(key_type key)
  {
    std::string command = url+"/key/"+key;

		if(curl_)
		{

            long response_code;
      CURLcode res;
			curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "DELETE");
			curl_easy_setopt(curl_, CURLOPT_URL, command);

		  res = curl_easy_perform(curl_);

          if(res == CURLE_OK) {

            curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
        }else{
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
            }

			curl_easy_cleanup(curl_);
            if (response_code == 200) {
                return true;
            }else {
                return false;
            }
		}

		return false;
  }


  size_type space_used() const{
    std::string command = url+"/head";
    Array_t* arr_t; //?????

    if(curl_) {
        CURLcode res;
        curl_easy_setopt(curl_,CURLOPT_URL,command);

        curl_easy_setopt(curl_, CURLOPT_HEADER, 1L);

        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback_func);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &arr_t);

        res = curl_easy_perform(curl_);

        if (res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));

        curl_easy_cleanup(curl_);
    }
    return arr_t -> array;
  }

  void reset() {
    std::string command = url+"/reset";

    if(curl_) {

        CURLcode res;

    		curl_easy_setopt(curl_, CURLOPT_URL, command);

    		curl_easy_setopt(curl_, CURLOPT_POST, 1L);

    		res = curl_easy_perform(curl_);

        if (res != CURLE_OK)
          fprintf(stderr, "curl_easy_perform() failed: %s\n",
          curl_easy_strerror(res));

        curl_easy_cleanup(curl_);
  	}
  }

};

Cache::Cache(string port, string address)
 : pImpl_(new Impl(port, address)) {}

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
