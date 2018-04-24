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
 VideoCapture cap(0);
  namedWindow("video",1);
 int j=0;


Mat frame,red;
cap>>frame;
red=imread(argv[1], CV_LOAD_IMAGE_COLOR);
imshow("hello",frame);
imshow( "Display window", red );

imwrite("snap2.png",frame);
waitKey(0);

}
