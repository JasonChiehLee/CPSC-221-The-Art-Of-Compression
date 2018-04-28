#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "stats.h"
#include <utility>
#include <vector>
using namespace std;
using namespace cs221util;

long stats::getSum(char channel, pair<int, int> ul, pair<int, int> lr) {

	long result = 0;

	if (channel == 'r') {
		if (ul.first == 0 && ul.second == 0) {
			result = sumRed[lr.first][lr.second];
		}
		else if (ul.first != 0 && ul.second ==0) {
			result = sumRed[lr.first][lr.second] - sumRed[ul.first - 1][lr.second];
		}
		else if (ul.first == 0 && ul.second != 0) {
			result = sumRed[lr.first][lr.second] - sumRed[lr.first][ul.second - 1];
		}
		else {
			result = sumRed[lr.first][lr.second] - (sumRed[lr.first][ul.second - 1] + sumRed[ul.first - 1][lr.second] - sumRed[ul.first - 1][ul.second - 1]);
		}
	}
	else if (channel == 'g') {
		if (ul.first == 0 && ul.second == 0) {
			result = sumGreen[lr.first][lr.second];
		}
		else if (ul.first != 0 && ul.second == 0) {
			result = sumGreen[lr.first][lr.second] - sumGreen[ul.first - 1][lr.second];
		}
		else if (ul.first == 0 && ul.second != 0) {
			result = sumGreen[lr.first][lr.second] - sumGreen[lr.first][ul.second - 1];
		}
		else {
			result = sumGreen[lr.first][lr.second] - (sumGreen[lr.first][ul.second - 1] + sumGreen[ul.first - 1][lr.second] - sumGreen[ul.first - 1][ul.second - 1]);
		}
	}
	else if (channel == 'b') {
		if (ul.first == 0 && ul.second == 0) {
			result = sumBlue[lr.first][lr.second];
		}
		else if (ul.first != 0 && ul.second == 0) {
			result = sumBlue[lr.first][lr.second] - sumBlue[ul.first - 1][lr.second];
		}
		else if (ul.first == 0 && ul.second != 0) {
			result = sumBlue[lr.first][lr.second] - sumBlue[lr.first][ul.second - 1];
		}
		else {
			result = sumBlue[lr.first][lr.second] - (sumBlue[lr.first][ul.second - 1] + sumBlue[ul.first - 1][lr.second] - sumBlue[ul.first - 1][ul.second - 1]);
		}
	}
	
	return result;
}

long stats::getSumSq(char channel, pair<int, int> ul, pair<int, int> lr) {
	long result = 0;

	if (channel == 'r') {
		if (ul.first == 0 && ul.second == 0) {
			result = sumsqRed[lr.first][lr.second];
		}
		else if (ul.first != 0 && ul.second == 0) {
			result = sumsqRed[lr.first][lr.second] - sumsqRed[ul.first - 1][lr.second];
		}
		else if (ul.first == 0 && ul.second != 0) {
			result = sumsqRed[lr.first][lr.second] - sumsqRed[lr.first][ul.second - 1];
		}
		else {
			result = sumsqRed[lr.first][lr.second] - (sumsqRed[lr.first][ul.second - 1] + sumsqRed[ul.first - 1][lr.second] - sumsqRed[ul.first - 1][ul.second - 1]);
		}
	}
	else if (channel == 'g') {
		if (ul.first == 0 && ul.second == 0) {
			result = sumsqGreen[lr.first][lr.second];
		}
		else if (ul.first != 0 && ul.second == 0) {
			result = sumsqGreen[lr.first][lr.second] - sumsqGreen[ul.first - 1][lr.second];
		}
		else if (ul.first == 0 && ul.second != 0) {
			result = sumsqGreen[lr.first][lr.second] - sumsqGreen[lr.first][ul.second - 1];
		}
		else {
			result = sumsqGreen[lr.first][lr.second] - (sumsqGreen[lr.first][ul.second - 1] + sumsqGreen[ul.first - 1][lr.second] - sumsqGreen[ul.first - 1][ul.second - 1]);
		}
	}
	else if (channel == 'b') {
		if (ul.first == 0 && ul.second == 0) {
			result = sumsqBlue[lr.first][lr.second];
		}
		else if (ul.first != 0 && ul.second == 0) {
			result = sumsqBlue[lr.first][lr.second] - sumsqBlue[ul.first - 1][lr.second];
		}
		else if (ul.first == 0 && ul.second != 0) {
			result = sumsqBlue[lr.first][lr.second] - sumsqBlue[lr.first][ul.second - 1];
		}
		else {
			result = sumsqBlue[lr.first][lr.second] - (sumsqBlue[lr.first][ul.second - 1] + sumsqBlue[ul.first - 1][lr.second] - sumsqBlue[ul.first - 1][ul.second - 1]);
		}
	}

	return result;
}

stats::stats(PNG & im) {

	for (unsigned int x = 0; x < im.width(); x++) {

		vector<long> redcolumn(im.height());
		vector<long> greencolumn(im.height());
		vector<long> bluecolumn(im.height());

		vector<long> redcolumnsquare(im.height());
		vector<long> greencolumnsquare(im.height());
		vector<long> bluecolumnsquare(im.height());
		
		for (unsigned int y = 0; y < im.height(); y++) {
			
			RGBAPixel* curr = im.getPixel(x, y);
			
			int prevRed = 0;
			int prevGreen = 0;
			int prevBlue = 0;
			
			int prevRedSq = 0;
			int prevGreenSq = 0;
			int prevBlueSq = 0;
			
			
			if (y > 0){	
				prevRed = redcolumn[y-1];
				prevGreen = greencolumn[y-1];
				prevBlue = bluecolumn[y-1];
				
				prevRedSq = redcolumnsquare[y-1];
				prevGreenSq =  greencolumnsquare[y-1];
				prevBlueSq = bluecolumnsquare[y-1];
			}
			
			int simpleRed = curr->r;
			int simpleGreen = curr->g;
			int simpleBlue = curr->b;

			int redSquared = curr->r * curr->r;
			int greenSquared = curr->g * curr->g; 
			int blueSquared = curr->b * curr->b;
			
			if (x == 0 && y == 0){
				redcolumn[y] = (simpleRed);
				greencolumn[y] = (simpleGreen);
				bluecolumn[y] = (simpleBlue);
				
				redcolumnsquare[y] = (redSquared);
				greencolumnsquare[y] = (greenSquared);
				bluecolumnsquare[y] = (blueSquared);
			}
			if (x == 0 && y > 0){
				redcolumn[y] = (simpleRed + prevRed);
				greencolumn[y] = (simpleGreen + prevGreen);
				bluecolumn[y] = (simpleBlue + prevBlue);
				
				redcolumnsquare[y] = (redSquared + prevRedSq);
				greencolumnsquare[y] = (greenSquared + prevGreenSq);
				bluecolumnsquare[y] = (blueSquared + prevBlueSq);
			}
			if (x > 0 && y == 0){
				redcolumn[y] = (sumRed[x-1][y] + simpleRed);
				greencolumn[y] = (sumGreen[x-1][y] + simpleGreen);
				bluecolumn[y] = (sumBlue[x-1][y] + simpleBlue);
				
				redcolumnsquare[y] = (sumsqRed[x-1][y] + redSquared);
				greencolumnsquare[y] = (sumsqGreen[x-1][y] + greenSquared);
				bluecolumnsquare[y] = (sumsqBlue[x-1][y] + blueSquared);
			}
			if (x > 0 && y > 0){
				redcolumn[y] = (sumRed[x-1][y] + simpleRed + prevRed - sumRed[x-1][y-1]);
				greencolumn[y] = (sumGreen[x-1][y] + simpleGreen + prevGreen - sumGreen[x-1][y-1]);
				bluecolumn[y] = (sumBlue[x-1][y] + simpleBlue + prevBlue - sumBlue[x-1][y-1]);
				
				redcolumnsquare[y] = (sumsqRed[x-1][y] + redSquared + prevRedSq - sumsqRed[x-1][y-1]);
				greencolumnsquare[y] = (sumsqGreen[x-1][y] + greenSquared + prevGreenSq - sumsqGreen[x-1][y-1]);
				bluecolumnsquare[y] = (sumsqBlue[x-1][y] + blueSquared + prevBlueSq - sumsqBlue[x-1][y-1]);
			}			

		}
			
			sumRed.push_back(redcolumn);
			sumGreen.push_back(greencolumn);
			sumBlue.push_back(bluecolumn);

			sumsqRed.push_back(redcolumnsquare);
			sumsqGreen.push_back(greencolumnsquare);
			sumsqBlue.push_back(bluecolumnsquare);


	}
}

long stats::getScore(pair<int, int> ul, pair<int, int> lr) {
	
	long redsquared = getSumSq('r', ul, lr);
	long greensquared = getSumSq('g', ul, lr);
	long bluesquared = getSumSq('b', ul, lr);

	long red = getSum('r', ul, lr);
	long green = getSum('g', ul, lr);
	long blue = getSum('b', ul, lr);

	long area = rectArea(ul, lr);

	red = red * red;
	green = green * green;
	blue = blue * blue;

	long reddiff = redsquared - (red / area);
	long greendiff = greensquared - (green / area);
	long bluediff = bluesquared - (blue / area);

	long sum = reddiff + greendiff + bluediff;

	return sum;
}

RGBAPixel stats::getAvg(pair<int, int> ul, pair<int, int> lr) {
	long area = rectArea(ul, lr);

	long redsum = getSum('r', ul, lr);
	long greensum = getSum('g', ul, lr);
	long bluesum = getSum('b', ul, lr);

	RGBAPixel averagepixel = RGBAPixel(redsum / area, greensum / area, bluesum / area);

	return averagepixel;

}

long stats::rectArea(pair<int, int> ul, pair<int, int> lr) {
	long x = lr.first - ul.first + 1;
	long y = lr.second - ul.second + 1;

	long product = x * y;

	return product;
}