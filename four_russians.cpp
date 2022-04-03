/*
 * trying to implement the Four Russians method for Boolean Matrix Multiplication.
 */

#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>

#include"csr.h"
#include"spmv.h"

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

std::vector<std::vector<int>> four_russians_bmm(std::vector<std::vector<int>> A_, std::vector<std::vector<int>> B_) {
    int bound = floor(log2(A_.size()));

    std::vector<std::vector<int>> C(A_.size(), std::vector<int>(A_.size()));

    std::vector<std::vector<int>> lookupTable;
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