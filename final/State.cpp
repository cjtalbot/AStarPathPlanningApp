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
#include "Node.h"
#include <string>
#include <math.h>
#include <sstream>
#include <iostream>
#include "State.h"
#include <cfloat>
#include "ai.h"

GLWidget::Point State::goal;

State::State(float x1, float y1, int p, int c)
{

	x = x1;
	y = y1;
	curloc = c;
	// if no parent, g(n) = 0, else calculate it based on the parent cost & distance from parent
	if (p == -1)
		gn = 0;
	else
		gn = AI::closed[p]->getgn() + sqrt(((x - AI::closed[p]->getx())*(x - AI::closed[p]->getx())) + ((y - AI::closed[p]->gety())*(y - AI::closed[p]->gety())));
	hn = sqrt(((goal.x - x)*(goal.x - x)) + ((goal.y - y)*(goal.y - y)));

	parent = p;
	
}


State::State()
{
	// default constructor
	x = FLT_MAX;
	y = FLT_MAX;
	gn = FLT_MAX;
	hn = FLT_MAX;

	parent = -1;
	curloc = -1;
}
State::~State()
{
	//delete parent;
}
int  State::getparent()
{
	return parent;
}
Node::Node State::getn()
{
	Node* n = new Node(x, y);
	return *n;
}
float State::getx()
{
	return x;
}
float State::gety()
{
	return y;
}
float State::getfn()
{
	return gn + hn;
}
float State::getgn()
{
	return gn;
}
float State::gethn()
{
	return hn;
}
string State::toString()
{
	// formats string
	stringstream retstring;
	retstring << "State Position = (" << x << ", " << y << "), g(n)=" << gn << ", h(n)=" << hn << ", f(n)=" << getfn() << ", Parent=" << ((parent==-1)?"NULL--":"Value--") << ((parent != -1)? AI::closed[parent]->getx():0) << ", " << ((parent != -1)? AI::closed[parent]->gety():0);
	return retstring.str();
}
bool State::isgoal()
{
	// returns if same point as goal
	if (x == goal.x and y == goal.y)
		return true;
	return false;
}

State& State::operator=(const State& s) 
{
	if(this == &s)
		return *this;
	
	parent = -1;
	// copies all values to this object
	x = s.x;
	y = s.y; 
	gn = s.gn;
	hn = s.hn;
	parent = s.parent;
	curloc = s.curloc;
	return *this;
}
string State::printparents()
{
	// formats string of parents
	stringstream retstring;
	retstring << "PARENTS:\n";
	retstring << "Node=(" << x << ", " << y << "), gn=" << gn << ", hn=" << hn << "\n";
	int s = parent;
	// go through parents untnil none left
	while(s != -1 and AI::closed[s]->getx() != FLT_MAX)
	{
		retstring << "Node=("  << x << ", " << y << "), gn=" << AI::closed[s]->getgn() << ", hn=" << AI::closed[s]->gethn() << "\n";
		s = AI::closed[s]->getparent();
	}
	return retstring.str();
}
		
bool State::operator==(const State &s1) const
{
	// check if same point
	if(s1.x == x and s1.y == y)
		return true;
	return false;
}

// used for comparing - not used since not using real priority queue
    bool CompareStates::operator()(State& s1, State& s2)
    {
      // if (s1.cost < s2.cost) return true;
       if ( s1.getfn() > s2.getfn() )
         {
            return true;
         }
         else if ( s1.getfn() < s2.getfn() )
         {
            return false;
         }
         else
         {
            if ( s1.getgn() > s2.getgn() )
            {
               // take ones with smaller actuals if the estimated total is the same
               return true;
            }
            else if ( s1.getgn() < s2.getgn() )
            {
               return false;
            }
            else
            {
               // f( n ), g( n ), and h( n ) are equal, so pick whichever you want
               return false;
            }
            // return 0;
         }
    }

