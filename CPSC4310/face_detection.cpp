/*
Program: face_detection
Usage:	./face_detection
Author:	William Beninger 001159518
Date: 	2015-04-18
Description: This program will begin by attempting to create a database either from images stored in ./database/ or
	from a previously saved database file. Additional images can be added to expand the database or it can simply 
	be used in its present state. Afterwards, the program will request a .ppm image file for input comparison.
	After one is submitted, it is histogram equalized; LBP; and spatial histogram comparison. When faces are detected 
	the program will draw a square around them to identify them and this final image will be saved to the folder.
*/

//For all pam functions/types
#include <pam.h>
//Printing to the cmd line for errors
#include <iostream>
//sine/cos/tan/min/max/ceil/floor
#include <math.h>
//vector
#include <vector>
//directories
#include <dirent.h>
//ostringstream
#include <sstream>
//string
#include <string>
//filestream
#include <fstream>

using namespace std;

//necessary constants
const double PI = atan2(0,-1);
const int MAX_INTENSITY = 256;
const int MAX_ANGLE = 360;
const int BUFFER_SIZE = 1024;
const int NUM_RECT = 3;
const double T = 0.7; //threshold for face detection
const int RECT_SIZE = 32; //size of template

//Structure for moving around points
struct point
{
	long x;
	long y;
	//Constructor
	point(const long &x1, const long &y1)
	{
		x=x1;
		y=y1;
	}
	
	point()
	{
		x=0;
		y=0;
	}
	
	//Equality Check
	bool operator ==(const point &b)
	{
		if (this->x == b.x && this->y == b.y)
			return true;
		return false;
	}
};

//face structure to hold where the origin of the face is and how large the box containing it is
struct face
{
	point first;
	long second;

	face(const point &a, const long &b):first(a),second(b)
	{}

	face(const face &a)
	{
		first.x = a.first.x;
		first.y = a.first.y;
		second = a.second;
	}
	
	face& operator=(const face &a)
	{
		if (this != &a)
		{
			this->first.x = a.first.x;
			this->first.y = a.first.y;
			this->second = a.second;
		}
		return *this;
	}

	//function to quickly detect if two squares are overlapping.
	bool overlaps(face b)
	{
		//optional and used to leave the different sized squares alone
		if (this->size() != b.size())
			return false;

		long aX1 = this->x()*this->size();
		long aX2 = this->x()*this->size()+(RECT_SIZE/this->size());
		long bX1 = b.x()*b.size();
		long bX2 = b.x()*b.size()+(RECT_SIZE/this->size());
		
		long aY1 = this->y()*this->size();
		long aY2 = this->y()*this->size()+(RECT_SIZE/this->size());
		long bY1 = b.y()*b.size();
		long bY2 = b.y()*b.size()+(RECT_SIZE/b.size());

		if(aX1<bX2 && aX2>bX1 && aY1<bY2 && aY2>bY1)
		{
			return true;
		}
		else
			return false;
	}

	long x() {return first.x;}
	long y() {return first.y;}
	long size() {return second;}
};

//structure to hold the 5 measurements
struct colourSpace
{
	long y, r, g, b, angle;
	
	colourSpace(long r1, long g1, long b1):r(r1),g(g1),b(b1)
	{
		//automated yuv conversion
		y = round(0.299*r1+0.587*g1+0.114*b1);
		double u = -0.147*r1-0.289*g1+0.436*b1;
		double v = 0.615*r1-0.515*g1-0.100*b1;
		angle = round(atan2(v,u)*180/PI);
		while (angle<0)
			angle+=360;
	}

	colourSpace()
	{
		y=r=g=b=angle=0;
	}

	long& operator [](int i)
	{
		switch (i)
		{
			case 0:
				return y;
				break;
			case 1:
				return r;
				break;
			case 2:
				return g;
				break;
			case 3:
				return b;
				break;
			case 4:
				return angle;
				break;
			default:
				return y; //wraparound to get rid of warning.
		}
	}
	const long& operator [](long i) const
	{
		switch (i)
		{
			case 0:
				return y;
				break;
			case 1:
				return r;
				break;
			case 2:
				return g;
				break;
			case 3:
				return b;
				break;
			case 4:
				return angle;
				break;
			default:
				return y; //wraparound to get rid of warning.
		}
	}
};

template <typename T>
	string NTS (T number)
	{
		ostringstream ss;
		ss << number;
		return ss.str();
	}


//Function declarations
tuple **read_image(char *filename, pam &inpam);
void write_image(char *filename, const pam &inpam, tuple **array);
colourSpace **convert_spaces(const pam &inpam, tuple **array);
void normalize(const pam &inpam, colourSpace **array);
void copyRGBtoRGB(const pam &inpam, tuple **rgbArray, colourSpace **clrArray);
void lbpOperation(const pam &inpam, colourSpace **array);
bool contains(const point a, const int length, const int width);
point closest(point &a, point b[], const int &size, const int &length, const int &width);
long *returnHistogram(colourSpace **image, const int &rectNum, const int &dim, long startx=0, long starty=0);
long histDiff(long *a, long *b);
long **createDatabase();
long **loadDatabase();
bool calcFreq(long **database, colourSpace **image, long startx=0, long starty=0);
vector<face> findFaces(long **database, colourSpace **image, const pam &inpam);
colourSpace **shrinkImage(colourSpace **image, pam &inpam, pam &shrunkPam, const int factor);
void bilinearInter(colourSpace **inA, colourSpace **outA, const pam &inpam, const pam &outpam, const int &factor);
void drawFaces(const pam &inpam, tuple **image, vector<face> faces);
void checkOverlapping(vector<face> &faces);
void copyImage(const pam &pamA, colourSpace **imageA, pam &pamB, colourSpace **imageB);

int main(int argc, char *argv[])
{
	//initialize netpbm library
	pm_init(argv[0], 0);
	
	//Basic error check for # args
	if (argc != 1)
	{
		cerr << "Invalid # of arguments." << endl << "Syntax: ./face_detection" << endl;
		return -1;
	}

	//image database
	long **database;

	bool notDone=true;
	cout << "Welcome to Will's Face Detection Program!" << endl;
	while (notDone)
	{
		int choice = 0;
		cout << "Please select from the following options:" << endl;
		cout << "1 - Read in Database and Create Backup File" << endl;
		cout << "2 - Read from Database file" << endl;
		cout << "3 - Exit" << endl;
		cin >> choice;
		
		if (choice == 3) // exit
		{
			cout << "Goodbye!" << endl;
			return 0;
		}
		else if (choice == 1) //Read in pictures
		{
			
			database = createDatabase();
			notDone=false;
			continue;
		}
		else if (choice == 2) //Read from file
		{
			database = loadDatabase();
			notDone=false;
			continue;
		}
		else
		{
			cout << "Invalid Selection" << endl;
		}
			
	}

	//get filename of test image
	cout << "Please enter the image file (.ppm) to search for faces:" << endl;
	char *fileIn;
	fileIn = new char[BUFFER_SIZE];
	cin >> fileIn;

	//in and out images
	pam inpam;
	//in and out arrays
	tuple **startArray;
	colourSpace **startSpace;

	//read input image param 1 into array
	startArray = read_image(fileIn, inpam);
	if (inpam.height<32 || inpam.width<32)
	{
		cerr << "Selected image is too small. Provide a larger image." << endl;
		pnm_freepamarray(startArray, &inpam);
		delete [] fileIn;
		return -1;
	}
	delete [] fileIn;

	//convert to YRGBangle
	startSpace = convert_spaces(inpam,startArray);

	vector<face> faces = findFaces(database, startSpace, inpam);
	//Draw Faces on original image
	drawFaces(inpam, startArray, faces);
	
	cout << "Please enter the destination image file (.ppm):" << endl;
	char *fileOut;
	fileOut = new char[BUFFER_SIZE];
	cin >> fileOut;

	//write final image
	write_image(fileOut, inpam, startArray);
	delete [] fileOut;

	cout << "Program Complete! Thank you!" << endl;

	//free memory (we're using C libraries)
	pnm_freepamarray(startArray, &inpam);
	for (int row=0; row<inpam.height; row++)
	{
		delete [] startSpace[row];
	}
	delete [] startSpace;
	for (int i=0; i<NUM_RECT; i++)
	{
		delete [] database[i];
	}
	delete [] database;

	return 0;
}
/*######################END MAIN############################*/

/*#######################BEGIN Functions####################*/
// Function is used to read in an image into the referenced
// 	variable inpam. Does error checking to ensure file is 
//		readable
tuple **read_image(char *filename, pam &inpam)
{
	FILE *f;
	tuple **A;

	//Check if file can be read
	if ((f = pm_openr(filename)) == NULL)
	{
		cerr << "Cannot open file \"" << filename << "\" for reading." << endl;
		exit(1);
	}

	//read into variable
	if ((A = pnm_readpam(f, &inpam, PAM_STRUCT_SIZE(tuple_type))) == NULL)
	{
		cerr << "Cannot read image \"" << filename << "\"." << endl;
		exit(1);
	}
	//Close file
	pm_close(f);
	return A;
}

// Function is used to write out the edited image out to the 
//		inputted file.
void write_image(char *filename, const pam &inpam, tuple **array)
{
	FILE *f;
	pam outpam = inpam;
	//Check if file can be read
	if ((f = pm_openw(filename)) == NULL)
	{
		cerr << "Cannot open file \"" << filename << "\" for writing." << endl;
		exit(1);
	}
	
	outpam.file = f;
  
	pnm_writepam(&outpam, array);

	pm_close(f);
}

//function to take in a RGB tuble array and output a 5 measurement array
colourSpace **convert_spaces(const pam &inpam, tuple **array)
{
	colourSpace **tempArray;
	tempArray = new colourSpace*[inpam.height];
	for (int row = 0; row < inpam.height; row++)
	{
   	tempArray[row] = new colourSpace[inpam.width];
	}

	for (int row = 0; row < inpam.height; row++)
	{
		for (int col = 0; col < inpam.width; col++)
		{
			//copy in values
			tempArray[row][col] = colourSpace(array[row][col][0],array[row][col][1],array[row][col][2]);
		}
	}
	
	return tempArray;
}

//histogram equalization over the individual measurements
void normalize(const pam &inpam, colourSpace **array)
{
	double yMap[MAX_INTENSITY] = {};
	double rMap[MAX_INTENSITY] = {};
	double gMap[MAX_INTENSITY] = {};
	double bMap[MAX_INTENSITY] = {};
	double aMap[MAX_ANGLE] = {};

	//iterate through image counting
	for (int row = 0; row < inpam.height; row++)
	{
   	for (int col = 0; col < inpam.width; col++)
		{
			yMap[array[row][col][0]]++;
			rMap[array[row][col][1]]++;
			gMap[array[row][col][2]]++;
			bMap[array[row][col][3]]++;
			aMap[array[row][col][4]]++;
		}
	}

	for (int i=0; i<MAX_ANGLE; i++)
	{
		//Normalize & collect p(r)
		if (i<MAX_INTENSITY)
		{
			yMap[i] /= (inpam.height * inpam.width);
			rMap[i] /= (inpam.height * inpam.width);
			gMap[i] /= (inpam.height * inpam.width);
			bMap[i] /= (inpam.height * inpam.width);
		}
		aMap[i] /= (inpam.height * inpam.width);

		//if not first, add it up
		if (i==0)
		{
			yMap[i] *= (MAX_INTENSITY-1);
			rMap[i] *= (MAX_INTENSITY-1);
			gMap[i] *= (MAX_INTENSITY-1);
			bMap[i] *= (MAX_INTENSITY-1);
			aMap[i] *= (MAX_ANGLE-1);
		}
		else
		{
			if (i<MAX_INTENSITY)
			{
				yMap[i] = yMap[i-1] + yMap[i]*(MAX_INTENSITY-1);
				rMap[i] = rMap[i-1] + rMap[i]*(MAX_INTENSITY-1);
				gMap[i] = gMap[i-1] + gMap[i]*(MAX_INTENSITY-1);
				bMap[i] = bMap[i-1] + bMap[i]*(MAX_INTENSITY-1);
			}
			aMap[i] = aMap[i-1] + aMap[i]*(MAX_ANGLE-1);
		}
		//round to nearest integer value
		if (i<MAX_INTENSITY)
		{
			yMap[i] = round(yMap[i]);
			rMap[i] = round(rMap[i]);
			gMap[i] = round(gMap[i]);
			bMap[i] = round(bMap[i]);
		}
		aMap[i] = round(aMap[i]);
	}

	//iterate through image updating intensity
	for (int row = 0; row < inpam.height; row++)
	{
   	for (int col = 0; col < inpam.width; col++)
		{
			array[row][col][0] = (long)yMap[array[row][col][0]];
			array[row][col][1] = (long)rMap[array[row][col][1]];
			array[row][col][2] = (long)gMap[array[row][col][2]];
			array[row][col][3] = (long)bMap[array[row][col][3]];
			array[row][col][4] = (long)aMap[array[row][col][4]];
		}
	}
}

//quick function to pull out the RGB from the colourspace when I want to save an image
void copyRGBtoRGB(const pam &inpam, tuple **rgbArray, colourSpace **clrArray)
{
	for (int row = 0; row < inpam.height; row++)
	{
   	for (int col = 0; col < inpam.width; col++)
		{
			rgbArray[row][col][0] = clrArray[row][col][1];
			rgbArray[row][col][1] = clrArray[row][col][2];
			rgbArray[row][col][2] = clrArray[row][col][3];
		}
	}
}

//linear binary operation to find texture patterns
void lbpOperation(const pam &inpam, colourSpace **array)
{
	static const int numNeigh = 9;
	static const int lbpVals[numNeigh] = {1,2,4,8,0,16,32,64,128};
	
	//allocate temparray
	colourSpace **tempArray;
	tempArray = new colourSpace*[inpam.height];
	for (int row = 0; row < inpam.height; row++)
	{
   	tempArray[row] = new colourSpace[inpam.width];
	}
	
	//Do lbp
	for (int row = 0; row < inpam.height; row++)
	{
   	for (int col = 0; col < inpam.width; col++)
		{
			//find neighbours
			point a[numNeigh] = {point(row-1,col-1),point(row-1,col),point(row-1,col+1),point(row,col-1),point(row,col),point(row,col+1),point(row+1,col-1),point(row+1,col),point(row+1,col+1)};

			long y[numNeigh], r[numNeigh], g[numNeigh], b[numNeigh], angle[numNeigh];
			
			for (int i=0; i<numNeigh; i++)
			{
				if (!contains(a[i], inpam.height, inpam.width))
				{
					point tempPoint = closest(a[i], a, numNeigh, inpam.height, inpam.width);
					y[i] = array[tempPoint.x][tempPoint.y][0];
					r[i] = array[tempPoint.x][tempPoint.y][1];
					g[i] = array[tempPoint.x][tempPoint.y][2];
					b[i] = array[tempPoint.x][tempPoint.y][3];
					angle[i] = array[tempPoint.x][tempPoint.y][4];
				}
				else
				{
					y[i] = array[a[i].x][a[i].y][0];
					r[i] = array[a[i].x][a[i].y][1];
					g[i] = array[a[i].x][a[i].y][2];
					b[i] = array[a[i].x][a[i].y][3];
					angle[i] = array[a[i].x][a[i].y][4];
				}
				//thresholding
				if (y[i]>array[row][col][0])
					y[i]=1;
				else
					y[i]=0;
				//
				if (r[i]>array[row][col][1])
					r[i]=1;
				else
					r[i]=0;
				//
				if (g[i]>array[row][col][2])
					g[i]=1;
				else
					g[i]=0;
				//
				if (b[i]>array[row][col][3])
					b[i]=1;
				else
					b[i]=0;
				//
				if (angle[i]>array[row][col][4])
					angle[i]=1;
				else
					angle[i]=0;
				
				//Now multiply by values
				y[i]*=lbpVals[i];
				r[i]*=lbpVals[i];
				g[i]*=lbpVals[i];
				b[i]*=lbpVals[i];
				angle[i]*=lbpVals[i];
			}
			
			long ySum, rSum, gSum, bSum, angleSum;
			ySum = 0;
			rSum = 0;
			gSum = 0;
			bSum = 0;
			angleSum = 0;
			//sum each of them
			for (int i=0; i<numNeigh; i++)
			{
				ySum+=y[i];
				rSum+=r[i];
				gSum+=g[i];
				bSum+=b[i];
				angleSum+=angle[i];
			}

			//store result
			tempArray[row][col][0] = ySum;
			tempArray[row][col][1] = rSum;
			tempArray[row][col][2] = gSum;
			tempArray[row][col][3] = bSum;
			tempArray[row][col][4] = angleSum;
		}
	}

	//copy back into original array
	for (int row = 0; row < inpam.height; row++)
	{
   	for (int col = 0; col < inpam.width; col++)
		{
			for (int i=0; i<5; i++)
			{
				array[row][col][i] = tempArray[row][col][i];
			}
		}
	}

	for (int row=0; row<inpam.height; row++)
	{
		delete[] tempArray[row];
	}
	delete[] tempArray;
}

//simple check if a point is contained within a box
bool contains(const point a, const int length, const int width)
{
	if ((a.x<length) && (a.x>=0) && (a.y<width) && (a.y>=0))
		return true;
	else
		return false;
}

//finding the closest point to a certain point that might be out of bounds
point closest(point &a, point b[], const int &size, const int &length, const int &width)
{
	double minDist = 2;
	long minPoint = 5;
	for (int i=0; i<size; i++)
	{
		if (a==b[i])
			continue;
		if (!contains(b[i], length, width))
			continue;
		double dist = sqrt(pow(a.x-b[i].x,2)+pow(a.y-b[i].y,2));
		if (dist<minDist)
		{
			minDist=dist;
			minPoint=i;
		}
	}
	return b[minPoint];
}

//function to quickly return a 32x32 histogram on any requested dimension.
long *returnHistogram(colourSpace **image, const int &rectNum, const int &dim, long startX, long startY)
{
	long *hist;
	hist = new long[MAX_INTENSITY];
	for (int i=0; i<MAX_INTENSITY; i++)
		hist[i]=0;
	
	for (int row=startX; row<(startX+RECT_SIZE); row++)
	{
		for (int col=startY; col<(startY+RECT_SIZE); col++)
		{
			switch (rectNum)
			{
				case 0: //rectangle 1
					hist[image[row][col][dim]]++;
					break;
				case 1: //rectangle 2
					if (row>(startX+4) && row<(startX+28))
						hist[image[row][col][dim]]++;
					break;
				case 2: //rectangle 3
					if (col>(startY+4) && col < (startY+28))
						hist[image[row][col][dim]]++;
					break;
			}
		}
	}

	return hist;
}

//difference between two histograms
long histDiff(long *a, long *b)
{
	long sum=0;
	for (int i=0; i<MAX_INTENSITY; i++)
	{
		sum+=min(a[i],b[i]);
	}
	return sum;
}

//function to create and manage the database of the training data
long **createDatabase()
{
	DIR *dp;
	struct dirent *dirp;
	vector<string> files;
	string prefix = "./database/";

	if ((dp = opendir(prefix.c_str())) == NULL)
	{
		cerr << "Error(" << errno << ") opening ./database/" << endl;
		exit (-1);
	}
	
	while ((dirp = readdir(dp)) != NULL)
	{
		string filename = string(dirp->d_name);
		if (filename.size() > 4 && filename.substr(filename.size()-4) == ".ppm")
			files.push_back(string(dirp->d_name));
	}
	closedir(dp);

	long **database;
	database = new long*[NUM_RECT];
	for (int i=0; i<NUM_RECT; i++)
	{
		database[i] = new long[MAX_INTENSITY];
		for (int j=0; j<MAX_INTENSITY; j++)
		{
			database[i][j]=0;
		}
	}

	for (unsigned int i=0; i<files.size(); i++)
	{
		//declares
		pam inpam;
		tuple **image;
		colourSpace **startSpace;
		long *imageHist;
		char *filename = new char[sizeof (prefix + files[i]).c_str()];
		strcpy(filename, (prefix + files[i]).c_str());

		image = read_image(filename, inpam);
		//convert to YRGBangle
		startSpace = convert_spaces(inpam,image);
		//normalize startSpace
		normalize(inpam, startSpace);
		//lbp
		lbpOperation(inpam, startSpace);
		//extract interested dimension
		for (int j=0; j<NUM_RECT; j++)
		{
			imageHist = returnHistogram(startSpace, j, 0); //height and width should always be 32
			for (int k=0; k<MAX_INTENSITY; k++)
			{
				database[j][k] += imageHist[k];
			}
		}

		//cleanup
		pnm_freepamarray(image, &inpam);
		for (int row=0; row<inpam.height; row++)
		{
			delete [] startSpace[row];
		}
		delete [] startSpace;
		delete [] imageHist;
	}
	
	//save the database!
	ofstream fout;
	fout.open("./imageDB.txt");

	for (int i=0; i<NUM_RECT; i++)
	{
		for (int j=0; j<MAX_INTENSITY; j++)
		{
			database[i][j] = round(database[i][j]/(1.0*files.size()));
			fout << NTS(database[i][j]).c_str();
			if (!(j == MAX_INTENSITY-1))
				fout << ",";
		}
		if (i != NUM_RECT-1)
			fout << endl;
	}
	fout.close();
	
	return database;
}

//load the database from a file
long **loadDatabase()
{
	long **database;
	database = new long*[NUM_RECT];
	for (int i=0; i<NUM_RECT; i++)
	{
		database[i] = new long[MAX_INTENSITY];
		for (int j=0; j<MAX_INTENSITY; j++)
		{
			database[i][j]=0;
		}
	}

	ifstream fin;
	fin.open("./imageDB.txt");
	if (!(fin.is_open()))
	{
		cerr << "Unable to open database file. Does it exist?" << endl;
		exit(1);
	}
	
	string line;
	for (int i=0; getline(fin,line); i++)
	{
		stringstream ss(line);
		string elem;
		for (int j=0; getline(ss, elem, ','); j++)
		{
			istringstream(elem) >> database[i][j];
		}
	}
	fin.close();
	
	return database;
}

//calculate the frequency of the difference between image histogram and training data
bool calcFreq(long **database, colourSpace **image, long startX, long startY)
{
	//pull 3 histograms and calculate difference
	long *rc1, *rc2, *rc3;
	rc1 = returnHistogram(image, 0, 0, startX, startY);
	rc2 = returnHistogram(image, 1, 0, startX, startY);
	rc3 = returnHistogram(image, 2, 0, startX, startY);
	double rect1 = histDiff(rc1, database[0]);
	double rect2 = histDiff(rc2, database[1]);
	double rect3 = histDiff(rc3, database[2]);
	
	//divide by number of white pixels in mask
	rect1/=(RECT_SIZE*RECT_SIZE);
	rect2/=((RECT_SIZE-8)*RECT_SIZE);
	rect3/=((RECT_SIZE-8)*RECT_SIZE);

	//cleanup
	delete [] rc1;
	delete [] rc2;
	delete [] rc3;
	
	if ((rect1 >= T) && (rect2 >= T) && (rect3 >= T))
		return true;
	else
		return false;
}

//all in one function to find faces. This function is called from main and calls the majority of the other
// functions
vector<face> findFaces(long **database, colourSpace **image, const pam &inpam)
{
	//allocate out some temporary storage so we don't wreck the original
	vector<face> faces;
	pam shrunkPam = inpam;
	colourSpace **shrunkImage;
	pam copyPam;
	colourSpace **copyI;
	
	//allocate out image
	shrunkImage = new colourSpace*[shrunkPam.height];
	for (int row = 0; row < shrunkPam.height; row++)
	{
   	shrunkImage[row] = new colourSpace[shrunkPam.width];
	}
	copyImage(inpam, image, shrunkPam, shrunkImage); //copy function
	
	long i = 1;
	bool notDone = true;

	while (notDone)
	{
		//create a copy so as not to change original
		copyPam = shrunkPam;
		copyI = new colourSpace*[copyPam.height];
		for (int row = 0; row < copyPam.height; row++)
		{
			copyI[row] = new colourSpace[copyPam.width];
		}
		copyImage(shrunkPam, shrunkImage, copyPam, copyI);
		
		//normalize/lbp startSpace
		normalize(shrunkPam, shrunkImage);
		lbpOperation(shrunkPam, shrunkImage);

		//scan through image
		for (int row=0; row<shrunkPam.height-(RECT_SIZE-1); row+=8)
		{
			for (int col=0; col<shrunkPam.width-(RECT_SIZE-1); col+=8)
			{
				if (calcFreq(database, shrunkImage, row, col))
				{
					point tempPoint = point(row,col);
					faces.push_back(face(tempPoint,i));
				}
			}
		}

		//although this looks scary, only executing 10 times or until image is too small to test
		if (i > 512 || (shrunkPam.width < RECT_SIZE*2) || (shrunkPam.height < RECT_SIZE*2))
		{
			if (faces.size()==0)
				cout << "Giving up, no faces found." << endl;
			notDone=false;
		}

		//cleanup
		for (int row=0; row<shrunkPam.height; row++)
		{
			delete [] shrunkImage[row];
		}
		delete [] shrunkImage;

		//shrink image by half and try again
		if (notDone)
			shrunkImage = shrinkImage(copyI, copyPam, shrunkPam, 2);

		//cleanup
		for (int row=0; row<copyPam.height; row++)
		{
			delete [] copyI[row];
		}
		delete [] copyI;

		i*=2;
	}
	return faces;
}

//standard shrink preparation
colourSpace **shrinkImage(colourSpace **image, pam &inpam, pam &shrunkPam, const int factor)
{
	//copy over the minor details
	shrunkPam.width = round(inpam.width/(1.0*factor));
	shrunkPam.height = round(inpam.height/(1.0*factor));

	//allocate out image
	colourSpace **shrunkImage;
	shrunkImage = new colourSpace*[shrunkPam.height];
	for (int row = 0; row < shrunkPam.height; row++)
	{
   	shrunkImage[row] = new colourSpace[shrunkPam.width];
	}

	bilinearInter(image, shrunkImage, inpam, shrunkPam, factor);

	return shrunkImage;
}

//bilinearinterpolation
void bilinearInter(colourSpace **inA, colourSpace **outA, const pam &inpam, const pam &outpam, const int &factor)
{
	//iterate through all of the output file pixels
	for (int row = 0; row < outpam.height; row++)
	{
   	for (int col = 0; col < outpam.width; col++)
		{
			//Offset the row/columns of new image to put it back into
			// same cartesian plane
			long oldX = row*factor;
			long oldY = col*factor;
			//if calculated pixel is out of bounds of original, ignore & continue
			if (oldX < 0 || oldX >= inpam.height || oldY < 0 || oldY >= inpam.width)
			{
				continue;
			}
			
			//find 4 nearest neighbors
			point a1 = point(oldX-1, oldY-1);
			point a2 = point(oldX-1, oldY);
			point a3 = point(oldX, oldY-1);
			point a4 = point(oldX, oldY);
			
			//grab intensity from neighbours
			point neighbours[] = {a1,a2,a3,a4};
			long neighbourY[4];
			long neighbourR[4];
			long neighbourG[4];
			long neighbourB[4];
			long neighbourA[4];
			//going to be used to find the average
			long avgY = 0;
			long avgR = 0;
			long avgG = 0;
			long avgB = 0;
			long avgA = 0;
			long corVals = 0;
			//iterate through 4 neighbours
			for (int i=0; i<4; i++)
			{
				//check if the neighbour falls out of bounds
				if (neighbours[i].x < 0 || neighbours[i].x >= inpam.height || neighbours[i].y < 0 || neighbours[i].y >= inpam.width)
				{
					//If out-of-bounds, assign value and deal with after
					neighbourY[i] = -1;
					neighbourR[i] = -1;
					neighbourG[i] = -1;
					neighbourB[i] = -1;
					neighbourA[i] = -1;
				}
				else
				{
					//assign the value into array and into avg sum
					avgY += neighbourY[i] = inA[neighbours[i].x][neighbours[i].y].y;
					avgR += neighbourR[i] = inA[neighbours[i].x][neighbours[i].y].r;
					avgG += neighbourG[i] = inA[neighbours[i].x][neighbours[i].y].g;
					avgB += neighbourB[i] = inA[neighbours[i].x][neighbours[i].y].b;
					avgA += neighbourA[i] = inA[neighbours[i].x][neighbours[i].y].angle;
					corVals++;
				}
			}
			//If it doesn't have an intensity, I'm giving it the average
			avgY = (long)round(avgY/corVals);
			avgR = (long)round(avgR/corVals);
			avgG = (long)round(avgG/corVals);
			avgB = (long)round(avgB/corVals);
			avgA = (long)round(avgA/corVals);
			for (int i=0; i<4; i++)
			{
				if (neighbourY[i] == -1)
				{
					neighbourY[i] = avgY;
					neighbourR[i] = avgR;
					neighbourG[i] = avgG;
					neighbourB[i] = avgB;
					neighbourA[i] = avgA;
				}
			}

			//do bilinear formula
			double distA = oldY - neighbours[0].y;
			double distB = oldX - neighbours[0].x;
			//simplified bilinear formula based on distance/intensity
			outA[row][col].y = (1-distA)*(1-distB)*neighbourY[0]+
									  (distA)*(1-distB)*neighbourY[1]+
									  (1-distA)*(distB)*neighbourY[2]+
									  (distA)*(distB)*neighbourY[3];
			outA[row][col].r = (1-distA)*(1-distB)*neighbourR[0]+
									  (distA)*(1-distB)*neighbourR[1]+
									  (1-distA)*(distB)*neighbourR[2]+
									  (distA)*(distB)*neighbourR[3];
			outA[row][col].g = (1-distA)*(1-distB)*neighbourG[0]+
									  (distA)*(1-distB)*neighbourG[1]+
									  (1-distA)*(distB)*neighbourG[2]+
									  (distA)*(distB)*neighbourG[3];
			outA[row][col].b = (1-distA)*(1-distB)*neighbourB[0]+
									  (distA)*(1-distB)*neighbourB[1]+
									  (1-distA)*(distB)*neighbourB[2]+
									  (distA)*(distB)*neighbourB[3];
			outA[row][col].angle = (1-distA)*(1-distB)*neighbourA[0]+
									  (distA)*(1-distB)*neighbourA[1]+
									  (1-distA)*(distB)*neighbourA[2]+
									  (distA)*(distB)*neighbourA[3];
		}
	}
}

//big call function from main to draw faces on the original image
void drawFaces(const pam &inpam, tuple **image, vector<face> faces)
{
	checkOverlapping(faces);
	
	for (unsigned int i=0; i<faces.size(); i++)
	{
		point start = point(faces[i].x()*faces[i].size(),faces[i].y()*faces[i].size());
		long newRectSize = (RECT_SIZE*faces[i].size()) - (faces[i].size());
		
		//draw scaled squares
		for (int row = start.x; row <= start.x+newRectSize; row++)
		{
			if (row == start.x || row == start.x+newRectSize)
			{
				for (int col = start.y; col <= start.y+newRectSize; col++)
				{
					image[row][col][0]=255;
					image[row][col][1]=255;
					image[row][col][2]=255;
				}
			}
			else
			{
				//first
				image[row][start.y][0]=255;
				image[row][start.y][1]=255;
				image[row][start.y][2]=255;
				//last
				image[row][start.y+newRectSize][0]=255;
				image[row][start.y+newRectSize][1]=255;
				image[row][start.y+newRectSize][2]=255;
			}
		}
	}
}

//check overlapping though most of the work is done in the struct
void checkOverlapping(vector<face> &faces)
{
	for (unsigned int i=0; i<faces.size()-1; i++)
	{
		bool notDone=true;
		unsigned int j=i+1;
		while (notDone)
		{
			if (j<faces.size() && j!=i)
			{
				if (faces[i].overlaps(faces[j]))
					faces.erase(faces.begin()+j);
				else
					j++;
			}
			else
			{
				notDone=false;
			}
		}
	}
}

//copy image data from two different pointed at data sources
void copyImage(const pam &pamA, colourSpace **imageA, pam &pamB, colourSpace **imageB)
{
	for (int row = 0; row < pamB.height; row++)
	{
		for (int col = 0; col < pamB.width; col++)
		{
			for (int i=0; i<5; i++)
			{
				imageB[row][col][i] = imageA[row][col][i];
			}
		}
	}
}
