#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>   //std::reverse
#include<filesystem>   

#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>    //CV_WINDOW_NORMAL

#pragma comment(lib, "opencv_world450d.lib")

namespace fs = std::experimental::filesystem;

//1、图像读取及展示;单张图片
//读取png图像，不会留下边缘颜色
void read_img_and_show_png()
{
	//读取图像
	cv::Mat img = cv::imread("./imgs/1.png");
	//构造图像窗口
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	//显示图像
	cv::imshow("src", img);

	std::vector<cv::Mat> bgr;			//存储图像的rgb通道
	cv::split(img, bgr);

	//显示B通道
	cv::namedWindow("B", CV_WINDOW_NORMAL);     //B通道：blue显示为白色，其他两个显示为黑色
	cv::imshow("B", bgr[0]);

	//显示G通道
	cv::namedWindow("G", CV_WINDOW_NORMAL);
	cv::imshow("G", bgr[1]);

	//显示R通道
	cv::namedWindow("R", CV_WINDOW_NORMAL);
	cv::imshow("R", bgr[2]);

	cv::waitKey(0);
}

//读取jpg图像,会留下边缘颜色
void read_img_and_show_jpg()
{
	//读取图像
	cv::Mat img = cv::imread("./imgs/2.jpg");
	//构造图像窗口
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	//显示图像
	cv::imshow("src", img);

	std::vector<cv::Mat> bgr;			//存储图像的rgb通道
	cv::split(img, bgr);

	//显示B通道
	cv::namedWindow("B", CV_WINDOW_NORMAL);     //B通道：blue显示为白色，其他两个显示为黑色
	cv::imshow("B", bgr[0]);

	//显示G通道
	cv::namedWindow("G", CV_WINDOW_NORMAL);
	cv::imshow("G", bgr[1]);

	//显示R通道
	cv::namedWindow("R", CV_WINDOW_NORMAL);
	cv::imshow("R", bgr[2]);

	cv::waitKey(0);
}

//读取图像目录,img_pth,img_list中存储图像列表
void read_img_pth(std::string img_pth, std::vector<std::string>& img_list)
{
	for (const auto & entry : fs::directory_iterator(img_pth))
	{
		img_list.push_back(entry.path().string());
	}
}

//2、BGR to RGB
//使用cv的函数，cvtColor
void bgr_to_rgb_1()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	cv::imshow("src", img);

	//BRG to RGB
	cv::Mat dst;
	cv::cvtColor(img, dst, cv::COLOR_BGR2RGB);
	cv::namedWindow("dst", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);

	cv::waitKey(0);
}

//使用cv的split，merge,及vector
void bgr_to_rgb_2()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	cv::imshow("src", img);

	std::vector < cv::Mat > bgr;
	cv::split(img, bgr);

	//调整3个通道的位置
	std::reverse(bgr.begin(), bgr.end());
	cv::Mat dst;
	cv::merge(bgr, dst);
	cv::namedWindow("dst", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);

	cv::waitKey(0);

}

//3、HWC to CHW(注意：cv读取后文件的存储是H(x)*W(y)，但是访问需要W(x),H(y))
//使用split,vector的方法;
void hwc_to_chw_1()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	
	std::vector<cv::Mat> rgb;
	cv::split(img, rgb);

	int img_size = img.rows * img.cols;
	int channels = img.channels();
	//std::array<uchar, img_size*channels> dst;    //此时应该使用可变长度的vector,而不是固定长度的std::array
	std::vector<uchar> dst(img_size*channels);

	//将3个通道依次放入到dst中，及3*H*W
	for (int i = 0; i < img.channels(); ++i)
	{
		//3个通道
		memcpy(dst.data() + i * img_size, rgb[i].data, sizeof(uchar)*img_size);
	}

	for (int i = 0; i < 10; ++i)
	{
		std::cout << int(dst[i]) << std::endl;
	}
}
//使用split和指针的方式;需要考虑H和W
void hwc_to_chw_2()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

	std::vector<cv::Mat> rgb;
	cv::split(img, rgb);

	int img_size = img.rows*img.cols;
	//std::array<uchar, img_size*img.channels> dst;
	std::vector<uchar> dst(img_size*img.channels());


	for (int ch = 0; ch < img.channels(); ++ch)
	{
		int x, y;
		for (int pid = 0; pid < img_size; ++pid)
		{
			x = pid / img.cols;                         //x,除以行数
			y = pid % img.rows;                         //y,
			dst[ch*img_size + pid] = rgb[ch].at<uchar>(x, y);    //不需要行列的变换
		}
	}

	//显示是否转化正确
	for (int i = 0; i < 10; ++i)
	{
		std::cout << int(dst[i]) << std::endl;
	}
}

//4.遍历cv::Mat及修改cv::Mat的数据类型
//用法:数据归一化
void traverse_mat()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	std::cout <<"原始数据："<< std::endl;
	for (int i = 0; i < img.rows; ++i)
	{
		if (i > 0)
			break;
		for (int j = 0; j < img.cols; ++j)
		{
			if (j == 10)
				break;
			std::cout << img.at<cv::Vec3b>(i, j) << std::endl;      //原始图像是uchar类型
		}
	}
	std::cout << std::endl;
	//转换数据类型
	img.convertTo(img, CV_32FC3);

	float mean[] = { 0.5,0.5,0.5 };
	float std[] = { 0.5,0.5,0.5 };

	std::vector<cv::Mat> bgr;
	cv::split(img, bgr);
	for (int ch = 0; ch < img.channels(); ++ch)
	{
		bgr[ch] = (bgr[ch] / 255.0 - mean[ch]) / std[ch];      //每个通道进行一次操作
	}

	cv::Mat dst;
	cv::merge(bgr, dst);
	std::cout << "归一化后的数据：" << std::endl;
	for (int i = 0; i < dst.rows; ++i)
	{
		if (i > 0)
			break;
		for (int j = 0; j < dst.cols; ++j)
		{
			if (j > 10)
				break;
			std::cout << dst.at<cv::Vec3f>(i, j) << std::endl;
		}
	}
}

//5.resize，crop , padding数据


