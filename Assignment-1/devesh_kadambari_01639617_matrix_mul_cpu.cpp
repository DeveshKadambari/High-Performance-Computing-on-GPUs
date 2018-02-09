#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<iostream>
#include <time.h>

    using namespace std;
int a[1024][2048];
int b[2048][1024];
int c[1024][1024];
int count;
//pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;

    class matrix
    {
    public:
      int n_o_r, n_o_c,d;
     };

void *data_in(void *threadid) {
 //cout<<"hello"<<endl;
int c=count;
   class matrix *L=(class matrix*)threadid;
    //cout<<"n_o_r"<<L->n_o_r<<" "<<"n_o_c"<<L->n_o_c<<endl;      //L->n_o_r = 1024 L->n_o_c = 2048 count will exceed 2048
//pthread_mutex_lock( &count_mutex );
    for(int j=0;j<(L->n_o_c);j++)
    {                                    //fill all the column elements by row wise
        if(c < 1025){//cout<<"c "<<c<<endl;
a[c][j]=rand()%10;
 //cout<<"a["<<c<<"]"<<"["<<j<<"]"<<a[c][j]<<endl;
}
        if(j<=1024){b[c][j]=rand()%10;}
     // cout<<"b["<<c<<"]"<<"["<<j<<"]"<<b[c][j]<<endl;
    }
// pthread_mutex_unlock( &count_mutex );

   pthread_exit(NULL);
}
void *multiplication(void *contex)
{
    int sum;
     class matrix *P=(class matrix*)contex;
      for(int k = 0; k< 2048; k++){
      sum += a[P->n_o_r][k] * b[k][P->n_o_c];
   }
   c[P->n_o_r][P->n_o_c] = sum;
   //cout<<"c["<<P->n_o_r<<"]"<<"["<<P->n_o_c<<"]"<< c[P->n_o_r][P->n_o_c]<<endl;
    pthread_exit(NULL);

}

    int main ()
    {
clock_t t;
    t = clock();

    matrix A, B;
      A.n_o_r= 1024;
      A.n_o_c= 2048;
      count=-1;
pthread_t threads[2048];
 int rc,i;
cout<<"Data_in has started"<<endl;
	 for( i = 0; i < 2048; i++ ) {                                   
     count= count+ 1;
     //cout<<"count"<<count<<endl;
      //cout << "main() : creating thread, " << i << endl;
      //if(i%2==0){
      rc = pthread_create(&threads[i], NULL,data_in,(void *) &A);             //initiate all the threads row wise
     
 //}
      /*else
      {
        rc = pthread_create(&threads[i], NULL,data_in,(void *) &B);   
      }*/
        //cout<<"Thread return value "<<rc<<endl;
	//cout<<"a[1024][2048]"<<a[1023][2047]<<endl;

}
for(int i = 0; i < 1024; i++) {
      for(int j = 0; j < 1024; j++) {
          B.n_o_r=i;
          B.n_o_c=j;
          rc = pthread_create(&threads[j], NULL,data_in,(void *) &B);
          pthread_join(threads[i], NULL);
      }
      
   }
t = clock()-t;
 double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
cout<<"data_in total time : "<<time_taken<<"seconds"<<endl;
 return 0;
    }
    
