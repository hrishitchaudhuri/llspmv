#include"csr.h"
#include<vector>
#include<iostream>

#include"spmv.h"

using namespace std;

int main(int argc, char * argv[]){
    vector<vector<int>> a = {{3,0,0,0}, {1,0,4,0}, {0,0,0,5}, {5,3,0,0}};
    CSR csr(a);

    vector<int> y = {4, 5, 3, 1};

    #if 0
    vector<int> res_1 = mv_naive(a, y);

    for (auto x: res_1) {
        cout << x << "\n";
    }
    #endif

    #if 0
    vector<int> res_2 = spmv_serial(csr, y);

    for (auto x: res_2) {
        cout << x << "\n";
    }
    #endif

    #if 1
    vector<int> res_3 = spmv_parallel_1(csr, y);

    for (auto x: res_3) {
        cout << x << "\n";
    }
    #endif

    return 0;
}