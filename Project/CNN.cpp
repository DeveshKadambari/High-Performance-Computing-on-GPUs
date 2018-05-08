#include<stdio.h>
#include <math.h> 
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>
#include <stdlib.h> 
#include <algorithm> 
using namespace std;
using namespace cv;

/*functions*/
void convolution(int org_n,int *org,int *out,int fil_f,int *fil,int pad,int str,int n_fil);
void filter(int *size,int *fil,int n_fil);
//void fully_connected(int out_n,int *in,int *out,int fil_f,int *fil,int *y);
void softmax(int *in,int *out);
void loss_function(int *predicted,int *truth,double *MSE);
void back_propagation(int *b,int *out,int *target,int max_fil,int fil_f,int *fil_out,int *fil_1,int *fil_0,int *out_1,int *out_2);
/*main thread*/
int main( int argc, const char** argv )
{
cout<<"hi_beginning"<<endl;
Mat img;
int *org_o,*out_1,*fil__f,*out;
img=imread(argv[1], CV_LOAD_IMAGE_COLOR);
imshow("hello",img);
unsigned char *org = (unsigned char*)(img.data);		// getting the image BGR data into char array(0->225) with 3 channels 
cout<<"hi_before_separating_b_g_r"<<endl;

int i,k,*b;

int green=img.rows*img.rows;
int red= green*green;
int *fil_0,*fil_1,*fil_out,fil_f=5;
cout<<"hi"<<endl;
fil_0=(int*) malloc(sizeof(int)*fil_f*fil_f*5);
fil_1=(int*) malloc(sizeof(int)*fil_f*fil_f*5);

int koo=img.rows-fil_f;
out_1=(int*) malloc(sizeof(int)*koo*koo*5);
int *out_2;
int foo= koo-fil_f;
out_2=(int*) malloc(sizeof(int)*foo*foo*5);
fil_out=(int*) malloc(sizeof(int)*fil_f*fil_f*5);
int poo=foo-fil_f;
out=(int*) malloc(sizeof(int)*poo*poo*4);
double *MSE;
MSE=(double*) malloc(sizeof(double)*1);
int *truth;
truth=(int*) malloc(sizeof(int)*4);
for(int i=0;i<4;i++)
{
if(i==2){
truth[i]=1;
}
else
truth[i]=0;
}
/*input layer 1*/
b=(int*) malloc(sizeof(int)*img.rows*img.cols+green+red);
cout<<"allocation"<<endl;

for(int i = 0;i < img.rows ;i++){
        for(int j = 0;j < img.cols ;j++){
            
	    b[img.cols * i + j] = org[img.cols * i + j ] ;			//Load mxmx3 matrix into the integer pointer with pixel values
            b[img.cols * i + j+green]=  org[img.cols * i + j + 1];			//After that sending it to convolution.
            b[img.cols * i + j+red] = org[img.cols * i + j + 2];
        }
    }

/*filter hidden layer 2*/
filter(&fil_f,fil_0,5);								//Hidden layer filters_1
/*filter hidden layer 3*/
filter(&fil_f,fil_1,5);								//Hidden layer filters_2	
/*filter output layer 4*/
filter(&fil_f,fil_out,4);
do{
/*Hidden 1 layer 1*/
convolution(img.rows, b,out_1,fil_f,fil_0,0,1,5);
/*Hidden 2 layer 2*/
convolution(koo, out_1,out_2,fil_f,fil_1,0,1,5);
/*output layer*/
convolution(foo, out_2,out,fil_f,fil_out,0,1,4);
softmax(out,out);
loss_function(out,truth,MSE);						//Truth not declared
int max_fil=fil_f*fil_f*fil_f;
back_propagation(b,out,truth,max_fil,fil_f,fil_out,fil_1,fil_0,out_1,out_2);
}
while(out[2] >= 0.5*truth[2]);

waitKey(0);

return 0;
}





/*First layer of CNN has Convlution,striding and activation function(ReLU)*/

void convolution(int org_n,int *org,int *out,int fil_f,int *fil,int pad,int str,int n_fil) //convolution(img.rows, *b,*out1,fil_f,*fil_0,0,0);
{

int incstr=str,o_row;
int o_col, f_row=fil_f,f_col=fil_f;
cout<<"hi_inside_convolution_f_col"<<fil_f<<",pad"<<pad<<",fil_f"<<fil_f<<",str"<<str<<",org_n"<<org_n<<endl;
o_row=((org_n+((2*pad)-fil_f))/str)+1;
cout<<"o_row"<<o_row<<endl;
o_col=o_row;
cout<<"hi_o_row_size"<<o_row<<endl;
int org_col=0;
cout<<"fil[6]"<<fil[6]<<endl;
cout<<"org[5]"<<org[5]<<endl;
//out[0]=0;
int p=0,max_fil;
max_fil=fil_f*fil_f*n_fil;

for(int n=0;n<max_fil;n++)
{
for(int l=0;l<o_row;l++)
{
for(int k=0;k<o_col;k++)
 {out[l*o_col+k]=0;
for (int i = 0; i < f_row; i++) 
  {
        for (int j = 0; j < f_col; j++) 
	{	
				
            out[l*o_col+k]+= org[i *org_n +org_col+j]*fil[i * f_col + j+p] ;        //Convolution taking place-Extending it for mxmx3(looked upon)
		

        }
  }
out[l*o_col+k]++;									//adding bias
out[l*o_col+k]=max(0,out[l*o_col+k]);							//Using Acivation function
//if((org_col+fil_f)%org_n)>=0){org_col=org_col+fil_f;}
org_col+=str;										//striding
 }
org_col--;
org_col=org_col+fil_f;									//shifting to next row
 
}
p+=fil_f*fil_f;
}
}

void filter(int *size,int *fil,int n_fil)
{
int fil_f=*size;
int p=0,max_fil;
max_fil=fil_f*fil_f*n_fil;
for(int n=0;n<max_fil;n++)
{ 
 for (int i = 0; i <fil_f; ++i) {
        for (int j = 0; j < fil_f; ++j) {
            fil[i * fil_f + j+p] = rand()%5 ;
	    cout<<"fil["<<i<<"*"<<fil_f<<"+"<<j<<"] = "<<fil[i * fil_f + j+p]<<endl;
        }
}
p=fil_f*fil_f;
}
}
/*full connected layer*/
/*
void fully_connected(int out_n,int *in,int *out,int fil_f,int *fil,int *y)
{
for(int k=0;k<out_size;k++)
 for (int i = 0; i <out_n; i++) {
        for (int j = 0; j < out_n; j++) {
            y+=in[i*out_n+j]*fil[i*out_n+j];
	    out[i*out_n+j]=in[i*out_n+j]*fil[i*out_n+j];
        }
}

}
*/
/*softmax layer*/

void softmax(int *in,int *out)
{
double *t,sum;
t=(double*) malloc(sizeof(double)*4*4);

for(int i=0;i<4;i++)
{
t[i]=exp(in[i]);
sum=sum + t[i];

}
for(int i=0;i<4;i++)
{
out[i]=(t[i]/sum);
}
}

/*loss function*/
void loss_function(int *predicted,int *truth,double *MSE)
{int k,joo;
for(int i=0;i<4;i++)
{
k=(predicted[i]-truth[i]);
joo=pow(k, 2);
MSE+=1/4*joo;
}

}
/*back propagation*/
void back_propagation(int *b,int *out,int *target,int max_fil,int fil_f,int *fil_out,int *fil_1,int *fil_0,int *out_1,int *out_2)
{
int eta=0.1,p=0;
/*This is for last filter fil_out */
for(int n=0;n<max_fil;n++)
{
int dE_dw=(out[n]-target[n])*(out[n]*(1-out[n]))*out_2[n];
 for (int i = 0; i <fil_f; ++i) {
        for (int j = 0; j < fil_f; ++j) {
            fil_out[i * fil_f + j+p] = fil_out[i * fil_f + j+p]-(eta*dE_dw) ;
	   
        }
}
p=fil_f*fil_f;
}
/*This is for last filter fil_1 dEtotal/dwh2 = dE_total/dout_h2 *dout_h2/dnet_h2 * dnet_h2/dw_h2 */
int dE_dw,dE_dw_0;
for(int n=0;n<max_fil;n++)
{ int dE_1_dout_h2=(out[n]-target[n])*(out[n])*(1-out[n])*fil_out[n];
   p=fil_f*fil_f;
 int dE_2_dout_h2=(out[n+1]-target[n+1])*(out[n+1]*(1-out[n+1]))*fil_out[n+p+1];
 int dE_3_dout_h2=(out[n+2]-target[n+2])*(out[n+2]*(1-out[n+2]))*fil_out[n+(2*p)+2];
 int dE_4_dout_h2=(out[n+3]-target[n+3])*(out[n+3]*(1-out[n+3]))*fil_out[n+(3*p)+3];
int dEtotal_dout_h2=dE_1_dout_h2+dE_2_dout_h2+dE_3_dout_h2+dE_4_dout_h2;
 for (int i = 0; i <fil_f; ++i) {
        for (int j = 0; j < fil_f; ++j) {
		dE_dw=dEtotal_dout_h2*1*out_1[i * fil_f + j+p];
            fil_1[i * fil_f + j+p] = fil_1[i * fil_f + j+p]-(eta*dE_dw) ;
	   
        }
}
/*This is for last filter fil_0 dEtotal/dwh2 = dE_total/dout_h1 *dout_h1/dnet_h1 * dnet_h1/dw_h1 */
int dEtotal_dout_1=dEtotal_dout_h2*1*fil_1[n];
    

for (int i = 0; i <fil_f; ++i) {
        for (int j = 0; j < fil_f; ++j) {
		dE_dw_0 = dEtotal_dout_1*1*b[i * fil_f + j+p];
            fil_0[i * fil_f + j+p] = fil_0[i * fil_f + j+p]-(eta*dE_dw_0) ;
	   
        }
}


}





}
