/*
 * trying to implement the Four Russians method for Boolean Matrix Multiplication.
 */

#include<iostream>
#include<vector>
#include<thread>
#include<algorithm>
#include<cmath>
#include<map>
#include<omp.h>

#include"csr.h"
#include"spmv.h"
#include"lookup.h"

std::vector<std::vector<int>> get_products(std::vector<std::vector<int>> x_) {
    int t = x_.size();

    std::vector<int> temp_;
    std::vector<std::vector<int>> res_;

    // std::cout << "[INFO] all possible vectors." << "\n";

    for (int i = 0; i < std::pow(2, t); i++) {
        for(int j = 0 ; j < t; ++j) {
                temp_.push_back((i & (0x01 << j)) ? 1 : 0); 
        }
        
        std::reverse(temp_.begin(),  temp_.end());

        /*
        for (int a: temp_) {
            std::cout << a << " ";
        }
        std::cout << "\n";
        */

        // this can def exploit sparseness later
            // also needs to be Boolean; not as much time spent
            // on random ALU ops as mults.
        res_.push_back(mv_naive(x_, temp_));

        // can we put this on an FPGA??
        temp_.clear();
    }

    // std::cout << "\n";

    return res_;
}

std::vector<std::vector<int>> four_russians_serial(std::vector<std::vector<int>>& A_, std::vector<std::vector<int>>& B_) {
    //int bound = floor(log2(A_.size()));

    std::vector<std::vector<int>> C(A_.size(), std::vector<int>(A_.size()));

    //CSR a_(A_);
    
    int n = A_.size();
    // int t = ceil(log2(n)/2);

    int t = 5;

    cout << "Partition Value: " << t << endl;

    map<vector<int>, vector<vector<int>>> lut;  
    long start = clock();

    for (int i = 0; i < n / t; i++) {
        for(int j = 0; j < n / t; j++) {
            vector<vector<int>> temp_mat;
            
            for(int m = 0; m < t; m++){
                vector<int> row;
                
                for(int n = 0; n < t; n++) {
                    row.push_back(A_[t*i + m][t*j + n]);
                }
                temp_mat.push_back(row);
            }
            CSR temp_csr(temp_mat);
            lut[{i,j}] = computeLUT_3(temp_csr);
        }
    }
    long end = clock();
    cout << "LUT Computation: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";
    #if 0
    for(auto i: lut){
        cout << "Key: " << i.first[0] << " " << i.first[1] << '\n';
        for(auto m: i.second){
            for(auto n: m){
                cout <<  n << " ";
            }
            cout << '\n';
        }  
    }
    #endif
    
    start = clock();
    for (int i = 0; i < n; i++) {
        for (int j = 0, m = 0; j < n; m = j) {
            int k = 0;
            int index = 0;

            while (k < t) {
                index = index << 1;
                index = index | B_[i][j];

                k++;
                j++;
            }

            vector<int> temp = lut[{i/t,(j-1)/t}][index];

            for(auto x : temp){
                C[m][i] = x;
                m++;
            }
        }
    }
    end = clock();
    cout << "Matrix Computation: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";
    return C;
}

std::vector<std::vector<int>> four_russians_parallel(std::vector<std::vector<int>>& A_, std::vector<std::vector<int>>& B_) {
    //int bound = floor(log2(A_.size()));

    std::vector<std::vector<int>> C(A_.size(), std::vector<int>(A_.size()));

    //CSR a_(A_);

    //omp_set_num_threads(NUM_THREADS);
    
    int n = A_.size();
    // int t = ceil(log2(n)/2);

    int t = 5;
    cout << "Partition Value: " << t << endl; 

    map<vector<int>, vector<vector<int>>> lut;  
    long start = clock();
    #pragma omp parallel for
    for (int i = 0; i < n / t; i++){
        
        #pragma omp parallel for
        for (int j = 0; j < n / t; j++){
            vector<vector<int>> temp_mat;
            
            for(int m = 0; m < t; m++){
                vector<int> row;
                for(int n = 0; n < t; n++){
                    row.push_back(A_[t*i + m][t*j + n]);
                }
                temp_mat.push_back(row);
            }
            
            CSR temp_csr(temp_mat);
            lut[{i, j}] = computeLUT_3(temp_csr);
        }
    }
    long end = clock();
    cout << "LUT Computation: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";

    #if 0
    for(auto i: lut){
        cout << "Key: " << i.first[0] << " " << i.first[1] << '\n';
        for(auto m: i.second){
            for(auto n: m){
                cout <<  n << " ";
            }
            cout << '\n';
        }  
    }
    #endif

    // omp_set_num_threads(4);
    
    start = clock();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0, m = 0; j < n; m = j) {
            int k = 0;
            int index = 0;
            
            while(k < t) {
                index = index << 1;
                index = index | B_[i][j];
                k++;
                j++;
            }
            
            vector<int> temp = lut[{i / t, (j - 1) / t}][index];
            for(auto x : temp) {
                C[m][i] = x;
                m++;
            }
        }
    }
    end = clock();
    cout << "Matrix Computation: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";
    return C;
}

#if 0
int main() {
    std::vector<std::vector<int>> x;

    x.push_back({1, 2, 5, 4});
    x.push_back({2, 6, 1, 0});
    x.push_back({9, 2, 0, 7});
    x.push_back({3, 2, 8, 4});

    x = get_products(x);

    std::cout << "[INFO] final products: " << "\n";

    for (vector<int> a: x) {
        for (int i: a) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

    std::cout<<"\n";

    return 0;
}
#endif

void getLUTComputeThread(int t, int a, int b, vector<vector<int>> A, map<vector<int>, vector<vector<int>>>& lut) {
    vector<vector<int>> temp;

    for (int i = 0; i < t; i++) {
        vector<int> row;

        for (int j = 0; j < t; j++) {
            row.push_back(A[t * a + i][t * b + j]);
        }

        temp.push_back(row);
    }

    CSR t_csr(temp);
    lut[{a, b}] = computeLUT_3(t_csr);
}

vector<vector<int>> four_russians_parallel_2(vector<vector<int>>& A_, vector<vector<int>>& B_) {
    vector<vector<int>> C(A_.size(), vector<int>(A_.size()));

    int n = A_.size();
    int t = 5;

    map<vector<int>, vector<vector<int>>> lut;

    long start = clock();

    vector<thread> threads;

    for (int i = 0; i < n / t; i++) {
        for (int j = 0; j < n / t; j++) {
           threads.push_back(thread(getLUTComputeThread, t, i, j, A_, std::ref(lut))); 
        }
    }

    cout << "Started threads :D" << "\n";

    for (auto& th: threads) {
        th.join();
    }

    cout << "Joined threads >:(" << "\n";
    long end = clock();

    cout << "LUT Computation: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";

    start = clock();
    #pragma omp parallel for
    for(int i = 0; i < n; i++){
        for(int j = 0, m = 0; j < n; m = j){
            int k = 0;
            int index = 0;
            while(k < t){
                index = index << 1;
                index = index | B_[i][j];
                k++;
                j++;
            }
            vector<int> temp = lut[{i/t,(j-1)/t}][index];
            for(auto x : temp){
                C[m][i] = x;
                m++;
            }
        }
    }
    end = clock();
    cout << "Matrix Computation: " << (double) (end - start) / CLOCKS_PER_SEC << "\n";
    return C;
}