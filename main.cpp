#include<vector>
#include<iostream>

#include<time.h>

#include"csr.h"
#include"spmv.h"
#include "lookup.h"
#include "four_russians.cpp"

#define NUM_ELEMENTS 1000
#define SPARSITY_COEF 7

using namespace std;
typedef std::vector<std::vector<int>> matrix;
typedef std::vector<int> vtr;

matrix generateSparseMatrix(int n, int sparsity) {
    matrix m_1;
    vtr m_2;

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {    
            if (rand() % sparsity == 0) {
                m_2.push_back(1);
            }

            else {
                m_2.push_back(0);
            }
        }

        m_1.push_back(m_2);
        m_2.clear();
    }

    return m_1;
}

vtr generateSparseVector(int n, int sparsity) {
    vtr v_1;

    for (int i = 0; i < n; i++) {
        if (rand() % sparsity == 0) {
            v_1.push_back(1);
        }

        else {
            v_1.push_back(0);
        }
    }

    return v_1;
}

int main(int argc, char * argv[]){
    matrix a;
    a = generateSparseMatrix(NUM_ELEMENTS, SPARSITY_COEF);
    matrix b = generateSparseMatrix(NUM_ELEMENTS, SPARSITY_COEF);

    //CSR csr(a);

    vtr y;
    y = generateSparseVector(NUM_ELEMENTS, SPARSITY_COEF);

    clock_t start, end;

    #if 0
    vtr res_1 = mv_naive(a, y);

    /*
    for (auto x: res_1) {
        cout << x << "\n";
    }
    */
    #endif

    #if 0
    vtr res_2 = spmv_serial(csr, y);

    /*
    for (auto x: res_2) {
        cout << x << "\n";
    }
    */
    #endif

    #if 0
    vtr res_3 = spmv_parallel_1(csr, y);

    /*
    for (auto x: res_3) {
        cout << x << "\n";
    }
    */
    #endif

    #if 0
    start = clock();
    matrix m_0 = computeLUT_1(a);
    end = clock();

    cout << "LUT 1: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";
    #endif

    #if 0
    start = clock();
    matrix m_1 = computeLUT_2(csr);
    end = clock();

    cout << "LUT 2: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";
    #endif

    #if 1
    //matrix m_2 = computeLUT_3(csr);
    cout << "Serial Output:\n";
    vector<vector<int>> out = four_russians_serial(a,b);
    #if 0
    for(auto i: out){
        for(auto j: i){
            cout << j << " ";
        }
        cout << "\n";
    }
    #endif
    cout << "Parallel Output:\n";
    out = four_russians_parallel(a,b);
    #if 0
    for(auto i: out){
        for(auto j: i){
            cout << j << " ";
        }
        cout << "\n";
    }
    #endif

    #endif

    return 0;
}