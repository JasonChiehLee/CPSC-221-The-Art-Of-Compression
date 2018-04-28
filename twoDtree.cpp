
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */
#include "limits.h" //TODO, DOUBLE CHECK THIS AND LONG_MAX
#include "twoDtree.h"

using namespace std;

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}
  

  /**
    * Constructor that builds a twoDtree out of the given PNG.
    * Every leaf in the tree corresponds to a pixel in the PNG.
    * Every non-leaf node corresponds to a rectangle of pixels 
    * in the original PNG, represented by an (x,y) pair for the 
    * upper left corner of the rectangle and an (x,y) pair for 
    * lower right corner of the rectangle. In addition, the Node
    * stores a pixel representing the average color over the 
    * rectangle. 
    *
    * Every node's left and right children correspond to a partition
    * of the node's rectangle into two smaller rectangles. The node's
    * rectangle is split by the horizontal or vertical line that 
    * results in the two smaller rectangles whose sum of squared 
    * differences from their mean (getScore) is as small as possible.
    *
    * The left child of the node will contain the upper left corner
    * of the node's rectangle, and the right child will contain the
    * lower right corner.
    *
    * This function will build the stats object used to score the 
    * splitting lines. It will also call helper function buildTree.
    */
twoDtree::twoDtree(PNG & imIn) {

	stats ourstats = stats(imIn);
	
	width = imIn.width();
	height = imIn.height();
	
	
	pair<int, int> topLeft = make_pair(0, 0);
	pair<int, int> bottomRight = make_pair(width - 1, height - 1);

	root = buildTree(ourstats, topLeft, bottomRight);
}

   /**
   * Private helper function for the constructor. Recursively builds
   * the tree according to the specification of the constructor.
   * @param s Contains the data used to split the rectangles
   * @param ul upper left point of current node's rectangle.
   * @param lr lower right point of current node's rectangle.
   */
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
		
	RGBAPixel averagepixel = s.getAvg(ul, lr);
	Node* currentnode = new Node(ul, lr, averagepixel);
		
	if (ul.first == lr.first && ul.second == lr.second) {
		return currentnode;
	}

	// single vertical column case
	if (ul.first == lr.first && lr.second - ul.second > 0) {

		pair<int, int> minleft;
		pair<int, int> minright;
		long minscore = -1;

		pair<int, int> newleft = make_pair(ul.first, ul.second + 1);
		pair<int, int> newright = make_pair(ul.first, ul.second);

		minscore = s.getScore(ul, newright) + s.getScore(newleft, lr);
		minleft = newleft;
		minright = newright;


		for (int i = newleft.second; i <= lr.second; i++) {
			long currentscore = s.getScore(ul, newright) + s.getScore(newleft, lr);

			if (currentscore <= minscore) {
				minscore = currentscore;
				minleft = newleft;
				minright = newright;
			}

			newleft.second = newleft.second + 1;
			newright.second = newright.second + 1;
		}

		currentnode->left = buildTree(s, ul, minright);
		currentnode->right = buildTree(s, minleft, lr);

		return currentnode;
	}

	// single horizontal row
	if (ul.second == lr.second && lr.first - ul.first > 0) {

		pair<int, int> minleft;
		pair<int, int> minright;
		long minscore = -1;

		pair<int, int> newleft = make_pair(ul.first + 1, ul.second);
		pair<int, int> newright = make_pair(ul.first, ul.second);

		minscore = s.getScore(ul, newright) + s.getScore(newleft, lr);
		minleft = newleft;
		minright = newright;

		for (int i = newleft.first; i <= lr.first; i++) {
			long currentscore = s.getScore(ul, newright) + s.getScore(newleft, lr);

			if (currentscore < minscore) {
				minscore = currentscore;

				minleft = newleft;
				minright = newright;
			}

			newleft.first = newleft.first + 1;
			newright.first = newright.first + 1;
		}

		currentnode->left = buildTree(s, ul, minright);
		currentnode->right = buildTree(s, minleft, lr);

		return currentnode;
	}
	

	pair<int, int> mintopleft;
	pair<int, int> minbottomright;
	long minscore = -1;

	pair<int, int> newtopleft = make_pair(ul.first+1, ul.second);
	pair<int, int> newbottomright = make_pair(ul.first, lr.second);

	if (ul.first != lr.first) {
		minscore = s.getScore(ul, newbottomright) + s.getScore(newtopleft, lr);
		mintopleft = newtopleft;
		minbottomright = newbottomright;
	}

	// vertical scores
	for (int i = newbottomright.first; i < lr.first; i++) {
		long currentscore = s.getScore(ul, newbottomright) + s.getScore(newtopleft, lr);

		if (currentscore < minscore) {
			minscore = currentscore;

			mintopleft = newtopleft;
			minbottomright = newbottomright;
		}

		newtopleft.first = newtopleft.first + 1;
		newbottomright.first = newbottomright.first + 1;
	}

	newtopleft.first = ul.first;
	newtopleft.second = ul.second+1;

	newbottomright.first = lr.first;
	newbottomright.second = ul.second;

	if (minscore == -1) {
		minscore = s.getScore(ul, newbottomright) + s.getScore(newtopleft, lr);
		mintopleft = newtopleft;
		minbottomright = newbottomright;
	}


	// horizontal scores
	for (int i = newbottomright.second; i < lr.second; i++) {
		long currentscore = s.getScore(ul, newbottomright) + s.getScore(newtopleft, lr);

		if (currentscore < minscore) {
			minscore = currentscore;

			mintopleft = newtopleft;
			minbottomright = newbottomright;
		}

		newtopleft.second = newtopleft.second + 1;
		newbottomright.second = newbottomright.second + 1;
	}

	currentnode->left = buildTree(s, ul, minbottomright);
	currentnode->right = buildTree(s, mintopleft, lr);

	return currentnode;

}

  /**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. may be used on pruned trees. Draws
    * every leaf node's rectangle onto a PNG canvas using the 
    * average color stored in the node.
    */
PNG twoDtree::render(){
	PNG pngee = PNG(width, height);
	
	renderHelper(root, pngee);
	return pngee;
}

void twoDtree::renderHelper(Node* curr, PNG& pngee){

	if (curr->left == NULL && curr->right == NULL){

		for (int j = curr->upLeft.first; j < curr->lowRight.first + 1; j++){
			for (int i = curr->upLeft.second; i < curr->lowRight.second + 1; i++){
				*(pngee.getPixel(j, i)) = curr->avg;
			}
		}
	}

	if (curr->left != NULL){
		renderHelper(curr->left, pngee);
	}


	if (curr->right != NULL){
		renderHelper(curr->right, pngee);
	}
}













 /*
    *  Prune function trims subtrees as high as possible in the tree.
    *  A subtree is pruned (cleared) 
	
	*  if at least (double pct) of its leaves are within (int tol) of the average color stored in the root of the subtree. 
    
	*  Pruning criteria should be evaluated on the original tree, not 
    *  on a pruned subtree. (we only expect that trees would be pruned once.)
    *  
    * You may want a recursive helper function for this one.
    */
void twoDtree::prune(double pct, int tol){
	if (root == NULL){
		return;
	}
	
	pruneHelper(root, pct, tol);
}

void twoDtree::countLeaves(Node * curr, Node* subRoot, int tol, double & numLeaves, double & goodLeaves){

	if (curr == NULL){
		return;
	}
	if (curr->left == NULL && curr->right == NULL ) {
		numLeaves++;
		if ((((curr->avg.r - subRoot->avg.r)*(curr->avg.r - subRoot->avg.r)) + ((curr->avg.g - subRoot->avg.g)*(curr->avg.g - subRoot->avg.g)) 
			+ ((curr->avg.b - subRoot->avg.b)*(curr->avg.b - subRoot->avg.b))) <= tol){
			goodLeaves++;
		}	
	} else {
		countLeaves(curr->left, subRoot, tol, numLeaves, goodLeaves);
		countLeaves(curr->right, subRoot, tol, numLeaves, goodLeaves);
	}	
}


void twoDtree::pruneHelper(Node* curr, double pct, int tol){
    double numLeaves = 0;
	double goodLeaves = 0;
	countLeaves(curr, curr, tol, numLeaves, goodLeaves);

	if ((goodLeaves / numLeaves) >= pct){
		clearHelper(curr->left);
		curr->left = NULL;
		clearHelper(curr->right);
		curr->right = NULL;
	}
	
	if (curr->left){
       pruneHelper(curr->left, pct, tol);
	}
    
	if (curr->right){
		pruneHelper(curr->right, pct, tol);
	}	
}










 /**
    * Destroys all dynamically allocated memory associated with the
    * current twoDtree class. Complete for PA3.
    * You may want a recursive helper function for this one.
    */
void twoDtree::clear() {
	clearHelper(root);
}

void twoDtree::clearHelper(Node* curr){
	if (curr == NULL){
		return;
	}

	clearHelper(curr->left);
	clearHelper(curr->right);
	
	delete curr;
	curr = NULL;
	
}














   /**
   * Copies the parameter other twoDtree into the current twoDtree.
   * Does not free any memory. Called by copy constructor and op=.
   * You may want a recursive helper function for this one.
   * @param other The twoDtree to be copied.
   */
void twoDtree::copy(const twoDtree & orig){
	width = orig.width;
	height = orig.height;
	this->root = copyHelper(orig.root);
}

twoDtree::Node * twoDtree::copyHelper(Node* orig){

	if (orig == NULL){
		return NULL;
	}
	
	Node* newNode = new Node(orig->upLeft, orig->lowRight, orig->avg);
	newNode->left = copyHelper(orig->left);
	newNode->right = copyHelper(orig->right);
	
	return newNode;
	
}
