/*
College: University of Massachusetts Lowell
EECE 7110:High-Performance Comp. on GPUs
Semester: Spring 2018
Student : 01639617
Project :Coalesced memory
Professor : Dr.Hang Liu
Due date: 2/12/2017
Authors : Sai Sri Devesh Kadambari
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
using namespace std;

#define zero 0

__global__ void gpu_transpose_mult(int *a,int *b, int *c, int m, int n, int k)
{
  __shared__ int smem[256];
    int row = blockIdx.x ;
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int step = m/gridDim.x;                                             //step=80
    
    float f=(blockDim.x)/2;int k_b;
    __syncthreads();                                                    //wait until all the threads in the block reach this point
    while(row < m)                            //Row=0->80
 { 	
	  for(int k_l=0;k_l<n;K_l++)
	  {
		while(tid<n)
			{
		smem[tid] +=a[row * n +tid] *b[k_l*k +tid];     //h_a[row_variable*Max_column + column_variable]//save multiplication value into the smem buffer
		tid+=blockDim.x;
			}
		
         __syncthreads();                                               //wait until all the threads reach this point
                for(int j=((blockDim.x)/2);j>0;j=ceilf(f))            //i=250/2 is 125->62.5(63)->(63-1)->(21)->20->(10)->(5)->(3)->(2)->(1)
                { 
			k_b=2*f;
                if(((k_b)%2!=0) && (threadIdx.x == (j-1)))
                	{
                        smem[threadIdx.x -1]+=smem[threadIdx.x];
                        j=j-1;
                        f=j;
               		}

                        if(threadIdx.x < j)
                        { int temp =smem[threadIdx.x]+smem[threadIdx.x + j];
                         smem[threadIdx.x]=temp;
                        }
			 __syncthreads();
                        f=f/2;
                }
          c[row * k+k_l]=smem[zero];
		
        }
  row +=gridDim.x;
}
}

__global__ void gpu_regular_mult(int *a,int *b, int *c, int m, int n, int k)
{
  __shared__ int smem[256];
    int row = blockIdx.x ;
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int step = m/gridDim.x;                                             //step=80
    
    float f=(blockDim.x)/2;int k_b;
    __syncthreads();                                                    //wait until all the threads in the block reach this point
    while(row < m)                            //Row=0->80
 { 	
	  for(int k_l=0;k_l<n;K_l++)
	  {
		while(tid<n)
			{
		smem[tid] +=a[row * n+tid] *b[tid*k+k_l];     //h_a[row_variable*Max_column + column_variable]//save multiplication value into the smem buffer
		tid+=blockDim.x;
			}
		
         __syncthreads();                                               //wait until all the threads reach this point
                for(int j=((blockDim.x)/2);j>0;j=ceilf(f))            //i=250/2 is 125->62.5(63)->(63-1)->(21)->20->(10)->(5)->(3)->(2)->(1)
                { 
			k_b=2*f;
                if(((k_b)%2!=0) && (threadIdx.x == (j-1)))
                	{
                        smem[threadIdx.x -1]+=smem[threadIdx.x];
                        j=j-1;
                        f=j;
               		}

                        if(threadIdx.x < j)
                        { int temp =smem[threadIdx.x]+smem[threadIdx.x + j];
                         smem[threadIdx.x]=temp;
                        }
			 __syncthreads();
                        f=f/2;
                }
          c[row * k+k_l]=smem[zero];
		
        }
  row +=gridDim.x;
}
}

int main(int argc, char const *argv[])
{
    int m, n, k;
    printf("please type in m=A_rows n=A_columns and k=B_columns \n");
    scanf("%d %d %d", &m, &n, &k);
    // allocate memory in host RAM, h_cc is used to store CPU result
    int *h_a, *h_b, *h_c;// *h_cc;
    cudaMallocHost((void **) &h_a, sizeof(int)*m*n);
    cudaMallocHost((void **) &h_b, sizeof(int)*n*k);
    cudaMallocHost((void **) &h_c, sizeof(int)*m*k);
    //cudaMallocHost((void **) &h_cc, sizeof(int)*m*k);

 for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
		if(j<510){	
	  h_a[i * n + j] = rand() % 1024;}          //h_a[row_variable*Max_column + column_variable]
		else{ h_a[i * n + j]=0;
			}

        }
}

for (int i = 0; i < n; ++i)
{
        for (int j = 0; j < k; ++j) 
	{
		if(j<510){h_b[i * k + j] = rand() % 1024;}
		else{ h_b[i * k + j] =0;}
        }
}


clock_t t;
    t = clock();
int *d_a, *d_b, *d_c;
    cudaMalloc((void **) &d_a, sizeof(int)*m*n);
    cudaMalloc((void **) &d_b, sizeof(int)*n*k);
    cudaMalloc((void **) &d_c, sizeof(int)*m*k);
    cudaMemcpy(d_a, h_a, sizeof(int)*m*n, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, sizeof(int)*n*k, cudaMemcpyHostToDevice);

    //unsigned int grid_rows = (m + BLOCK_SIZE - 1) / BLOCK_SIZE;
    //unsigned int grid_cols = (k + BLOCK_SIZE - 1) / BLOCK_SIZE;
    dim3 dimGrid(128);
    dim3 dimBlock(256);

if(m==512){
 gpu_transpose_mult<<<dimGrid, dimBlock>>>(d_a, d_b, d_c, m, n, k);}
else{
gpu_regular_mult<<<dimGrid, dimBlock>>>(d_a, d_b, d_c, m, n, k);}

cudaMemcpy(h_c, d_c, sizeof(int)*m*k, cudaMemcpyDeviceToHost);

cudaThreadSynchronize();

t = clock()-t;
double time_taken = ((double)t)/CLOCKS_PER_SEC;
printf("Time elapsed on matrix multiplication of %dx%d . %dx%d on CPU: %lf ms.\n\n", m, n, n, k, (time_taken*1000));

  cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cudaFreeHost(h_a);
    cudaFreeHost(h_b);
    cudaFreeHost(h_c);
     return 0;
}

