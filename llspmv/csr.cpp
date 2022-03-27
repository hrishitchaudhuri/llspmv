/*
 * CSR Implementation
 */
#include<iostream>
#include"csr.h"
#include<vector>

CSR::CSR(vector<vector<int>> arr){
    int rowptr = 0;
    int i = 0;
    row_pointer.push_back(rowptr);
    for(int i = 0; i < arr.size(); i++){
        for(int j = 0; j < arr[i].size(); j++){
            if(arr[i][j]!=0){
                column_index.push_back(j);
                values.push_back(arr[i][j]);
                rowptr++;
            }
        }
        row_pointer.push_back(rowptr);
    }
    for(auto k: row_pointer){
        cout << k << ' ';
    }
    cout << endl;
    for(auto k: column_index){
        cout << k << ' ';
    }
    cout << endl;
    for(auto k: values){
        cout << k << ' ';
    }
    cout << endl;
}