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
cout<<"hi_beginning"<<endl;
Mat img;
int *org_o,*out,*fil__f;
img=imread(argv[1], CV_LOAD_IMAGE_COLOR);
imshow("hello",img);
unsigned char *org = (unsigned char*)(img.data);		// getting the image BGR data into char array(0->225) with 3 channels 
cout<<"hi_before_separating_b_g_r"<<endl;
int i,k,*r,*g,*b;
r=(int*) malloc(sizeof(int)*img.rows*img.cols);
b=(int*) malloc(sizeof(int)*img.rows*img.cols);
g=(int*) malloc(sizeof(int)*img.rows*img.cols);
cout<<"allocation"<<endl;
for(int i = 0;i < img.rows ;i++){
        for(int j = 0;j < img.cols ;j++){
            
	    b[img.cols * i + j] = org[img.cols * i + j ] ;			//Load mxmx3 matrix into the integer pointer with pixel values
            g[img.cols * i + j]=  org[img.cols * i + j + 1];			//After that sending it to convolution.
            r[img.cols * i + j] = org[img.cols * i + j + 2];
        }
    }
cout<<"hi"<<endl;
int *out1,*out2,*out3,fil_f=3,*trial,*trial_out,trial_s=5;
trial=new int(5*5);
trial_out=new int(3*3);
*trial_out=0;
cout<<"hi_before_filters"<<endl;
int *fil_0,*fil_1,*fil_2,*fil_3,*fil_4;
fil_0=(int*) malloc(sizeof(int)*fil_f*fil_f);
/*fil_1=(int*) malloc(sizeof(int)*fil_f*fil_f);
fil_2=(int*) malloc(sizeof(int)*fil_f*fil_f);
fil_3=(int*) malloc(sizeof(int)*fil_f*fil_f);
fil_4=(int*) malloc(sizeof(int)*fil_f*fil_f);*/
filter(&fil_f,fil_0);
cout<<"fil_0done-1"<<endl;
filter(&trial_s,trial);
cout<<"trial_done"<<endl;
for(int i=0;i<25;i++)
{
cout<<"trial["<<i<<"]"<<trial[i]<<endl;
}
convolution(5, trial,trial_out,fil_f,fil_0,0,1);
cout<<"conv-done"<<endl;
for(int i=0;i<9;i++)
{
cout<<"trial_out["<<i<<"]"<<trial_out[i]<<endl;
}
/*filter(&fil_f,fil_1);
cout<<"done-2"<<endl;
filter(&fil_f,fil_2);
cout<<"done-3"<<endl;
filter(&fil_f,fil_3);
cout<<"done-4"<<endl;
filter(&fil_f,fil_4);
cout<<"done-5"<<endl;
cout<<"hi_before_convolution"<<endl;
int out_row=((img.rows+((2*0)-fil_f))/1)+1;
out1=new int(out_row*out_row);
out2=new int(out_row*out_row);
out3=new int(out_row*out_row);
convolution(img.rows, b,out1,fil_f,fil_0,0,1);
convolution(img.rows, g,out2,fil_f,fil_0,0,1);
convolution(img.rows, r,out3,fil_f,fil_0,0,1);*/

waitKey(0);

return 0;
}





/*First layer of CNN has Convlution,striding and activation function(tanh)*/

void convolution(int org_n,int *org,int *out,int fil_f,int *fil,int pad,int str) //convolution(img.rows, *b,*out1,fil_f,*fil_0,0,0);
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
for(int l=0;l<o_row;l++)
{
for(int k=0;k<o_col;k++)
 {out[l*o_col+k]=0;
for (int i = 0; i < f_row; i++) 
  {
        for (int j = 0; j < f_col; j++) 
	{	
				
            out[l*o_col+k]+= org[i *org_n +org_col+j]*fil[i * f_col + j] ;        //Convolution taking place-Extending it for mxmx3(looked upon)
		

        }
  }
//out[l*o_col+k]++;									//adding bias
//out[l*o_col+k]=tanh (out[l*o_col+k]);							//Using Acivation function
//if((org_col+fil_f)%org_n)>=0){org_col=org_col+fil_f;}
org_col+=str;										//striding
 }
org_col--;
org_col=org_col+fil_f;									//shifting to next row
 
}

}

void filter(int *size,int *fil)
{
int fil_f=*size;
 for (int i = 0; i <fil_f; ++i) {
        for (int j = 0; j < fil_f; ++j) {
            fil[i * fil_f + j] = rand()%5 ;
	    cout<<"fil["<<i<<"*"<<fil_f<<"+"<<j<<"] = "<<fil[i * fil_f + j]<<endl;
        }
}

}
/*full connected layer... pending*/
/*softmax...pending*/
/*back propagation...pending*/
