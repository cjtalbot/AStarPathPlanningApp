// Christine Talbot
//
// Class Name(s):
//		Node
//
// Description:
//		Implementation of Node class which provides the "points" in the graph
//
// Method Summary:
// 		Node();
//			constructor
//		Node(float x, float y);
//			constructor
//		Node(const Node &n);
//			constructor
//		void setx(float a);
//			sets the x coordinate to a
//		void sety(float a);
//			sets the y coordinate to a
//		virtual ~Node();
//			destructor
//		string toString();
//			formats the node for printing
//		bool operator==(const Node &n1) const;
//			overloads the == operator for comparing two nodes
//		Node & operator=(const Node &n);
//			overloads the = operator for copying / assigning a node to this one
//		float getx();
//			returns the x coordinate
//		float gety();
//			returns the y coordinate
//
// -------------------------------------------------------------------------------------------

#include "Node.h"
#include <sstream>
#include <string>
using namespace std;

Node::Node(float a, float b) {
	// initialize the point
	x = a;
	y = b;
}

Node::~Node() {
	// destructor
}
Node::Node(const Node &n)
{
	//constructor
	x = n.x;
	y = n.y;
}
Node::Node()
{
	// do nothing
}
void Node::setx(float a)
{
	//sets x
	x = a;
}
void Node::sety(float a)
{
	// sets y
	y = a;
}

string Node::toString()
{
	// formats point for printing
	stringstream retstring;
	retstring << "(" << x << ", " << y << ")";
	return retstring.str();
}
bool Node::operator==(const Node &n1) const
{
	// check if the two points are the same
	if(n1.x == x and n1.y == y)
		return true;
	return false;
}
Node& Node::operator=(const Node &n)
{
	// copy the point to this one
	if(this == &n)
		return *this;
	x = n.x;
	y = n.y;
	return *this;
}
float Node::getx()
{
	return x;
}
float Node::gety()
{
	return y;
}
