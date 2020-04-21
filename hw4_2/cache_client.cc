
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <string>
#include <stdio.h>
#include <curl/curl.h>
#include "cache.hh"
#include <cassert>
using namespace std;


struct Array_t{
    char* array;
    int array_size;
};

//https://curl.haxx.se/mail/lib-2012-06/0308.html
/* the function to invoke as the data recieved */
Cache::size_type static write_callback_func(void *buffer, Cache::size_type _ ,Cache::size_type buffer_size,void *userp)
{
    // printf("\n\nIn write_callback_func()...\n");
    int bufferlen = buffer_size;
    // printf("In write_callback_func(): strlen(buffer) = [%d]\n", bufferlen);
    // printf("In write_callback_func(): buffer = [%s]\n", buffer);
    char **response_ptr = (char**)userp;
    // printf("In write_callback_func(): About to call strndup()...\n");
    /* assuming the response is a string */
    //*response_ptr = strndup(buffer, (size_t)(size *nmemb));
    //apr_cpystrn((const char *)userp, buffer, bufferlen);
    // printf("In write_callback_func(): Finished setting *response_ptr, returning...\n");

    Array_t* arr_t = (Array_t*) userp;

    if (arr_t -> array == NULL){
        arr_t -> array_size = buffer_size;
        arr_t -> array = new char[arr_t-> array_size];
        char *bufferp = (char*) buffer;
        for(int i = 0; i < buffer_size; i++){
            arr_t -> array[i] = bufferp[i];
        }
    } else{

        Array_t new_arr_t;
        new_arr_t.array_size = arr_t -> array_size + buffer_size;

        new_arr_t.array = new char[new_arr_t.array_size];

        for(int i = 0; i < arr_t -> array_size; i++){
            new_arr_t.array[i]= arr_t -> array[i];
        }
        char *bufferp = (char*) buffer;
        for(int i = 0; i < buffer_size; i++){
            new_arr_t.array[i+arr_t -> array_size]= bufferp[i];
        }

        delete[] arr_t -> array;

        *arr_t = new_arr_t;

    }


    return buffer_size;
} // end write_callback_func()

class Cache::Impl {

    const string port_;
    const string address_;

    char* check_ = NULL;
    string url_;

public:

    Impl(string port, string address) :
    port_(port), address_(address) {
        url_ = "http://"+address_+":"+port_;
    }

    ~Impl(){
    }

    void set(key_type key, val_type val, size_type size)
    {
        CURL* curl_ = curl_easy_init();
        string vals = val;
        std::cout << vals << '\n';
        string command = url_+"/keyval/"+key+"/"+vals;
        std::cout << command << '\n';
        char *commandc = (char*) command.c_str();
        if (curl_){
            CURLcode res;

            curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt(curl_,CURLOPT_URL,commandc);

            res = curl_easy_perform(curl_);

            if(res == CURLE_OK) {

            }else{
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl_);
        }
    }

  val_type get(key_type key, size_type& val_size)
  {
      CURL* curl_ = curl_easy_init();
      std::string command = url_+"/key/"+key;
      char *commandc = (char*) command.c_str();
      Array_t arr_t = {};

      if (curl_){
          CURLcode res;
          long response_code;

          curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "GET");
          curl_easy_setopt(curl_,CURLOPT_URL,commandc);

          /* setting a callback function to return the data */
          curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback_func);
          /* passing the pointer to the response as the callback parameter */
          //setting the callback user data(userp).
          curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &arr_t);
          res = curl_easy_perform(curl_);
          if(res == CURLE_OK) {
              curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
          }else{
			  fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
              curl_easy_cleanup(curl_);
              val_size = 0;
              return nullptr;
          }

          if (response_code == 404) {
              val_size =0;
              curl_easy_cleanup(curl_);
              return nullptr;
          }else {
              size_type i;
              val_type val = &arr_t.array[10];
              for (i=10; arr_t.array[i]!=',';i++){}
              val_size = i-11;
              //memory leak
              if(check_ != NULL) {
                  delete[]check_;
              }
              check_ = arr_t.array;
              curl_easy_cleanup(curl_);
              return val;
          }
      }
      val_size = 0;
      return nullptr;
  }

  bool del(key_type key)
  {
      CURL* curl_ = curl_easy_init();

      std::string command = url_+"/key/"+key;
      char *commandc = (char*) command.c_str();
	  if(curl_)
	  {
          long response_code;
          CURLcode res;
		  curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "DELETE");
		  curl_easy_setopt(curl_, CURLOPT_URL, commandc);
		  res = curl_easy_perform(curl_);
          if(res == CURLE_OK) {
              curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &response_code);
          }else{
			  fprintf(stderr, "curl_easy_perform() failed: %s\n",
			  curl_easy_strerror(res));
              curl_easy_cleanup(curl_);
              return false;
          }
          if (response_code == 200) {
              curl_easy_cleanup(curl_);
              return true;
          }else {
              curl_easy_cleanup(curl_);
              return false;
          }
      }
      return false;

  }


  size_type space_used() const{
      std::string command = url_+"/space_used";
      char *commandc = (char*) command.c_str();
      Array_t arr_t = {};

      CURL* curl_ = curl_easy_init();

      if (curl_){
          CURLcode res;

          curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "GET");

          curl_easy_setopt(curl_, CURLOPT_URL,commandc);


          /* setting a callback function to return the data */
          curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, write_callback_func);

          /* passing the pointer to the response as the callback parameter */
          //setting the callback user data(userp).
          curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &arr_t);

          res = curl_easy_perform(curl_);

          if (res != CURLE_OK){
              fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
              Cache::size_type err = 0;
              curl_easy_cleanup(curl_);
              return err;
          }
          curl_easy_cleanup(curl_);
      }

      std::string str;
      size_type space_used = 0;
      for (int i=0;i< arr_t.array_size; i++) {
          space_used = space_used * 10 + arr_t.array[i]-'0';
      }
      return space_used;
  }

  void reset() {
      std::string command = url_+"/reset";
      char *commandc = (char*) command.c_str();
      CURL* curl_ = curl_easy_init();
      if(curl_) {
          CURLcode res;
          curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST");
    	  curl_easy_setopt(curl_, CURLOPT_URL, commandc);
    	  res = curl_easy_perform(curl_);
          if (res != CURLE_OK)
          {
              fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
          }
          curl_easy_cleanup(curl_);
  	  }

  }

};

Cache::Cache(string port, string address): pImpl_(new Impl(port, address)) {}

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

// A sample text main.
// int main(){
//     string port = "11112";
//     string address = "0.0.0.0";
//     Cache c(port, address);
//
//     c.reset();
//
//     key_type key1 = "a";
//     key_type key2 = "b";
//     key_type key3 = "c";
//     key_type key4 = "d";
//     key_type key5 = "e";
//     Cache::val_type val = "z";
//     Cache::size_type size = strlen(val);
//     c.set(key1, val, size);
//     c.set(key2, val, size);
//     c.set(key3, val, size);
//     c.set(key4, val, size);
//     c.set(key5, val, size);
//
//     key_type key6 = "f";
//     c.set(key6, val, size);
//
//     assert(c.space_used() == 5);
//
//     auto evict_val = c.get(key1, size);
//     assert(evict_val == nullptr);
//     assert(size == 0);
//     c.reset();
//
//     return 0;
// }
