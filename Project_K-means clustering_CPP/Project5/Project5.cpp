#include<iostream>
#include<fstream>
#include<time.h>
#include<string>
#include<math.h>
using namespace std;

class point {
public:
	int x_coordinate, y_coordinate, label;
	double distance;

	point() {
		x_coordinate = 0;
		y_coordinate = 0;
		label = 0;
		distance = 1000.00;
	}
	string printPoint() {
		string s = "";
		s+=  to_string(x_coordinate) + " " +to_string(y_coordinate)  + " " + to_string(label);
		return s;
	}
};

class k_mean {
	int k;
	struct xyCoord {
		int x_coordinate, y_coordinate, label;
	};

public:
	xyCoord *kCentroids;
	int numPts, numRow, numCol;
	point *pointSet;
	int** imageArray;
	int changeLabel;

	k_mean(int k1, int row, int col, int pts) {
		numPts = pts;
		k = k1;
		numRow = row;
		numCol = col;
		imageArray = new int*[numRow];
		for (int i = 0; i < numRow; i++)
			imageArray[i] = new int[numCol];
		kCentroids = new xyCoord[k];
		pointSet = new point[numPts];
	}

	~k_mean(){
		delete[] kCentroids;
		delete[] pointSet;
		for (int i = 0; i < numRow; i++)
			delete[] imageArray[i];
		delete[] imageArray;
	}

	void loadPointSet(int row, int col,int pt) {
		pointSet[pt].x_coordinate=row;
		pointSet[pt].y_coordinate =col;
	}

	void assignLabel() {
		srand((unsigned) time(NULL));
		for (int i = 0; i < numPts; i++) {	
			pointSet[i].label = rand() % k + 1;
		}
	}

	void mapPoint2Image() {
		for (int i = 0; i < numPts; i++) {
			imageArray[pointSet[i].x_coordinate][pointSet[i].y_coordinate]=pointSet[i].label;
		}
	}
	
	void kMeanClustering() {
		int meanX1=0, meanY1=0, meanX2 = 0, meanY2 = 0, meanX3 = 0, meanY3 = 0, meanX4 = 0, meanY4 = 0, k1=0, k2 = 0, k3 = 0, k4 = 0;
		for (int i = 0; i < numPts; i++) {
			if (pointSet[i].label == 1) {
				k1++;
				meanX1 += pointSet[i].x_coordinate;
				meanY1 += pointSet[i].y_coordinate;
			}
			else if (pointSet[i].label == 2) {
				k2++;
				meanX2 += pointSet[i].x_coordinate;
				meanY2 += pointSet[i].y_coordinate;
			}
			else if (pointSet[i].label == 3) {
				k3++;
				meanX3 += pointSet[i].x_coordinate;
				meanY3 += pointSet[i].y_coordinate;
			}
			else {
				k4++;
				meanX4 += pointSet[i].x_coordinate;
				meanY4 += pointSet[i].y_coordinate;
			}
				
		}
		kCentroids[0].label = 1;
		kCentroids[1].label = 2;
		kCentroids[2].label = 3;
		kCentroids[3].label = 4;

		if (k1 == 0) {
			kCentroids[0].x_coordinate = 0;
			kCentroids[0].y_coordinate = 0;
		}
		else {
			kCentroids[0].x_coordinate = meanX1 / k1;
			kCentroids[0].y_coordinate = meanY1 / k1;
		}
		if (k2 == 0) {
			kCentroids[1].x_coordinate = 0;
			kCentroids[1].y_coordinate = 0;
		}
		else {
			kCentroids[1].x_coordinate = meanX2 / k2;
			kCentroids[1].y_coordinate = meanY2 / k2;
		}
		if (k3 == 0) {
			kCentroids[2].x_coordinate = 0;
			kCentroids[2].y_coordinate = 0;
		}
		else {
			kCentroids[2].x_coordinate = meanX3 / k3;
			kCentroids[2].y_coordinate = meanY3 / k3;
		}
		if (k4 == 0) {
			kCentroids[3].x_coordinate = 0;
			kCentroids[3].y_coordinate = 0;
		}
		else {
			kCentroids[3].x_coordinate = meanX4 / k4;
			kCentroids[3].y_coordinate = meanY4 / k4;
		}
	
	}

	void printPointset(ofstream & outPut1) {
		outPut1 << k << endl;
		outPut1 << numPts << endl;
		outPut1 << numRow << " " << numCol<<  endl;
		point pt;
		for (int i = 0; i < numPts; i++) {
			outPut1<< pointSet[i].printPoint() << endl;
		}	
	}

	void prettyPrint(ofstream &outPut2) {
		static int count = 0;
		outPut2 << "----------------------------------------------------------------------------------" << endl;
		for (int i = 0; i < numRow; i++) {
			outPut2 << "|";
			for (int j = 0; j < numCol; j++) {
				if (kCentroids[0].x_coordinate == i&&kCentroids[0].y_coordinate == j)
					outPut2 << 'A';
				if (kCentroids[1].x_coordinate == i&&kCentroids[1].y_coordinate == j)
					outPut2 << 'B';
				if (kCentroids[2].x_coordinate == i&&kCentroids[2].y_coordinate == j)
					outPut2 << 'C';
				if (kCentroids[3].x_coordinate == i&&kCentroids[3].y_coordinate == j)
					outPut2 << 'D';
				if (imageArray[i][j] > 0)
					outPut2 << imageArray[i][j];
				else
					outPut2 << " ";
			}
			outPut2 << "|"<<endl;
		}
		
		outPut2 << "----------------------------------------------------------------------------------" << endl;
		outPut2 << count << " iteration" << endl << endl;
		count++;
	}

	double getDistance(int px, int py, int cx, int cy) {
		return sqrt(pow(cx - px, 2.0)+pow(cy - py, 2.0));
	}
};

int main(int argc, char** argv) {
	ifstream inFile;
	ofstream outPut1, outPut2;
	inFile.open(argv[1]);
	outPut1.open(argv[2]);
	outPut2.open(argv[3]);
	
	int row, column,k,numRow,numCol, numPts,pt=0;
	inFile >> k;
	inFile >> numPts;
	inFile >> numRow;
	inFile >> numCol;
	k_mean km(k, numRow, numCol, numPts);
	while (inFile >> row) {
		inFile >> column;
		km.loadPointSet(row,column, pt);
		pt++;
	}
	km.assignLabel();
	km.mapPoint2Image();
	km.prettyPrint(outPut2);
	bool change = true;
	while (change) {
		change = false;
		srand(time(NULL));
		int changeLabel = 0;
		km.kMeanClustering();
		int px, py, cx, cy,min_i=0;
		for (int i = 0; i < numPts; i++) {
			px=km.pointSet[i].x_coordinate;
			py=km.pointSet[i].y_coordinate;
			for (int j = 0; j <k; j++) {
				cx=km.kCentroids[j].x_coordinate;
				cy=km.kCentroids[j].y_coordinate;
				double dist=km.getDistance(px, py, cx, cy);
				if (dist < km.pointSet[i].distance) {
					km.pointSet[i].distance = dist;
					min_i = j+1;
					if (km.pointSet[i].label != min_i) {
						km.pointSet[i].label = min_i;
						changeLabel++;
					}
				}
				if (dist == km.pointSet[i].distance&&(rand()%2+1)==1) {
					km.pointSet[i].distance = dist;
					min_i = j + 1;
					if (km.pointSet[i].label != min_i) {
						km.pointSet[i].label = min_i;
						changeLabel++;
					}
				}//choose randomly if they have two same distance
			}
		}//for
		if (changeLabel > 0)
			change = true;
		else
			change = false;
		km.mapPoint2Image();
		km.prettyPrint(outPut2);		
	}//while
	km.printPointset(outPut1);
	inFile.close();
	outPut1.close();
	outPut2.close();
}
