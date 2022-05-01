#include<stdio.h>
#include<omp.h>


int main(){
    #pragma omp parallel
    {
        printf("Hello world! %d\n", omp_get_thread_num());
    }
    int a[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                printf("%d from %d/%d\n", a[i][j], omp_get_thread_num(), omp_get_num_threads());
            }
        }
    }
    return 0;
}