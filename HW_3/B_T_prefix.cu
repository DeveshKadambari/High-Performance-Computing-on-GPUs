/*
College: University of Massachusetts Lowell
EECE 7110:High-Performance Comp. on GPUs
Semester: Spring 2018
Student : 01639617
Project : Assignment_3
Professor : Dr.Hang Liu
Due date: 4/16/2018
Authors : Sai Sri Devesh Kadambari
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
using namespace std;

#define zero 0

__global__ void gpu_up_swing(int *a,int *c, int m)
{
  __shared__ int smem[1];
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int p=16384,n=1,pwr;					//p=m/blockdim*griddim
    while((tid<(p*n))&&(tid<(32000000)))
{
    __syncthreads();                                                    //wait until all the threads in the block reach this point
for(int depth =0;depth< __logf ( 16384) ;depth++)
{	pwr=__powf(depth, 2);
	pwr_1=__powf(depth, 2+1);
if(tid%pwr_1==0){
   a[tid+pwr_1-1]+=a[tid+pwr_1-1];
}
__syncthreads();
}
	gpu_down_swing(*a,*c,m);
     smem[1]=a[p*n];
	tid+=p;
	a[tid]+=smem[1];
	n++;
	__syncthreads(); 
}
}

__global__ void gpu_down_swing(int *a,int *c, int m)
{
  __shared__ int smem[1];
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int p=16384,n=1,pwr;					//p=m/blockdim*griddim
    while((tid<(p*n))&&(tid<(32000000)))
{
    __syncthreads();                                                    //wait until all the threads in the block reach this point
for(int depth =0;depth< __logf ( 16384) ;depth++)
{	pwr=__powf(depth, 2);
	pwr_1=__powf(depth, 2+1);

   a[tid+pwr_1-1]+=a[tid+pwr_1-1];
__syncthreads();
}
     smem[1]=a[p*n];
	tid+=p;
	a[tid]+=smem[1];
	n++;
	__syncthreads(); 
}
}


int main(int argc, char const *argv[])
{ int m;
    printf("please type in m(size)  \n");
    scanf("%d", &m);

    // allocate memory in host RAM, h_cc is used to store CPU result
    int *h_a, *h_c;// *h_cc;
    cudaMallocHost((void **) &h_a, sizeof(int)*m);
    cudaMallocHost((void **) &h_c, sizeof(int)*m);

 for (int i = 0; i < m; ++i) {
       
  h_a[i] = rand() % 1024;          //loading random values
       
}

clock_t t;
    t = clock();
int *d_a,*d_c;
    cudaMalloc((void **) &d_a, sizeof(int)*m);
    cudaMalloc((void **) &d_c, sizeof(int)*m);
    cudaMemcpy(d_a, h_a, sizeof(int)*m, cudaMemcpyHostToDevice);
    dim3 dimGrid(128);
    dim3 dimBlock(128);


 gpu_up_swing<<<dimGrid, dimBlock>>>(d_a,d_c, m);

cudaMemcpy(h_c, d_c, sizeof(int)*m, cudaMemcpyDeviceToHost);
gpu_up_swing<<<dimGrid, dimBlock>>>(d_a,d_c, m);
cudaThreadSynchronize();

t = clock()-t;
 double time_taken = ((double)t)/CLOCKS_PER_SEC;
printf("Time elapsed on operation of %d: %lf ms.\n\n", m,(time_taken/1000));

cudaFree(d_a);
    
    cudaFree(d_c);
    cudaFreeHost(h_a);
   
    cudaFreeHost(h_c);
    //cudaFreeHost(h_cc);
    return 0;
}

