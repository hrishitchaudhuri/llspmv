#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <time.h>

#include "fr.h"

#include <omp.h>

#define CLOCK 1

using namespace std;

vector<int> booleanVectorMultiplication(vector<vector<int>> &mat, vector<int> &vec)
{
    vector<int> out;
    int size = mat.size();
    for (int i = 0; i < size; i++)
    {
        int res = 0;
        for (int j = 0; j < mat[i].size(); j++)
        {
            res = res || (mat[i][j] && vec[j]);
        }
        out.push_back(res);
    }
    return out;
}

vector<vector<int>> calculateProducts(vector<vector<int>> &mat)
{
    vector<vector<int>> lut;
    int size = mat[0].size();
    long num_combinations = pow(2, size);
    for (long i = 0; i < num_combinations; i++)
    {
        vector<int> temp;
        for (int j = size - 1; j >= 0; j--)
        {
            temp.push_back((i & (0x01 << j)) ? 1 : 0);
        }
        vector<int> t = booleanVectorMultiplication(mat, temp);
        lut.push_back(t);
    }
    return lut;
}

vector<vector<int>> parallel_calculateProducts(vector<vector<int>> &mat)
{
    vector<vector<int>> lut;
    int size = mat[0].size();
    long num_combinations = pow(2, size);
    #pragma omp parallel for ordered
    for (long i = 0; i < num_combinations; i++)
    {
        vector<int> temp;
        for (int j = size - 1; j >= 0; j--)
        {
            temp.push_back((i & (0x01 << j)) ? 1 : 0);
        }
        vector<int> t = booleanVectorMultiplication(mat, temp);
        #pragma omp ordered
        lut.push_back(t);
    }
    return lut;
}

vector<vector<int>> fourRussians(vector<vector<int>> &a, vector<vector<int>> &b)
{
    int T = 5;
    int size = a.size();
    double start = omp_get_wtime();
    // Preprocess A matrix
    map<int, vector<vector<int>>> LUT;
    for (int I = 0; I < size; I += T)
    {
        vector<vector<int>> current_matrix(T, vector<int>(a.size(), 0));
        for (int i = 0; i < T; i++)
        {
            for (int j = 0; j < a[0].size(); j++)
            {
                current_matrix[i][j] = a[i + I][j];
            }
        }
        LUT[I / T] = calculateProducts(current_matrix);
    }
    // Preprocess B matrix
    vector<int> pre_b(b.size(), 0);
    for (int j = 0; j < b[0].size(); j++)
    {
        int res = 0;
        for (int i = 0; i < b.size(); i++)
        {
            res = (res << 1) | b[i][j];
        }
        pre_b[j] = res;
    }
    double end = omp_get_wtime();
#if CLOCK
    cout << "FR Preprocessing time taken: " << (double)(end - start) << "\n";
#endif
#if 0
    // Print the LUT
    for(auto i: LUT){
        cout << i.first << ":\n";
        for(auto j: i.second){
            for(auto k: j){
                cout << k << " ";
            }
            cout << "\n";
        }
    }
#endif
#if 0
    // Print preprocessed B matrix
    cout << "Preprocessed B:\n";
    for(auto i : pre_b){
        cout << i << " ";
    }
    cout << "\n";
#endif

    vector<vector<int>> output(size, vector<int>(size, 0));
#if 0
    for(int j = 0; j < size; j++){
        for(int i = 0; i < size/T; i++){
            cout << "Current index info: " << j/T << " " << i
                 << " Index : "  << i << " " << j  << " " << pre_b[i][j] << endl; 
            vector<int> req = LUT[{j/T,i}][pre_b[i][j]];
            for(int k = i*T; k < T; k++){
                output[k][j] = req[k];
            }
        }
    }
#endif
    start = omp_get_wtime();
    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size / T; i++)
        {
            vector<int> req = LUT[i][pre_b[j]];
            for (int k = 0; k < T; k++)
            {
                output[k + T * i][j] = req[k];
            }
        }
    }
    end = omp_get_wtime();
#if CLOCK
    cout << "Result matrix computation time taken: " << (double)(end - start) << "\n";
#endif
    return output;
}

vector<vector<int>> parallel_fourRussians(vector<vector<int>> &a, vector<vector<int>> &b)
{
    int T = 5;
    int size = a.size();
    double start = omp_get_wtime();
    map<int, vector<vector<int>>> LUT;
    vector<int> pre_b(b.size(), 0);
// Preprocess A matrix
#pragma omp parallel for
    for (int I = 0; I < size; I += T)
    {
        vector<vector<int>> current_matrix(T, vector<int>(a.size(), 0));
        for (int i = 0; i < T; i++)
        {
            for (int j = 0; j < a[0].size(); j++)
            {
                current_matrix[i][j] = a[i + I][j];
            }
        }
        LUT[I / T] = parallel_calculateProducts(current_matrix);
    }
// Preprocess B matrix
#pragma omp nowait
#pragma omp parallel for
    for (int j = 0; j < b[0].size(); j++)
    {
        // cout << omp_get_thread_num() << "/" << omp_get_num_threads() << '\n';
        int res = 0;
        for (int i = 0; i < b.size(); i++)
        {
            res = (res << 1) | b[i][j];
        }
        pre_b[j] = res;
    }
#pragma omp barrier
    double end = omp_get_wtime();
#if CLOCK
    cout << "FR PARALLEL Preprocessing time taken: " << (double)(end - start) << "\n";
#endif
#if 0
    // Print the LUT
    for(auto i: LUT){
        cout << i.first << ":\n";
        for(auto j: i.second){
            for(auto k: j){
                cout << k << " ";
            }
            cout << "\n";
        }
    }
#endif
#if 0
    // Print preprocessed B matrix
    cout << "Preprocessed B:\n";
    for(auto i : pre_b){
        cout << i << " ";
    }
    cout << "\n";
#endif

    vector<vector<int>> output(size, vector<int>(size, 0));
#if 0
    for(int j = 0; j < size; j++){
        for(int i = 0; i < size/T; i++){
            cout << "Current index info: " << j/T << " " << i
                 << " Index : "  << i << " " << j  << " " << pre_b[i][j] << endl; 
            vector<int> req = LUT[{j/T,i}][pre_b[i][j]];
            for(int k = i*T; k < T; k++){
                output[k][j] = req[k];
            }
        }
    }
#endif
    start = omp_get_wtime();
    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size / T; i++)
        {
            vector<int> req = LUT[i][pre_b[j]];
            for (int k = 0; k < T; k++)
            {
                output[k + T * i][j] = req[k];
            }
        }
    }
    end = omp_get_wtime();
#if CLOCK
    cout << "Result matrix PARALLEL computation time taken: " << (double)(end - start) << "\n";
#endif
    return output;
}
