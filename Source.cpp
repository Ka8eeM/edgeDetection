#include"Image.h"
bool isBinary(ifstream &inFile)
{
	char readChar = ' ';
	string errMessage("Error, incorrect picture format.");

	if (!(inFile >> readChar) || readChar != 'P')
	{
		cout << errMessage << endl;
		puts("P");
		exit(1002);
	}

	if (!(inFile >> readChar) || (readChar != '2'&&readChar != '5'))
	{
		cout << errMessage << endl;
		cout << readChar << endl;
		exit(1002);
	}
	if (readChar == '5')
		return true;
	return false;
}

void play(char **argv)
{
	ifstream inFile;
	inFile.open(argv[1], ios::binary | ios::in);
	ofstream outFile;
	outFile.open(argv[2], ios::binary | ios::trunc | ios::out);

	if (isBinary(inFile))
	{
		BinaryImage binImage;
		binImage.readHeader(inFile);
		binImage.readImage(inFile);
		binImage.edgeDetection();
		binImage.scaleImage();
		binImage.writeImage(outFile);
	}
	else
	{
		AsciiImage ascImage;
		ascImage.readHeader(inFile);
		ascImage.readImage(inFile);
		ascImage.edgeDetection();
		ascImage.scaleImage();
		ascImage.writeImage(outFile);
	}

	inFile.close();
	outFile.close();
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << "Usage: ImageManip imageName.pgm output.pgm\n";
		return 0;
	}
	int start, end;
	start = clock();
	play(argv);
	end = clock();
	double totalTime = (end - start) / 1000.0;
	cout << "Execution time: " << totalTime << endl;
	return 0;
}