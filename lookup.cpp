#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>

#include"csr.h"
#include"spmv.h"
#include "lookup.h"

using namespace std;

vector<vector<int>> computeLUT_1(vector<vector<int>> x) {
    int t = x.size();

    vector<int> m_1;
    vector<vector<int>> m_2;

    for (int i = 0; i < pow(2, t); i++) {
        for (int j = t - 1; j >= 0; j--) {
            m_1.push_back((i & (0x01 << j)) ? 1 : 0);
        }

        m_2.push_back(bool_mv_naive(x, m_1));

        m_1.clear();
    }

    return m_2;
}

vector<vector<int>> computeLUT_2(CSR x) {
    int t = x.original_size;

    vector<int> m_1;
    vector<vector<int>> m_2;

    for (int i = 0; i < pow(2, t); i++) {
        for (int j = t - 1; j >= 0; j--) {
            m_1.push_back((i & (0x01 << j)) ? 1 : 0);
        }

        m_2.push_back(bool_spmv_serial(x, m_1));

        m_1.clear();
    }

    return m_2;
}

vector<vector<int>> computeLUT_3(CSR x) {
    int t = x.original_size;

    vector<int> m_1;
    vector<vector<int>> m_2;

    for (int i = 0; i < pow(2, t); i++) {
        for (int j = t - 1; j >= 0; j--) {
            m_1.push_back((i & (0x01 << j)) ? 1 : 0);
        }

        m_2.push_back(bool_spmv_parallel_1(x, m_1));
        m_1.clear();
    }

    return m_2;
}

vector<vector<int>> computeLUT_vector(vector<vector<int>> mat){
    int t = mat.size();
    vector<int> m_1;
    vector<vector<int>> m_2;
    for(int i = 0; i < pow(2, t); i++){
        for (int j = t - 1; j >= 0; j--) {
            m_1.push_back((i & (0x01 << j)) ? 1 : 0);
        }

        m_2.push_back(bool_spmv_parallel_1(mat, m_1));
        m_1.clear();
    }
    return m_2;
}