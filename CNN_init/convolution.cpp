#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <cctype>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;
int const max_lowThreshold = 100;
Mat convlinear(Mat &org); 
Mat can(Mat &org); 

int main( int argc, const char** argv )
{

Mat input,output,out1;

input= imread(argv[1], CV_LOAD_IMAGE_COLOR);
imshow("input",input);
 	
output=convlinear(input);

imshow("Result",output);
cout<<"Towards function"<<endl;
out1=can(input);
cout<<"outside function"<<endl;
imwrite("canny_output.png",out1);
waitKey(0);
}




/**************Fuctions**************/

          /*Convolution with linear filter*/

Mat convlinear(Mat &org)
{
Mat outpt;
 Mat kernel;
  Point anchor;
  double delta;
  int ddepth;
  int kernel_size;
  namedWindow( "filter2D Demo", CV_WINDOW_AUTOSIZE );
anchor = Point( -1, -1 );
  delta = 0;
  ddepth = -100;
int ind = 0;

int c;
  while(true )
    {
      c = waitKey(500);
      /// Press 'ESC' to exit the program
      if( (char)c == 27 )
        { break; }

      /// Update kernel size for a normalized box filter
      kernel_size = 3 + 2*( ind%5 );
      kernel = Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);

      /// Apply filter
      filter2D(org, outpt, ddepth , kernel, anchor, delta, BORDER_DEFAULT );
      imshow( "filter2D Demo", outpt );
      ind++;
    }
return(outpt);
}

   /**canny edge detector**/


Mat can(Mat &org)
{

Mat output,org_gray;

namedWindow( "threshold", CV_WINDOW_AUTOSIZE );

cout<<"inside the function"<<endl;
cvtColor( org, org_gray, CV_BGR2GRAY );

Canny( org_gray, output, 50, 53, 3 );
imshow( "Canny Output",  output );
return( output);

}
