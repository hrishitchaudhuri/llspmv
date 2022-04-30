#include<iostream>
#include<vector>
#include<cmath>

using namespace std;

typedef vector<vector<int>> Matrix;
typedef vector<int> Vector;

Vector getRowFromBottom(Matrix A, int idx, int ps, int position) {
    return A[ps * (idx + 1) - position];
}

Vector vectorOr(Vector v1, Vector v2) {
    Vector v3;

    for (int i = 0; i < v1.size(); i++) {
        v3.push_back(v1[i] | v2[i]);
    }

    return v3;
}

Matrix getRowSums(Matrix A, int n, int ps, int idx) {
    Matrix temp (pow(2, ps), Vector(n));
    Vector v_t (n);

    int bp = 1;
    int k = 0;

    for (int i = 1; i < pow(2, ps); i++) {
        v_t = getRowFromBottom(A, idx, ps, k + 1);
        temp[i] = vectorOr(temp[i - pow(2, k)], v_t);

        if (bp == 1) {
            bp = i + 1;
            k++;
        }

        else {
            bp--;
        }
    }

    return temp;
}

void copyVectors(Vector& dest, Vector& src, int start1, int end1, int start2, int end2) {
    for (int i1 = start1, i2 = start2; i1 < end1; i1++, i2++) {
        dest[i1] = src[i2];
    }
}

int getDecimalFromBinVector(Vector v) {
    int val = 0;

    for (int i1 = 0, i2 = v.size() - 1, i3 = 1; i1 < v.size(); i1++, i2--, i3 = i3 << 1) {
        val += v[i2] * i3;
    }

    return val;
}

Matrix matrixOr(Matrix m1, Matrix m2) {
    Matrix temp (m1.size(), Vector (m1.size())); 

    for (int i = 0; i < m1.size(); i++) {
        for (int j = 0; j < m2.size(); j++) {
            temp[i][j] = m1[i][j] | m2[i][j];
        }
    }

    return temp;
}

Matrix boolMatrixMult(Matrix A, Matrix B, int partitionSize) {
    Matrix C (A.size(), Vector(A.size()));
    Matrix C_temp (A.size(), Vector(A.size()));
    Matrix RowSums;

    Vector IndexVector (partitionSize);
    int idx;

    for (int i = 0; i < A.size() / partitionSize; i++) {
        RowSums = getRowSums(B, B.size(), partitionSize, i);

        for (int j = 0; j < A.size(); j++) {
            copyVectors(IndexVector, A[j], 0, partitionSize, i * partitionSize, (i + 1) * partitionSize);
            
            #if 0
            cout << "INDEX: " << "\n";
            for (int k = 0; k < partitionSize; k++) {
                cout << IndexVector[k] << " ";
            }
            cout << "\n\n";
            #endif

            idx = getDecimalFromBinVector(IndexVector);
            copyVectors(C_temp[j], RowSums[idx], 0, A.size(), 0, A.size());
        }

        C = matrixOr(C, C_temp);

        #if 0
        cout << "TEMP C_temp in ITER " << i << ":\n";

        for (int i = 0; i < C_temp.size(); i++) {
            for (int j = 0; j < C_temp[i].size(); j++) {
                cout << C_temp[i][j] << " ";
            }
            cout << "\n";
        }

        cout << "\n\n\n";  

        cout << "TEMP C in ITER " << i << ":\n";

        for (int i = 0; i < C.size(); i++) {
            for (int j = 0; j < C[i].size(); j++) {
                cout << C[i][j] << " ";
            }
            cout << "\n";
        }

        cout << "\n\n\n";  
        #endif      
    }

    return C;
}

Matrix generateSparseMatrix(int n, int sparsity) {
    Matrix m_1;
    Vector m_2;

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

Vector generateSparseVector(int n, int sparsity) {
    Vector v_1;

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

#define NUM_ELEMENTS 15
#define SPARSITY_COEF 7
#define PARTITION_SZ 3

int main() {
    Matrix a = generateSparseMatrix(NUM_ELEMENTS, SPARSITY_COEF);
    Matrix b = generateSparseMatrix(NUM_ELEMENTS, SPARSITY_COEF);

    cout << "A: " << "\n";

    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            cout << a[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n\n\n";

    cout << "B: " << "\n";

    for (int i = 0; i < b.size(); i++) {
        for (int j = 0; j < b[i].size(); j++) {
            cout << b[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n\n\n";

    Matrix c;
    Vector temp;

    int res;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        temp.clear();
        for (int j = 0; j < NUM_ELEMENTS; j++) {
            res = 0;
            for (int k = 0; k < NUM_ELEMENTS; k++) {
                res = res | (a[i][k] & b[k][j]);
            }
            temp.push_back(res);
        }
        c.push_back(temp);
    }

    cout << "C: " << "\n";

    for (int i = 0; i < c.size(); i++) {
        for (int j = 0; j < c[i].size(); j++) {
            cout << c[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n\n\n";

    #if 0
    Vector test_v = getRowFromBottom(c, 0, c.size(), 2);
    for (int i = 0; i < c.size(); i++) {
        cout << test_v[i] << " ";
    }
    cout << "\n";
    #endif

    Matrix out_1 = boolMatrixMult(a, b, PARTITION_SZ);

    cout << "OUT_1: " << "\n";

    for (int i = 0; i < out_1.size(); i++) {
        for (int j = 0; j < out_1[i].size(); j++) {
            cout << out_1[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n\n\n";


    #if 1
    for (int i = 0; i < out_1.size(); i++) {
        for (int j = 0; j < out_1[i].size(); j++) {
            if (out_1[i][j] != c[i][j]) {
                cout << "Mismatch on " << i << " " << j << "\n";
            }
        }
    }
    #endif
}