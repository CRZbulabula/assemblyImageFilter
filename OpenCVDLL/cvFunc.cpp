#include "cvFunc.h"


void greyImage(char* inputPath, char* outputPath)   //self
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float avg = 0.3 * R + 0.59 * G + 0.11 * B;
			if (avg > 255) avg = 255;
			else if (avg < 0) avg = 0;
			P1[3 * x] = avg;
			P1[3 * x + 1] = avg;
			P1[3 * x + 2] = avg;
		}

	}
	//imshow("黑白", img);
	imwrite(outputPath, img);
	waitKey(0);
}

void oldImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 0; y < heigh; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			float B = P0[3 * x];
			float G = P0[3 * x + 1];
			float R = P0[3 * x + 2];
			float newB = 0.272 * R + 0.534 * G + 0.131 * B;
			float newG = 0.349 * R + 0.686 * G + 0.168 * B;
			float newR = 0.393 * R + 0.769 * G + 0.189 * B;
			if (newB < 0)newB = 0;
			if (newB > 255)newB = 255;
			if (newG < 0)newG = 0;
			if (newG > 255)newG = 255;
			if (newR < 0)newR = 0;
			if (newR > 255)newR = 255;
			P1[3 * x] = (uchar)newB;
			P1[3 * x + 1] = (uchar)newG;
			P1[3 * x + 2] = (uchar)newR;
		}

	}
	//imshow("怀旧色", img);
	imwrite(outputPath, img);
	waitKey(0);
}

void mblImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	RNG rng;
	Mat img(src.size(), CV_8UC3);
	for (int y = 1; y < heigh - 1; y++)
	{
		uchar* P0 = src.ptr<uchar>(y);
		uchar* P1 = img.ptr<uchar>(y);
		for (int x = 1; x < width - 1; x++)
		{
			int tmp = rng.uniform(0, 9);
			P1[3 * x] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3));
			P1[3 * x + 1] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 1);
			P1[3 * x + 2] = src.at<uchar>(y - 1 + tmp / 3, 3 * (x - 1 + tmp % 3) + 2);
		}

	}
	//imshow("扩散", img);
	imwrite(outputPath, img);
	waitKey(0);
}

void dkImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	Mat img(src.size(), CV_8UC3);
	for (int y = 1; y < src.rows - 1; y++)
	{
		uchar* p0 = src.ptr<uchar>(y);
		uchar* p1 = src.ptr<uchar>(y + 1);

		uchar* q1 = img.ptr<uchar>(y);
		for (int x = 1; x < src.cols - 1; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				int tmp1 = p0[3 * (x - 1) + i] - p1[3 * (x + 1) + i] + 128;//雕刻
				if (tmp1 < 0)
					q1[3 * x + i] = 0;
				else if (tmp1 > 255)
					q1[3 * x + i] = 255;
				else
					q1[3 * x + i] = tmp1;
			}
		}
	}
	imwrite(outputPath, img);
	//imshow("src", src);
	//imshow("雕刻", img);
	waitKey(0);
}

void fdImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	Mat img(src.size(), CV_8UC3);
	
	for (int y = 1; y < src.rows - 1; y++)
	{
		uchar* p0 = src.ptr<uchar>(y);
		uchar* p1 = src.ptr<uchar>(y + 1);

		uchar* q0 = img.ptr<uchar>(y);
		for (int x = 1; x < src.cols - 1; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				int tmp0 = p1[3 * (x + 1) + i] - p0[3 * (x - 1) + i] + 128;//浮雕
				if (tmp0 < 0)
					q0[3 * x + i] = 0;
				else if (tmp0 > 255)
					q0[3 * x + i] = 255;
				else
					q0[3 * x + i] = tmp0;
			}
		}
	}
	imwrite(outputPath, img);
	//imshow("src", src);
	//imshow("浮雕", img);
	waitKey(0);

}

void smImage(char* inputPath, char* outputPath)
{
	Mat src = imread(inputPath);
	int width = src.cols;
	int heigh = src.rows;
	Mat gray0, gray1;
	//imshow("src", src);
	//去色
	cvtColor(src, gray0, CV_BGR2GRAY);
	//反色
	addWeighted(gray0, -1, NULL, 0, 255, gray1);
	//高斯模糊,高斯核的Size与最后的效果有关
	GaussianBlur(gray1, gray1, Size(11, 11), 0);

	//融合：颜色减淡
	Mat img(gray1.size(), CV_8UC1);
	for (int y = 0; y < heigh; y++)
	{

		uchar* P0 = gray0.ptr<uchar>(y);
		uchar* P1 = gray1.ptr<uchar>(y);
		uchar* P = img.ptr<uchar>(y);
		for (int x = 0; x < width; x++)
		{
			int tmp0 = P0[x];
			int tmp1 = P1[x];
			P[x] = (uchar)min((tmp0 + (tmp0 * tmp1) / (256 - tmp1)), 255);
		}

	}
	imwrite(outputPath, img);
	//imshow("素描", img);
	waitKey(0);
}

void openCamera()
{
	VideoCapture capture(0);

	//namedWindow("empty", CV_WINDOW_NORMAL);
	//resizeWindow("empty", 10, 10);
	//moveWindow("empty", 300, 40);
	namedWindow("camera", CV_WINDOW_NORMAL);
	moveWindow("camera", 300, 40);

	while (true)
	{
		Mat frame;
		capture >> frame;
		imshow("camera", frame);
		moveWindow("camera", 300, 400);
		imwrite("images/Video.png", frame);
		waitKey(300);	//延时30
		//remove("images/Video.png");
	}
}

void captureFrame()
{
	VideoCapture capture(0);
	Mat frame;
	capture >> frame;
	imwrite("images/Video.png", frame);
}

int main()
{
	char p1[] = "D:/test6.png";
	char p2[] = "D:/111.jpg";
	greyImage(p1, p2);
	return 0;
}