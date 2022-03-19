#include"csr.h"
#include<vector>

using namespace std;

int main(int argc, char * argv[]){
    vector<vector<int>> a = {
        {3,0,0,0}, 
        {1,0,4,0}, 
        {0,0,0,5}, 
        {5,3,0,0}
    };
    CSR csr(a);
    csr.display_csr();
}