/*
 * CSR Implementation
 */
#include <iostream>
#include "csr.h"
#include <vector>
#include <thread>
#include <functional>

void CSR::scan_row_thread(vector<int> &row)
{
    int rowptr = 0;
    int num_cols = row.size();
    for (int i = 0; i < num_cols; i++)
    {
        if (row[i] != 0)
        {
            column_index.push_back(i);
            values.push_back(row[i]);
            rowptr++;
        }
    }
}

CSR::CSR(vector<vector<int>> arr)
{
    int rowptr = 0;
    int i = 0;
    row_pointer.push_back(rowptr);
    int num_rows = arr.size();
    vector<int> row_ptr_arr(num_rows, 0); 
    for (int i = 0; i < num_rows; i++)
    {
        /*
        int num_cols = arr[i].size();
        for (int j = 0; j < num_cols; j++)
        {
            if (arr[i][j] != 0)
            {
                column_index.push_back(j);
                values.push_back(arr[i][j]);
                rowptr++;
            }
        }
        */
        thread csr_init(&CSR::scan_row_thread, this, ref(arr[i]));
        csr_init.join();
        row_pointer.push_back(rowptr);
    }
}

void CSR::display_csr()
{
    cout << "Row pointer:" << endl;
    for (auto k : row_pointer)
    {
        cout << k << ' ';
    }
    cout << "\nColumn index:\n";
    for (auto k : column_index)
    {
        cout << k << ' ';
    }
    cout << "\nValues:\n";
    for (auto k : values)
    {
        cout << k << ' ';
    }
    cout << endl;
}