#pragma once
#ifndef IMAGE_H_
#define IMAGE_H_

#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>
#include<time.h>
#include<sstream>

using namespace std;


class Image
{
public:
	Image() :
		height(0),
		width(0),
		maxPixelValue(0),
		minpix(0),
		maxpix(0),
		imageSize(0) {}

	virtual ~Image() {}
	virtual void readImage(ifstream &inFile)  = 0;
	virtual void writeImage(ofstream &outFile) = 0;

	void readHeader(ifstream  &inFile);
	void scaleImage();
	void edgeDetection();
	int getHeight();
	int getWidth();
	int getMaxPixelValue();

	void setHeight(int h);
	void setWidth(int w);
	void setMaxPixelValue(int mpv);

	void findMin();
	void findMax();
protected:
	int height;
	int width;
	int maxPixelValue;
	int minpix;
	int maxpix;
	int imageSize;
	vector<int>pixels;
};
class BinaryImage :public Image
{
public:
	BinaryImage() {}
	~BinaryImage() {}
	void readImage(ifstream &inFile);
	void writeImage(ofstream &outFile);
};
class AsciiImage :public Image
{
public:
	AsciiImage() {}
	~AsciiImage() {}
	void readImage(ifstream &inFile);
	void writeImage(ofstream &outFile);
};
#endif /* IMAGE_H_ */