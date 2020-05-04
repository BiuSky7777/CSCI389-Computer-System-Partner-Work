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
#include <thread>
#include <pthread.h>

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
            std::cout << "PUT" << '\n';
            auto start1 = chrono::high_resolution_clock::now();

            auto key_size = generate_random_key_size();
            auto key = generate_random_key(key_size);
            auto val_size = generate_random_key_size();
            auto val = generate_random_val(val_size);
            c->set(key, val, val_size);
            key_storage.push_back(key);

            auto stop1 = chrono::high_resolution_clock::now();
            auto t1 = chrono::duration_cast<chrono::nanoseconds>(stop1 - start1).count();
            result.push_back(t1/1000000.0);

        }else if(request.compare("GET") == 0){
            std::cout << "GET" << '\n';

            auto start2 = chrono::high_resolution_clock::now();

            count ++;
            Cache::size_type val_size = 0;
            std::uniform_int_distribution<int> distribution_key(0,key_storage.size()-1);

            std::cout << distribution_key(generator) << '\n';
            std::cout << key_storage.size() << '\n';

            auto key = key_storage[distribution_key(generator)];

            Cache::val_type val = c->get(key, val_size);

            auto stop2 = chrono::high_resolution_clock::now();
            auto t2 = chrono::duration_cast<chrono::nanoseconds>(stop2 - start2).count();
            result.push_back(t2/1000000.0);
            if (val != nullptr) {
                hitnum++;
            }

        }else{
            std::cout << "DELETE" << '\n';

            if (key_storage.size()>0){

                auto start3 = chrono::high_resolution_clock::now();

                std::uniform_int_distribution<int> distribution_key(0,key_storage.size()-1);
                auto i = distribution_key(generator);

                std::cout << key_storage.size() << '\n';

                auto key = key_storage[i];

                c->del(key);
                key_storage[i] = key_storage[key_storage.size()-1];
                key_storage.pop_back();

                auto stop3 = chrono::high_resolution_clock::now();
                auto t3 = chrono::duration_cast<chrono::nanoseconds>(stop3 - start3).count();
                result.push_back(t3/1000000.0);

            }

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

void warm_up(Cache* c, int initial_size, vector<key_type> *key_set_vec){
    for(int i = 0; i < initial_size; i++){
        // std::cout << "set" << '\n';
        auto key_size = generate_random_key_size();
        // std::cout << "key_size: "<<key_size << '\n';
        auto key = generate_random_key(key_size);
        // std::cout << "key: "<<key << '\n';
        auto val_size = generate_random_key_size();
        // std::cout << "val_size: "<<val_size << '\n';
        auto val = generate_random_val(val_size);
        // std::cout << "val: " << val << '\n';
        c->set(key, val, val_size);
        key_set_vec->push_back(key);
    }
}

struct thread_data {
    Cache* cptr;
    int nreq;
    vector<key_type> key_storage;
    vector<double> result_latency;
    pthread_mutex_t count_mutex;
};

void *runThread(void *threadarg) {
    cout << "Message :  thread start running"<< endl;


    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    auto latency_vec = baseline_latencies(my_data -> cptr, my_data -> nreq, my_data -> key_storage);
    my_data->result_latency = latency_vec;

    cout << "Message :  thread end running"<< endl;

    pthread_mutex_unlock( &my_data->count_mutex);

    pthread_exit(NULL);

}


vector<double> threaded_performance (int initial_size, int nreq, int nthreads){
    string port = "11112";
    string address = "0.0.0.0";

    pthread_t threads[nthreads];
    struct thread_data td[nthreads];
    int rc;

    vector<double> all_thread_latency_vec;

    for(int i = 0; i < nthreads; i++ ) {

        cout <<"main() : creating thread, " << i << endl;

        Cache* cptr  = new Cache(port, address);

        vector<key_type> key_set_vec;
        warm_up(cptr, initial_size, &key_set_vec);

        td[i].cptr = cptr;
        td[i].nreq = nreq;
        td[i].key_storage = key_set_vec;
        td[i].count_mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock( &td[i].count_mutex );

        rc = pthread_create(&threads[i], NULL, runThread, (void *)&td[i]);
    }


    for(int i = 0; i < nthreads; i++ ) {

        pthread_mutex_lock( &td[i].count_mutex );

        auto single_thread_latency = td[i].result_latency;
        for(int j = 0; j < single_thread_latency.size(); j++){
            all_thread_latency_vec.push_back(single_thread_latency[j]);
        }

        if (rc) {
           cout << "Error:unable to create thread," << rc << endl;
           exit(-1);
        }

    }

    return all_thread_latency_vec;
}

void write_file(vector<double> all_thread_latency_vec){
    ofstream myfile;
    myfile.open ("baseline_latencies.txt");
    myfile<<"baseline_latencies = {";

    for (int i = 0; i < all_thread_latency_vec.size(); i++) {
        myfile << all_thread_latency_vec[i] <<",";

    }
    myfile<<"};";

    myfile.close();
}

int main(){

    int nreq = 1000;
    int initial_size = 500;
    int nthread = 20;

    ofstream ninefivefile;
    ninefivefile.open ("95_percentile_latencies2.txt");
    ofstream meanfile;
    meanfile.open ("mean_throughput_latencies2.txt");

    ninefivefile<<"ninefive_latencies = {";
    meanfile<<"mean_latencies = {";

    for (int i =1; i<= nthread; i++){

        vector<double> all_thread_latency_vec = threaded_performance(initial_size, nreq, i);


        double mean_throughput = 0.0;
        auto performance = baseline_performance(all_thread_latency_vec, &mean_throughput);
        cout<<"95 percentile: "<<performance<<endl;
        cout<<"mean: "<< mean_throughput <<endl;

        ninefivefile << performance;
        meanfile << mean_throughput;

        if(i != nthread){
            ninefivefile <<",";
            meanfile <<",";
        }

    }

    ninefivefile<<"};";
    ninefivefile.close();
    meanfile<<"};";
    meanfile.close();

    return 1;
}
