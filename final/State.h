// Christine Talbot
//
// Class Name(s):
//		State
//
// Description:
//		Implementation of State class which provides the point, current
//		cost, and estimated future cost for expanded & generated nodes
//		during A* algorithm
//
// Method Summary:
// 		State(float x, float y, int p, int c);
//			constructor
//		State();
//			constructor
//		~State();
//			destructor
//		float getfn();
//			calculates the sum of g(n) + h(n) for total estimated cost
//		float getgn();
//			returns the current cost to get here
//		float gethn();
//			returns the estimated cost to get from here to the goal
//		string toString();
//			prints a formatted version of this state
//		bool isgoal();
//			returns whether this state is the same as the goal state or not
//		State& operator=(const State& s);
//			overloads the = operator to copy the state
//		float getx();
//			gets the x coordinate
//		float gety();
//			gets the y coordinate
//		int getparent();
//			gets the parent for this node (which point we came from)
//		Node getn();
//			returns the node / point for this state
//		bool operator==(const State &s1) const;
//			overloads the == operator to allow comparisons for equality
//		string printparents();
//			for debugging - prints the parent hierarchy for this state
//
// -------------------------------------------------------------------------------------------
#ifndef STATE_H
#define STATE_H

#include "Node.h"
#include "glWidget.h"
#include <string>
#include <map>

class State
{
	private:

		float x;
		float y;
		float gn;
		float hn;
		int curloc;
		int parent; // int location of the parent in the map

	public:
		State(float x, float y, int p, int c);

		State();
		~State();
		float getfn();
		float getgn();
		float gethn();
		string toString();
		static GLWidget::Point goal;
		bool isgoal();
		State& operator=(const State& s);
		float getx();
		float gety();
		int getparent();
		Node getn();
		bool operator==(const State &s1) const;
		string printparents();
};


class CompareStates {
public:
    bool operator()(State& s1, State& s2);

    
};
#endif // STATE_H
