#include "Image.h"
#include<algorithm>
/*
 * check if the header contain comments
 * comments start with # .
 */
bool isComment(string comment)
{
	for (int i = 0;i < comment.size();i++)
	{
		if (comment[i] == '#')
			return true;
		if (!isspace(comment[i]))
			return false;
	}
	return true;
}

int Image::getHeight()
{
	return height;
}

int Image::getWidth()
{
	return width;
}
int Image::getMaxPixelValue()
{
	return maxPixelValue;
}
void Image::setMaxPixelValue(int mpv)
{
	maxPixelValue = mpv;
}
void Image::setHeight(int h)
{
	height = h;
}
void Image::setWidth(int w)
{
	width = w;
}
//read binary pixels value in image
void BinaryImage::readImage(ifstream &inFile)
{
	if (!inFile)
	{
		puts("could not read from file...");
		exit(1000);
	}

	char *byteArray = new char[imageSize + 1];

	inFile.read(byteArray, imageSize);

	if (inFile.fail())
	{
		puts("Error : can not read pixels!");
		exit(1000);
	}
	byteArray[imageSize] = '\0';
	for (int i = 0;i < imageSize;i++)
		pixels.push_back((int)((char)byteArray[i]));
	delete[] byteArray;
}

void BinaryImage::writeImage(ofstream &outFile)
{
	if (!outFile)
	{
		puts("could not write to file");
		exit(1000);
	}

	outFile << "P5" << " " <<
		width << " " <<
		height << " " <<
		maxPixelValue << endl;
	char * byteArray = new char[imageSize + 1];

	for (int i = 0;i < imageSize;i++)
		byteArray[i] = (int)pixels[i];

	byteArray[imageSize] = '\0';
	outFile.write(byteArray, imageSize);

	if (outFile.fail())
	{
		puts("can not write to file...");
		exit(1001);
	}
	int pixelValue;

	for (int i = 0;i < imageSize;i++)
	{
		if (i%width == 0 && i)
			outFile << '\n';
		outFile << pixels[i] << '\t';
	}

}
void AsciiImage::readImage(ifstream &inFile)
{
	if (!inFile)
	{

		puts("Could not read from file.");
		exit(1001);
	}
	int pixelValue;
	while (inFile >> pixelValue)
		pixels.push_back(pixelValue);

}

void AsciiImage::writeImage(ofstream & outFile)
{
	if (!outFile)
	{
		puts("Could not write to file.");
		exit(1001);
	}
	outFile << "P2" << ' ' <<
		width << ' ' <<
		height << ' ' <<
		maxPixelValue << '\n';
	for (int i = 0; i < imageSize; i++)
	{
		if (i % width == 0 && i)
			outFile << '\n';
		outFile << pixels.at(i) << '\t';
	}
}
void Image::readHeader(ifstream &inFile)
{
	stringstream sStream;
	string line;
	if (!inFile)
	{
		puts("Error could not open file...");
		exit(1002);
	}

	char readChar;
	string errMessage("Error , incorrect picture format");
	getline(inFile, line);
	for (int i = 0;i < line.size();i++)
	{
		if (!isspace(line[i]))
		{
			cout << errMessage << endl;
			puts("Extra info after magic number");
			exit(1002);
		}
	}
	while (getline(inFile, line))
		if (!isComment(line))
			break;
	sStream << line;

	if (!(sStream >> width))
	{
		cout << errMessage << endl;
		puts("can not read width");
		exit(1002);
	}
	if (!(sStream >> height))
	{
		cout << errMessage << endl;
		puts("can not read height");
		exit(1002);
	}

	while (sStream >> readChar)
	{
		if (!isspace(readChar))
		{
			cout << errMessage << endl;
			puts("Extra info when reading height and width.");
			exit(1002);
		}
	}
	if (height <= 0 || width <= 0)
	{
		puts("Error width and height can not be negative.");
		exit(1002);
	}
	while (getline(inFile, line))
		if (!isComment(line))
			break;
	sStream.str("");
	sStream.clear();
	sStream << line;
	if (!(sStream >> maxPixelValue))
	{
		cout << errMessage << endl;
		puts("Extra info after max pixel value");
		exit(1002);
	}
	if (maxPixelValue < 0 || maxPixelValue>255)
	{
		cout << errMessage << endl;
		puts("invalid max pixel value");
		exit(1002);
	}
	imageSize = width*height;
}

void Image::findMax()
{
	int mx = 0;
	for (int i = 0;i < imageSize;i++)
		mx = max(pixels[i], mx);
	maxpix = mx;
}

void Image::findMin()
{
	int mn = 255;
	for (int i = 0;i < imageSize;i++)
		mn = min(mn, pixels[i]);
	minpix = mn;
}

void Image::scaleImage()
{
	double scal = 0.0;
	findMax();
	findMin();
	for (int i = 0;i < imageSize;i++)
	{
		scal = (double)(pixels[i] - minpix) / (maxpix - minpix);
		pixels[i] = round(scal * 255);
	}
	maxPixelValue = 255;
}
void Image::edgeDetection()
{
	int x, y, xG, yG;
	x = y = xG = yG = 0;
	vector<int>tempImage(imageSize);
	for (int i = 0;i < imageSize;i++)
	{
		x = i%width;
		if (i&&!x)
			++y;

		if (x < (width - 1) && y < (height - 1) && x&&y)
		{
			//index = x + (y * width)
			//Finds the horizontal gradient
			xG = (pixels[(x + 1) + ((y - 1) * width)] + (2 * pixels[(x + 1) + (y * width)]) + pixels[(x + 1) + ((y + 1) * width)]
				- pixels[(x - 1) + ((y - 1) * width)] - (2 * pixels[(x - 1) + (y * width)]) - pixels[(x - 1) + ((y + 1) * width)]);

			//Finds the vertical gradient
			yG = (pixels[(x - 1) + ((y + 1) * width)] + (2 * pixels[(x)+((y + 1) * width)]) + pixels[(x + 1) + ((y + 1) * width)]
				- pixels[(x - 1) + ((y - 1) * width)] - (2 * pixels[(x)+((y - 1) * width)]) - pixels[(x + 1) + ((y - 1) * width)]);

			//newPixel = sqrt(xG^2 + yG^2)
			tempImage[i] = sqrt((xG * xG) + (yG * yG));
		}
		else
		{
			//Pads out of bound pixels with 0
			tempImage[i] = 0;
		}
	}
	for (int i = 0;i < imageSize;i++)
		pixels[i] = tempImage[i];
}