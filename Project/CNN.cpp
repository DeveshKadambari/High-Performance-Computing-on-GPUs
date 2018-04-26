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

using namespace std;
using namespace cv;

/*functions*/
void convolution(int org_n,int *org,int *out,int fil_f,int *fil,int pad,int str);
void filter(int *size,int *fil);


/*main thread*/
int main( int argc, const char** argv )
{
Mat img;
int *org_o,*out,*fil__f;
img=imread(argv[1], CV_LOAD_IMAGE_COLOR);
imshow("hello",img);
unsigned char *org = (unsigned char*)(img.data);		// getting the image BGR data into char array(0->225) with 3 channels 

int i,j,*r,*g,*b;
for(int i = 0;i < img.rows ;i++){
        for(int j = 0;j < img.cols ;j++){
            b[img.step * j + i] = org[img.step * j + i ] ;			//Load mxmx3 matrix into the integer pointer with pixel values
            g[img.step * j + i]=  org[img.step * j + i + 1];			//After that sending it to convolution.
            r[img.step * j + i] = org[img.step * j + i + 2];
        }
    }
int *out1,fil_f=3;

int *fil_0,*fil_1,*fil_2,*fil_3,*fil_4;
filter(&fil_f,fil_0);
cout<<"done-1"<<endl;
filter(&fil_f,fil_1);
cout<<"done-2"<<endl;
filter(&fil_f,fil_2);
cout<<"done-3"<<endl;
filter(&fil_f,fil_3);
cout<<"done-4"<<endl;
filter(&fil_f,fil_4);
cout<<"done-5"<<endl;

convolution(img.rows, b,out1,fil_f,fil_0,0,0);
convolution(img.rows, g,out1,fil_f,fil_0,0,0);
convolution(img.rows, r,out1,fil_f,fil_0,0,0);

waitKey(0);

return 0;
}





/*First layer of CNN has Convlution,striding and activation function(tanh)*/

void convolution(int org_n,int *org,int *out,int fil_f,int *fil,int pad,int str) //convolution(img.rows, *b,*out1,fil_f,*fil_0,0,0);
{
int incstr=str,o_row=((org_n+(2*pad)-fil_f)/str)+1, o_col=o_row, f_row=fil_f,f_col=fil_f;
int org_col=0;
for(int l=0;l<o_row;l++)
{
for(int k=0;k<o_col;k++)
 {
for (int i = 0; i < f_row; ++i) 
  {
        for (int j = 0; j < f_col; ++j) 
	{
            out[l*o_col+k]+= org[i *org_n +org_col+j]*fil[i * f_col + j] ;                //Convolution taking place-Extending it for mxmx3
        }
  }
out[l*o_col+k]++;									//adding bias
out[l*o_col+k]=tanh (out[l*o_col+k]);							//Using Acivation function
//if((org_col+fil_f)%org_n)>=0){org_col=org_col+fil_f;}
org_col+=str;										//striding
 }
org_col=org_col+fil_f;									//shifting to next row
 
}

}

void filter(int *size,int *fil)
{
int fil_f=*size;
 fil=(int*) malloc(sizeof(int)*fil_f*fil_f);
 for (int i = 0; i <fil_f; ++i) {
        for (int j = 0; j < fil_f; ++j) {
            fil[i * fil_f + j] = 2*rand()/32767;
	    cout<<"fil["<<i<<"*"<<fil_f<<"+"<<j<<"] = "<<fil[i * fil_f + j]<<endl;
        }
}

}
/*full connected layer... pending*/
/*softmax...pending*/
/*back propagation...pending*/
