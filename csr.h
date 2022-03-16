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
    public:
    CSR(vector<vector<int>> matrix);
};
#endif