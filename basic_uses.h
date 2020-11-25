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

//1��ͼ���ȡ��չʾ;����ͼƬ
//��ȡpngͼ�񣬲������±�Ե��ɫ
void read_img_and_show_png()
{
	//��ȡͼ��
	cv::Mat img = cv::imread("./imgs/1.png");
	//����ͼ�񴰿�
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	//��ʾͼ��
	cv::imshow("src", img);

	std::vector<cv::Mat> bgr;			//�洢ͼ���rgbͨ��
	cv::split(img, bgr);

	//��ʾBͨ��
	cv::namedWindow("B", CV_WINDOW_NORMAL);     //Bͨ����blue��ʾΪ��ɫ������������ʾΪ��ɫ
	cv::imshow("B", bgr[0]);

	//��ʾGͨ��
	cv::namedWindow("G", CV_WINDOW_NORMAL);
	cv::imshow("G", bgr[1]);

	//��ʾRͨ��
	cv::namedWindow("R", CV_WINDOW_NORMAL);
	cv::imshow("R", bgr[2]);

	cv::waitKey(0);
}

//��ȡjpgͼ��,�����±�Ե��ɫ
void read_img_and_show_jpg()
{
	//��ȡͼ��
	cv::Mat img = cv::imread("./imgs/2.jpg");
	//����ͼ�񴰿�
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	//��ʾͼ��
	cv::imshow("src", img);

	std::vector<cv::Mat> bgr;			//�洢ͼ���rgbͨ��
	cv::split(img, bgr);

	//��ʾBͨ��
	cv::namedWindow("B", CV_WINDOW_NORMAL);     //Bͨ����blue��ʾΪ��ɫ������������ʾΪ��ɫ
	cv::imshow("B", bgr[0]);

	//��ʾGͨ��
	cv::namedWindow("G", CV_WINDOW_NORMAL);
	cv::imshow("G", bgr[1]);

	//��ʾRͨ��
	cv::namedWindow("R", CV_WINDOW_NORMAL);
	cv::imshow("R", bgr[2]);

	cv::waitKey(0);
}

//��ȡͼ��Ŀ¼,img_pth,img_list�д洢ͼ���б�
void read_img_pth(std::string img_pth, std::vector<std::string>& img_list)
{
	for (const auto & entry : fs::directory_iterator(img_pth))
	{
		img_list.push_back(entry.path().string());
	}
}

//2��BGR to RGB
//ʹ��cv�ĺ�����cvtColor
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

//ʹ��cv��split��merge,��vector
void bgr_to_rgb_2()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	cv::namedWindow("src", CV_WINDOW_NORMAL);
	cv::imshow("src", img);

	std::vector < cv::Mat > bgr;
	cv::split(img, bgr);

	//����3��ͨ����λ��
	std::reverse(bgr.begin(), bgr.end());
	cv::Mat dst;
	cv::merge(bgr, dst);
	cv::namedWindow("dst", CV_WINDOW_NORMAL);
	cv::imshow("dst", dst);

	cv::waitKey(0);

}

//3��HWC to CHW(ע�⣺cv��ȡ���ļ��Ĵ洢��H(x)*W(y)�����Ƿ�����ҪW(x),H(y))
//ʹ��split,vector�ķ���;
void hwc_to_chw_1()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
	
	std::vector<cv::Mat> rgb;
	cv::split(img, rgb);

	int img_size = img.rows * img.cols;
	int channels = img.channels();
	//std::array<uchar, img_size*channels> dst;    //��ʱӦ��ʹ�ÿɱ䳤�ȵ�vector,�����ǹ̶����ȵ�std::array
	std::vector<uchar> dst(img_size*channels);

	//��3��ͨ�����η��뵽dst�У���3*H*W
	for (int i = 0; i < img.channels(); ++i)
	{
		//3��ͨ��
		memcpy(dst.data() + i * img_size, rgb[i].data, sizeof(uchar)*img_size);
	}

	for (int i = 0; i < 10; ++i)
	{
		std::cout << int(dst[i]) << std::endl;
	}
}
//ʹ��split��ָ��ķ�ʽ;��Ҫ����H��W
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
			x = pid / img.cols;                         //x,��������
			y = pid % img.rows;                         //y,
			dst[ch*img_size + pid] = rgb[ch].at<uchar>(x, y);    //����Ҫ���еı任
		}
	}

	//��ʾ�Ƿ�ת����ȷ
	for (int i = 0; i < 10; ++i)
	{
		std::cout << int(dst[i]) << std::endl;
	}
}

//4.����cv::Mat���޸�cv::Mat����������
//�÷�:���ݹ�һ��
void traverse_mat()
{
	cv::Mat img = cv::imread("./imgs/1.png");
	std::cout <<"ԭʼ���ݣ�"<< std::endl;
	for (int i = 0; i < img.rows; ++i)
	{
		if (i > 0)
			break;
		for (int j = 0; j < img.cols; ++j)
		{
			if (j == 10)
				break;
			std::cout << img.at<cv::Vec3b>(i, j) << std::endl;      //ԭʼͼ����uchar����
		}
	}
	std::cout << std::endl;
	//ת����������
	img.convertTo(img, CV_32FC3);

	float mean[] = { 0.5,0.5,0.5 };
	float std[] = { 0.5,0.5,0.5 };

	std::vector<cv::Mat> bgr;
	cv::split(img, bgr);
	for (int ch = 0; ch < img.channels(); ++ch)
	{
		bgr[ch] = (bgr[ch] / 255.0 - mean[ch]) / std[ch];      //ÿ��ͨ������һ�β���
	}

	cv::Mat dst;
	cv::merge(bgr, dst);
	std::cout << "��һ��������ݣ�" << std::endl;
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

//5.resize��crop , padding����


