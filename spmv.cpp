#include<iostream>
#include<vector>
#include<thread>
#include<omp.h>

#include"csr.h"

#define NUM_THREADS 4

std::vector<int> mv_naive(std::vector<std::vector<int>> mtx_, std::vector<int> y_) {
	std::vector<int> res;
	int temp;

	for (int i = 0; i < y_.size(); i++) {
		temp = 0;
		for (int j = 0; j < mtx_[i].size(); j++) {
			temp += mtx_[i][j] * y_[j];
		}
		res.push_back(temp);
	}

	return res;
}

std::vector<int> bool_mv_naive(std::vector<std::vector<int>> mtx_, std::vector<int> y_) {
	std::vector<int> res;
	int temp;

	for (int i = 0; i < y_.size(); i++) {
		temp = 0;
		for (int j = 0; j < mtx_[i].size(); j++) {
			if (mtx_[i][j] & y_[j]) 
				temp = temp ^ 1;
		}
		res.push_back(temp);
	}

	return res;
}

std::vector<int> bool_mv_naive_2(std::vector<std::vector<int>> mtx_, std::vector<int> y_) {
	std::vector<int> res;
	int temp;

	for (int i = 0; i < y_.size(); i++) {
		temp = 0;

		#pragma omp parallel for
		for (int j = 0; j < mtx_[i].size(); j++) {
			if (mtx_[i][j] & y_[j]) 
				temp = temp ^ 1;
		}
		res.push_back(temp);
	}

	return res;
}

std::vector<int> spmv_serial(CSR csr_, std::vector<int> x_)	{
	int y_0;
	std::vector<int> y;

	for (int i = 0; i < (int) csr_.row_pointer.size(); i++) {
		y_0 = 0;
		
		for (int k = csr_.row_pointer[i]; k < csr_.row_pointer[i+1]; k++) {
			y_0 += csr_.values[k] * x_[csr_.column_index[k]];

			// y_0 is a shared variable; bottleneck

			// This part can be quite efficiently parallelized though
		}

		y.push_back(y_0);
	}

	return y;
}

std::vector<int> bool_spmv_serial(CSR csr_, std::vector<int> x_)	{
	int y_0;
	std::vector<int> y;

	for (int i = 0; i < (int) csr_.row_pointer.size(); i++) {
		y_0 = 0;
		
		for (int k = csr_.row_pointer[i]; k < csr_.row_pointer[i+1]; k++) {
			y_0 = y_0 ^ csr_.values[k] & x_[csr_.column_index[k]];

			// y_0 is a shared variable; bottleneck

			// This part can be quite efficiently parallelized though
		}

		y.push_back(y_0);
	}

	return y;
}

std::vector<int> spmv_parallel_1(CSR csr_, std::vector<int> x_) {
	int y_0;
	std::vector<int> y;

	omp_set_num_threads(NUM_THREADS);

	for (int i = 0; i < (int) csr_.row_pointer.size(); i++) {
		y_0 = 0;

		#pragma omp for
		for (int k = csr_.row_pointer[i]; k < csr_.row_pointer[i+1]; k++) {
			y_0 += csr_.values[k] * x_[csr_.column_index[k]];
		}

		y.push_back(y_0);
	}

	return y;
}

std::vector<int> bool_spmv_parallel_1(CSR csr_, std::vector<int> x_) {
	int y_0;
	std::vector<int> y;

	omp_set_num_threads(NUM_THREADS);

	for (int i = 0; i < (int) csr_.row_pointer.size()-1; i++) {
		y_0 = 0;

		#pragma omp parallel for
		for (int k = csr_.row_pointer[i]; k < csr_.row_pointer[i+1]; k++) {
			//printf("%d %d\n", k, omp_get_thread_num());
			y_0 = y_0 ^ (csr_.values[k] & x_[csr_.column_index[k]]);
		}

		y.push_back(y_0);
	}

	return y;
}

std::vector<int> bool_spmv_parallel_2(CSR csr_, std::vector<int> x) {
	int y_0;
	std::vector<int> y;
	return y;
}