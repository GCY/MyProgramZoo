/*-----------------------------------------------------------------------------
/
/
/----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "DebugPrintf.h"
#include "OpenCV_util.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
/*---------------------------------------------------------------------------*/
using namespace std;
using namespace cv;
/*---------------------------------------------------------------------------*/
uint8_t saturated_add(uint8_t val1, int8_t val2);
uint8_t binary(uint8_t val1, int8_t threshold);
/*---------------------------------------------------------------------------*/



void MeanFilter_my1(const Mat &src, Mat &dst, int ksize)  //均值滤波   Scr 为要处理的图像，dst为目标图像，Ksize为卷积核的尺寸，卷积核的尺寸一般为 奇数
{
	CV_Assert(ksize % 2 == 1);                    // 不满足这个条件，则返回一个错误。           
 
	int *kernel = new int[ksize*ksize];           // 卷积核的大小
	for (int i = 0; i < ksize*ksize; i++)         // 均值滤波所以都为1
		kernel[i] = 1;
	Mat tmp;
	int len = ksize / 2;                         
	tmp.create(Size(src.cols + len, src.rows + len), src.type()); //添加边框
	dst.create(Size(src.cols, src.rows), src.type());
 
	int channel = src.channels();
	uchar *ps = src.data;
	uchar *pt = tmp.data;
 
	//添加边框是为了让图片周围的ksize/2 的像素都能进行均值滤波，若Ksize为3，,若是图片左上角的那个元素进行均值滤波，其实求的平均是 三个数(右、下、右下)的平均值。
 
	for (int row = 0; row < tmp.rows; row++)//添加边框的过程
	{
		for (int col = 0; col < tmp.cols; col++)
		{
			for (int c = 0; c < channel; c++)
			{
				if (row >= len && row < tmp.rows - len && col >= len && col < tmp.cols - len)
					pt[(tmp.cols * row + col)*channel + c] = ps[(src.cols * (row - len) + col - len) * channel + c];
				else
					pt[(tmp.cols * row + col)*channel + c] = 0;
			}
		}
	}
 
 
	uchar *pd = dst.data;
	pt = tmp.data;
	for (int row = len; row < tmp.rows - len; row++)//卷积的过程
	{
		for (int col = len; col < tmp.cols - len; col++)
		{
			for (int c = 0; c < channel; c++)
			{
				short t = 0;
				for (int x = -len; x <= len; x++)
				{
					for (int y = -len; y <= len; y++)
					{
						t += kernel[(len + x) * ksize + y + len] * pt[((row + x) * tmp.cols + col + y) * channel + c];
					}
				}
				pd[(dst.cols * (row - len) + col - len) * channel + c] = saturate_cast<ushort> (t / (ksize*ksize));//防止数据溢出ushort是16为数据
			}
		}
	}
	delete[] kernel;       // 释放 new 的卷积和空间
}

int main( int argc, char** argv )
{    
  Mat rawImg;
  Mat dithImg;
  int imgWidth;
  int imgHeight;
  uint8_t** imgPtr;

  if(argc != 2)
  {
    printf("> Usage: ./dithering [path_to_image]\n");
    return -1;
  }

  /* Load the image and convert to grayscale first */
  rawImg = imread(argv[1]);    
  cvtColor(rawImg, dithImg, cv::COLOR_BGR2GRAY);  

  /* Get the size info */
  imgWidth = dithImg.cols;
  imgHeight = dithImg.rows;
  dbg("> Image width: %d\r\n",imgWidth);
  dbg("> Image height: %d\r\n",imgHeight);

  /* Run the 'Floyd-Steinberg' dithering algorithm ... */  
  int err;
  int8_t a,b,c,d;
  
  for(int i=0; i<imgHeight; i++)
  {      
    for(int j=0; j<imgWidth; j++)
    {
      if(dithImg.at<uint8_t>(i,j) > 127)
      {
        err = dithImg.at<uint8_t>(i,j) - 255;
        dithImg.at<uint8_t>(i,j) = 255;
      }
      else
      {
        err = dithImg.at<uint8_t>(i,j) - 0;
        dithImg.at<uint8_t>(i,j) = 0;
      }

      a = (err * 7) / 16;
      b = (err * 1) / 16;
      c = (err * 5) / 16;
      d = (err * 3) / 16;
      
      if((i != (imgHeight-1)) && (j != 0) && (j != (imgWidth - 1)))
      {
        dithImg.at<uint8_t>(i+0,j+1) = saturated_add(dithImg.at<uint8_t>(i+0,j+1),a);
        dithImg.at<uint8_t>(i+1,j+1) = saturated_add(dithImg.at<uint8_t>(i+1,j+1),b);
        dithImg.at<uint8_t>(i+1,j+0) = saturated_add(dithImg.at<uint8_t>(i+1,j+0),c);
        dithImg.at<uint8_t>(i+1,j-1) = saturated_add(dithImg.at<uint8_t>(i+1,j-1),d);
      }
    } 
  }

    cv::Mat dstImage = cv::Mat::zeros(imgWidth,imgHeight,CV_8UC1);
 /*
    int k_size = 3;
    int p = floor(k_size / 2);
 
    //filtering
    double v = 0;
    int *vs = new int[k_size * k_size];  //int vs[9];
    int count = 0;
 
    for (int j = 0; j < imgHeight;j++)
    {
        for (int i = 0 ; i < imgWidth ; i++)
        {
            for (int c = 0; c<3; c++)
            {
                v = 0;
                count = 0;
 
                //初始化一个3*3的kernel
                for(int i = 0;i < k_size*k_size;i++)
                {
                    vs[i] = 999;
                }
 
                //将每一个像素anchor点，取其3*3的像素值，储存在vs[3*3]中
                for(int _j = -p;_j < p+1;_j++)
                {
                    for (int _i = -p; _i < p+1;_i++)
                    {
                        //当此像素anchor不为图像的第一个像素点时
                        if(((j + _j) >= 0)&&((i + _i) >=0))
                        {
                            vs[count++] = (int)dithImg.at<cv::Vec3b>(j + _j,i + _i)[c];
                        }
                    }
                }
 
                //将3*3的数组，按升序进行排序sort(first,last)
                std::sort(vs,vs + (k_size * k_size));
                //取其中值，定义为该像素的灰度值
                dstImage.at<cv::Vec3b>(j,i)[c] = (uchar)vs[int(floor(count/2)) + 1];
            }
        }
    }
*/
    MeanFilter_my1(dithImg,dstImage,5);

    for (int j = 0; j < imgWidth;j++)
    {
        for (int i = 0 ; i < imgHeight; i++)
        {
	   dstImage.at<uint8_t>(i,j) = binary(dstImage.at<uint8_t>(i,j),127);
	}
    }


  /* Show results. */
  imshow("Raw Image",rawImg);
  imshow("Dithered Image",dithImg);  
  imshow("blur_img Image",dstImage);
  printf("> Press any key to exit ...\n");

  waitKey(0);  
  return 0;
}
/*---------------------------------------------------------------------------*/
uint8_t saturated_add(uint8_t val1, int8_t val2)
{
  int16_t val1_int = val1;
  int16_t val2_int = val2;
  int16_t tmp = val1_int + val2_int;

  if(tmp > 255)
  {
    return 255;
  }
  else if(tmp < 0)
  {   
    return 0;
  }
  else
  {
    return tmp;
  }
}
/*---------------------------------------------------------------------------*/

uint8_t binary(uint8_t val1, int8_t threshold)
{
   return ((val1>threshold)?255:0);
}

