/*
 * Header file for the CSR Library
 */
#ifndef CSR_H
#define CSR_H
#include<vector>
using namespace std;

class CSR {
    vector<int> row_pointer;
    vector<int> column_index;
    vector<int> values;
    void scan_row_thread(vector<int>& row);
    public:
    CSR(vector<vector<int>> matrix);
    void display_csr();
};
#endif