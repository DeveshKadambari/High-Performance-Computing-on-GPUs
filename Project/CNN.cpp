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
void convolution(int org_n,double *org,double *out,int fil_f,int *fil,int pad,int str,int n_fil);
void filter(int *size,int *fil,int n_fil);
//void fully_connected(int out_n,int *in,int *out,int fil_f,int *fil,int *y);
void softmax(double *in,double	 *out);
void loss_function(double *predicted,int *truth,double *MSE);
void back_propagation(double *b,double *out,int *target,int max_fil,int fil_f,int *fil_out,int *fil_1,int *fil_0,double *out_1,double *out_2);
/*main thread*/
int main( int argc, const char** argv )
{
Mat img;
int *org_o,*fil__f;
double *out_1,*out;
img=imread(argv[1], CV_LOAD_IMAGE_COLOR);
cvtColor(img, img, CV_BGR2GRAY);
unsigned char *org = (unsigned char*)(img.data);		// getting the image BGR data into char array(0->225) with 3 channels 
int i,k;
double *b;
int green=img.rows*img.cols;
int red= green*2;
int *fil_0,*fil_1,*fil_out,fil_f=32;
fil_0=(int*) malloc(sizeof(int)*fil_f*fil_f*5);
fil_1=(int*) malloc(sizeof(int)*fil_f*fil_f*5);
int koo=img.rows-fil_f+1;
out_1=(double*) malloc(sizeof(double)*koo*koo*5);
double *out_2;
int foo= koo-fil_f+1;
out_2=(double*) malloc(sizeof(double)*foo*foo*5);
fil_out=(int*) malloc(sizeof(int)*fil_f*fil_f*5);
int poo=foo-fil_f;
out=(double*) malloc(sizeof(double)*poo*poo*4);
double *d_out;
d_out=(double*) malloc(sizeof(double)*poo*poo*4);
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
int a=img.rows*img.cols;
b=(double*) malloc(sizeof(double)*a);
for(int i = 0;i < img.rows ;i++){
        for(int j = 0;j < img.cols ;j++){
                b[img.cols * i + j] = org[img.cols * i + j ] ;			//Load mxmx3 matrix into the integer pointer with pixel values
		
        }
    }
int max_fil=fil_f*fil_f*5;
int ab,ba;
/*
int *trial_f,*trial_in,*trial_out,t_fsize;
double *t_out;
trial_f=(int*)malloc(sizeof(int)*3*3*2);
trial_in=(int*)malloc(sizeof(int)*5*5);
trial_out=(int*)malloc(sizeof(int)*3*3*2);
t_out=(double*)malloc(sizeof(double)*3*3*2);
t_fsize=3;
filter(&t_fsize,trial_f,2);
for(int i=0;i<25;i++)
{
trial_in[i]=1;
}
convolution(5, trial_in,trial_out,3,trial_f,0,1,2);
softmax(trial_out,t_out);
for(int i=0;i<18;i++)
{
cout<<"t_out["<<i<<"]"<<t_out[i]<<endl;
}
*/

//filter hidden layer 2
filter(&fil_f,fil_0,5);								//Hidden layer filters_1
//filter hidden layer 3
filter(&fil_f,fil_1,5);								//Hidden layer filters_2	
//filter output layer 4
filter(&fil_f,fil_out,4);
do{
//Hidden 1 layer 1
convolution(img.rows, b,out_1,fil_f,fil_0,0,1,5);
cout<<"conv_1 done"<<endl;

//Hidden 2 layer 2
convolution(koo, out_1,out_2,fil_f,fil_1,0,1,5);
cout<<"conv_2 done"<<endl;

//output layer
convolution(foo, out_2,out,fil_f,fil_out,0,1,4);			//****working good uptil here****...!!
cout<<"output layer done"<<endl;
								
softmax(out,d_out);
loss_function(d_out,truth,MSE);						//Truth not declared
cout<<"loss function done"<<endl;
//for(int i=0;i<4;i++)
//{
//cout<<"out["<<i<<"]="<<out[i];
//}
cout<<"MSE"<<*MSE<<endl;
back_propagation(b,d_out,truth,max_fil,fil_f,fil_out,fil_1,fil_0,out_1,out_2);//back_propagation(int *b,int *out,int *target,int max_fil,int fil_f,int *fil_out,int *fil_1,int *fil_0,int *out_1,int *out_2)
//cout<<"back propagation done"<<endl;
ab=d_out[2] ;
ba=truth[2];
cout<<"ab = "<<ab<<endl;
cout<<"ba ="<<ba<<endl;
}
while((ab != 0.5*ba)||(ab >= 0.5*ba));

waitKey(0);

return 0;
}





/*First layer of CNN has Convlution,striding and activation function(ReLU)*/

void convolution(int org_n,double *org,double *out,int fil_f,int *fil,int pad,int str,int n_fil) //convolution(img.rows, *b,*out1,fil_f,*fil_0,0,0);
{

int incstr=str,o_row;
int o_col, f_row=fil_f,f_col=fil_f;
//cout<<"hi_inside_convolution_f_col"<<fil_f<<",pad"<<pad<<",fil_f"<<fil_f<<",str"<<str<<",org_n"<<org_n<<endl;
o_row=((org_n+((2*pad)-fil_f))/str)+1;
//cout<<"o_row"<<o_row<<endl;
o_col=o_row;
//cout<<"hi_o_row_size"<<o_row<<endl;
int org_col=0;
//cout<<"fil[6]"<<fil[6]<<endl;
//cout<<"org[5]"<<org[5]<<endl;
//out[0]=0;
int p=0,ln=0;

for(int n=0;n<n_fil;n++)
{
for(int l=0;l<o_row;l++)
{
for(int k=0;k<o_col;k++)
 {out[l*o_col+k+ln]=0;
for (int i = 0; i < f_row; i++) 
  {
        for (int j = 0; j < f_col; j++) 
	{	
				
            out[l*o_col+k+ln]+= org[i *org_n +org_col+j]*fil[i * f_col + j+p] ;        //Convolution taking place-Extending it for mxmx3(looked upon)
		//cout<<"org["<<i<<"*"<<org_n<<"+"<<org_col<<"+"<<j<<"]"<<org[i *org_n +org_col+j]<<endl;
		//cout<<"fil["<<i<<"*"<<f_col<<"+"<<j<<"+"<<p<<"]"<<fil[i * f_col + j+p]<<endl;

        }
  }

out[l*o_col+k+ln]++;
if(out[l*o_col+k+ln]<0)
{									//adding bias
out[l*o_col+k+ln]=0;
}								//Using Acivation function
//if((org_col+fil_f)%org_n)>=0){org_col=org_col+fil_f;}
org_col+=str;										//striding
//cout<<"out["<<l<<"*"<<o_col<<"+"<<k<<"+"<<ln<<"]"<<out[l*o_col+k+ln]<<endl;
 }
org_col--;
org_col=org_col+fil_f;									//shifting to next row
 
}
org_col=0;
p+=fil_f*fil_f;
ln+=o_row*o_row;
}
//cout<<"hiii--"<<endl;
//cout<<"hood"<<endl;
}

void filter(int *size,int *fil,int n_fil)
{
int fil_f=*size;
int p=0,max_fil;
max_fil=fil_f*fil_f*n_fil;
for(int n=0;n<n_fil;n++)
{ 
 for (int i = 0; i <fil_f; ++i) {
        for (int j = 0; j < fil_f; ++j) {
            fil[i * fil_f + j+p] = rand()%5 ;
	    //cout<<"fil["<<i<<"*"<<fil_f<<"+"<<j<<"+"<<p<<"] = "<<fil[i * fil_f + j+p]<<endl;
        }
}
p+=fil_f*fil_f;
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

void softmax(double *in,double *out)
{
double *t,sum;
t=(double*) malloc(sizeof(double)*4*4);

for(int i=0;i<4;i++)
{
t[i]=exp(in[i]);
//cout<<"t["<<i<<"]"<<t[i]<<endl;
sum=sum + t[i];

}
for(int i=0;i<4;i++)
{//cout<<"t["<<i<<"]/sum"<<t[i]/sum<<endl;
out[i]=(t[i]/sum);
//cout<<"out["<<i<<"]"<<out[i]<<endl;
}
}

/*loss function*/
void loss_function(double *predicted,int *truth,double *MSE)
{double k;
int joo;
for(int i=0;i<4;i++)
{
k=(predicted[i]-truth[i]);
joo=pow(k, 2);
MSE+=1/4*joo;
}

}
/*back propagation*/
void back_propagation(double *b,double *out,int *target,int max_fil,int fil_f,int *fil_out,int *fil_1,int *fil_0,double *out_1,double *out_2)
{

int eta=0.01,p=0;
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
cout<<"before_for2 backprop"<<endl;

}
