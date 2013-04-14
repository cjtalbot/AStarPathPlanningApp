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

#ifndef NODE_H_
#define NODE_H_

#include <string>

using namespace std;

class Node {
	private:
		float x;
		float y;
		//friend class State;
	public:
		Node();
		Node(float x, float y);
		Node(const Node &n);
		void setx(float a);
		void sety(float a);
		virtual ~Node();
		string toString();
		bool operator==(const Node &n1) const;
		Node & operator=(const Node &n);
		float getx();
		float gety();
};

#endif /* NODE_H_ */
