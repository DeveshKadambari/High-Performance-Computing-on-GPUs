#include<stdio.h>
#include <math.h> 
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;

int main( int argc, const char** argv )
{
Mat img;
int *org,*out,*fil;
img=imread(argv[1], CV_LOAD_IMAGE_COLOR);
imshow("hello",img);
unsigned char *org = (unsigned char*)(img.data);		// getting the image BGR data into char array(0->225) with 3 channels 

waitKey(0);
	

}





/*First layer of CNN has Convlution,striding and activation function(tanh)*/

void convolution(int org_n,char *org,int *out,int fil_f,int *fil,int pad,int str)
{
int incstr=str,int o_row=((org_n+(2*pad)-fil_f)/str)+1,int o_col=o_row,int f_row=f_col;
int org_col=0;
for(int l=0;l<o_row;l++)
{
for(int k=0;k<o_col;k++)
 {
for (int i = 0; i < f_row; ++i) 
  {
        for (int j = 0; j < f_col; ++j) 
	{
            out[l*o_col+k]+= org[i *org_n +org_col+j]*fil[i * f_col + j] ;                //Convolution taking place
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
