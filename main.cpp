
// File:        main.cpp
// Author:      cheeren
// Date:        2018-02-25
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets


#include "twoDtree.h"
#include "cs221util/RGBAPixel.h"
#include "cs221util/PNG.h"

using namespace cs221util;
using namespace std;

int main()
{

	// read in image
	PNG origIm1;
	origIm1.readFromFile("images/stanley-totem-poles.png");
//	origIm1.resize(100, 100);
	PNG origIm2;
	origIm2.readFromFile("images/ubc-totem-poles.png");
	PNG origIm3;
	origIm3.readFromFile("images/remb.png");
	PNG origIm4;
	origIm4.readFromFile("images/rosa.png");

	// use it to build a twoDtree
	cout << "construct origIm1" << endl;
	cout << "height: " << origIm1.height() << ", width: " << origIm1.width() << endl;

	cout << "do we get past here?" << endl;
	twoDtree t1(origIm1);
	twoDtree tCopy1(t1);
	cout << "construct origIm2" << endl;
	twoDtree t2(origIm2);
	twoDtree tCopy2(t2);
	cout << "construct origIm3" << endl;
	twoDtree t3(origIm3);
	twoDtree tCopy3(t3);
	cout << "construct origIm4" << endl;
	twoDtree t4(origIm4);
	twoDtree tCopy4(t4);

	// prune the twoDtree
	//where at least 100*X% of pixels are within Y of mean
	
	cout << "pruning" << endl;
	tCopy1.prune(0.95, 10000); 
	tCopy2.prune(0.95, 10000); 
	tCopy3.prune(0.95, 3000); 
	tCopy4.prune(0.95, 3000); 
	
	// render the twoDtree
	PNG ppic1 = tCopy1.render();
	PNG ppic2 = tCopy2.render();
	PNG ppic3 = tCopy3.render();
	PNG ppic4 = tCopy4.render();

	ppic1.writeToFile("images/prunedstanleytotem.png");
	ppic2.writeToFile("images/prunedubctotem.png");
	ppic3.writeToFile("images/prunedremb.png");
	ppic4.writeToFile("images/prunedrosa.png");

	//t1.prune(0.95, 1000);
	/*PNG photo1 = t1.render();
	cout << "done rendering" << endl;
	photo1.writeToFile("images/prunedstanleytotem.png");
*/	
	cout << "WE DONT MAIN" << endl;
	return 0;
}
