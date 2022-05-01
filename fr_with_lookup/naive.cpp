#include<iostream>
#include<vector>
#include<map>
#include "naive.h"
#include "omp.h"

#define CLOCK 1

using namespace std;

vector<vector<int>> booleanMatrixMultiplication(vector<vector<int>>& a, vector<vector<int>>& b){
    vector<vector<int>> c;
    double start = omp_get_wtime();
    for (int i = 0; i < a.size(); i++) {
        vector<int> temp;
        for (int j = 0; j < a[i].size(); j++) {
            int res = 0;
            for (int k = 0; k < a[i].size(); k++) {
                res = res || (a[i][k] && b[k][j]);
            }
            temp.push_back(res);
        }
        c.push_back(temp);
    }
    double end = omp_get_wtime();
    #if CLOCK
    cout << "Naive time taken: " << (double) (end-start) << "\n";
    #endif
    return c;
}

vector<vector<int>> parallel_booleanMatrixMultiplication(vector<vector<int>>& a, vector<vector<int>>& b){
    vector<vector<int>> c;
    double start = omp_get_wtime();
    #pragma omp parallel for ordered
    for (int i = 0; i < a.size(); i++) {
        vector<int> temp;
        for (int j = 0; j < a[i].size(); j++) {
            int res = 0;
            for (int k = 0; k < a[i].size(); k++) {
                res = res || (a[i][k] && b[k][j]);
            }
            temp.push_back(res);
        }
        #pragma omp ordered
        c.push_back(temp);
    }
    double end = omp_get_wtime();
    #if CLOCK
    cout << "Naive PARALLEL time taken: " << (double) (end-start) << "\n";
    #endif
    return c;
}