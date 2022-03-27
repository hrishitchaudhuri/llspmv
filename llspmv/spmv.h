std::vector<int> mv_naive(std::vector<std::vector<int>> mtx_, std::vector<int> y_);
std::vector<int> spmv_serial(CSR csr_, std::vector<int> x_);
std::vector<int> spmv_parallel_1(CSR csr_, std::vector<int> x_);