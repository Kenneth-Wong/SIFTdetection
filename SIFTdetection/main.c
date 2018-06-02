#include "io.h"
#include "cvtools.h"
#include<string.h>
#include "ds.h"
#include "sift.h"
#include <stdbool.h>

#define MAX_NUM 300000


int main(int argc, char* argv[])
{
	uchar color[7][3] = { { 255, 0, 0 }, { 255, 0, 255 }, { 0, 127, 255 },
	{ 11, 134, 184 }, { 0, 255, 140 }, { 237, 149, 100 }, { 255, 255, 0 } };
	uchar pt_color[3] = { 0, 255, 255 };

	ClImage* img1 = clLoadImage("1.bmp");
	KeyPoint* keypoints1 = (KeyPoint*)malloc(sizeof(KeyPoint)*MAX_NUM);
	Descriptor* descp1 = detect(img1, keypoints1, false);

	ClImage* img2 = clLoadImage("2.bmp");
	KeyPoint* keypoints2 = (KeyPoint*)malloc(sizeof(KeyPoint)*MAX_NUM);
	Descriptor* descp2 = detect(img2, keypoints2, false);

	write(keypoints1, descp1, "pt1.txt", "descp1.txt");
	write(keypoints2, descp2, "pt2.txt", "descp2.txt");

	ClImage* cmb_img = combine(img1, img2);
	int matchp[5000][2];	
	int len = match(img1, img2, descp1, descp2, keypoints1, keypoints2, matchp);
	printf("Get %d matched points.\n", len);

	int i;
	for (i = 0; i < len; i++)
	{
		Point2d p1 = { (int)(keypoints1[matchp[i][0]].pt.x), (int)(keypoints1[matchp[i][0]].pt.y) };
		draw(cmb_img, p1, pt_color);
		Point2d p2 = { (int)(keypoints2[matchp[i][1]].pt.x+img1->width), (int)(keypoints2[matchp[i][1]].pt.y) };
		draw(cmb_img, p2, pt_color);
		drawLine(cmb_img, p1, p2, color[i % 7]);
	}
	clSaveImage("match.bmp", cmb_img);

	for (i = 0; i < descp1->height; i++)
	{
		Point2d p = { (int)(keypoints1[i].pt.x), (int)(keypoints1[i].pt.y) };
		draw(img1, p, pt_color);
	}
	clSaveImage("detectpt1.bmp", img1);

	for (i = 0; i < descp2->height; i++)
	{
		Point2d p = { (int)(keypoints2[i].pt.x), (int)(keypoints2[i].pt.y) };
		draw(img2, p, pt_color);
	}
	clSaveImage("detectpt2.bmp", img2);

	system("pause");
}