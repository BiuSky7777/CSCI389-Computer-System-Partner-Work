#include <math.h>
#include <algorithm>
#include "cache.hh"
#include "evictor.hh"
#include <stdlib.h>
#include <random>
#include <iostream>
#include <string>
#include <chrono>
#include <numeric>
#include <tuple>
#include <vector>

#include <fstream>

using namespace std;

//#include <opencv2\opencv.hpp>
#include <cmath>
//using namespace cv;


double mu_k = 30.7984;
double sigma_k = 8.20449;
double k_k = 0.078688;

double mu_v = 50;
double sigma_v = 4.0;

double get_rate = 1/31.;
double set_rate = (1-get_rate) * (6/10.);
double del_rate = (1-get_rate) * (4/10.);

std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0,1);
std::normal_distribution<double> distribution_v(mu_v,sigma_v);

// Extreme Value Disctibution
// https://stackoverflow.com/questions/28744065/generalized-extreme-value-distribution-in-c-gev
double icdf(double mu, double sigma, double xi){
        double x = distribution(generator);

        double a = pow(-1*log(x),-1*xi);
        double b = -1*xi*mu*pow(-log(x),xi);
        double c = sigma * pow(-log(x),xi) - sigma;

        return (-1)*(a*(b+c) )/xi;
}

Cache::size_type generate_random_key_size(){
     auto randnum = int(icdf(mu_k, sigma_k, k_k));
     return static_cast<Cache::size_type>(randnum);
 }

//Generate a random string
//http://www.cplusplus.com/forum/windows/88843/
string gen_random(Cache::size_type size)
 {
     static const char alphanum[] =
     "0123456789"
     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
     "abcdefghijklmnopqrstuvwxyz";

     int stringLength = sizeof(alphanum) - 1;

     string str;
     for(unsigned int i = 0; i < size; ++i){
         str += alphanum[rand() % stringLength];
     }
     return str;
 }


key_type generate_random_key(Cache::size_type key_size){
    auto s = gen_random(key_size);
    return static_cast<key_type>(s);
}

Cache::size_type generate_random_val_size(){
    auto randnum = int(distribution_v(generator));
    return static_cast<Cache::size_type>(randnum);
}

Cache::val_type generate_random_val(Cache::size_type val_size){
    auto s = gen_random(val_size);
    char* s_ptr = new char[val_size + 1];
    for(int i = 0; i < val_size; i++){
        s_ptr[i] = s[i];
    }
    s_ptr[val_size] = '\0';
    std::cout << "generate random value: "<<s_ptr << '\n';
    return s_ptr;
}

std::string generate_random_request(){
    std::uniform_int_distribution<int> distribution(0,100);

    auto randnum = distribution(generator);

    if (randnum <= round(100*get_rate)){
        return "PUT";
    } else if (randnum <= round(100*(set_rate+get_rate))){
        return "GET";
    } else {
        return "DELETE";
    }
}

double hit_rate(Cache* c, int iter, vector<key_type> key_storage){
    int count = 0;
    int hitnum = 0;

    for(int i = 0; i<iter; i++){
        auto request = generate_random_request();
        std::cout << request << '\n';
        if (request.compare("PUT") == 0){
            auto key_size = generate_random_key_size();
            auto key = generate_random_key(key_size);
            auto val_size = generate_random_key_size();
            auto val = generate_random_val(val_size);
            c->set(key, val, val_size);
            key_storage.push_back(key);

        }else if(request.compare("GET") == 0){
            count ++;
            Cache::size_type val_size = 0;
            std::uniform_int_distribution<int> distribution_key(0,key_storage.size()-1);
            auto key = key_storage[distribution_key(generator)];
            Cache::val_type val = c->get(key, val_size);
            if (val != nullptr) {
                hitnum++;
            }
        }else{
            std::uniform_int_distribution<int> distribution_key(0,key_storage.size()-1);
            auto i = distribution_key(generator);
            auto key = key_storage[i];
            c->del(key);
            key_storage[i] = key_storage[key_storage.size()-1];
            key_storage.pop_back();

        }
    }
    auto hit_rate = double(hitnum)/double(count);

    return hit_rate;
}


vector<double> baseline_latencies(Cache* c, int nreq, vector<key_type> key_storage){
    vector<double> result;
    int count = 0;
    int hitnum = 0;

    for(int i = 0; i < nreq; i++){
        auto request = generate_random_request();
        if (request.compare("PUT") == 0){
            auto start1 = chrono::high_resolution_clock::now();

            auto key_size = generate_random_key_size();
            auto key = generate_random_key(key_size);
            auto val_size = generate_random_key_size();
            auto val = generate_random_val(val_size);
            c->set(key, val, val_size);

            auto stop1 = chrono::high_resolution_clock::now();
            auto t1 = chrono::duration_cast<chrono::nanoseconds>(stop1 - start1).count();
            result.push_back(t1/1000000.0);

        }else if(request.compare("GET") == 0){

            auto start2 = chrono::high_resolution_clock::now();

            count ++;
            Cache::size_type val_size = 0;
            std::uniform_int_distribution<int> distribution_key(0,key_storage.size()-1);
            auto key = key_storage[distribution_key(generator)];
            Cache::val_type val = c->get(key, val_size);

            auto stop2 = chrono::high_resolution_clock::now();
            auto t2 = chrono::duration_cast<chrono::nanoseconds>(stop2 - start2).count();
            result.push_back(t2/1000000.0);
            if (val != nullptr) {
                hitnum++;
            }

        }else{
            auto start3 = chrono::high_resolution_clock::now();

            std::uniform_int_distribution<int> distribution_key(0,key_storage.size()-1);
            auto i = distribution_key(generator);
            auto key = key_storage[i];
            c->del(key);
            key_storage[i] = key_storage[key_storage.size()-1];
            key_storage.pop_back();

            auto stop3 = chrono::high_resolution_clock::now();
            auto t3 = chrono::duration_cast<chrono::nanoseconds>(stop3 - start3).count();
            result.push_back(t3/1000000.0);
        }
    }

    return result;
}

double baseline_performance(vector<double> v, double* mean_throughput){
    sort(v.begin(), v.end());
    auto latency = v[int(0.95*v.size())];

    *mean_throughput = accumulate(v.begin(), v.end(), 0.0)/v.size();

    return latency;
}



int main(){
    string port = "11112";
    string address = "0.0.0.0";
    Cache c(port, address);
    Cache* cptr = &c;
    int hitnum = 0;
    int count = 0;

    int nreq = 500;
    vector<key_type> key_set_vec;

    //Warm up
    int initial_size = 10000;
    for(int i = 0; i < initial_size; i++){
        std::cout << "set" << '\n';
        auto key_size = generate_random_key_size();
        std::cout << "key_size: "<<key_size << '\n';
        auto key = generate_random_key(key_size);
        std::cout << "key: "<<key << '\n';
        auto val_size = generate_random_key_size();
        std::cout << "val_size: "<<val_size << '\n';
        auto val = generate_random_val(val_size);
        std::cout << "val: " << val << '\n';
        c.set(key, val, val_size);
        key_set_vec.push_back(key);
    }


    auto latency_vec = baseline_latencies(cptr, nreq,key_set_vec);
    double mean_throughput = 0.0;
    auto performance = baseline_performance(latency_vec, &mean_throughput);
    cout<<"95 peercentile: "<<performance<<endl;
    cout<<"meean: "<< mean_throughput <<endl;

    // Write the result in an output file.
    ofstream myfile;
    myfile.open ("baseline_latencies.txt");
    myfile<<"baseline_latencies = {";

    for (int i = 0; i < latency_vec.size(); i++) {
        myfile << latency_vec[i] <<",";

    }
    myfile<<"};";

    myfile.close();


    return 1;
}
