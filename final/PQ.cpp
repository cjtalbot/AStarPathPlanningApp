// Christine Talbot
//
// Class Name(s):
//		PQ
//
// Description:
//		Implementation of Priority Queue class which provides the min
//		extract as well as deletions 
//
// Method Summary:
//		PQ();
//			constructor
//		~PQ();
//			destructor
//		void push(int s);
//			adds s to the priority queue in the correct priority order
//		int peek();
//			lets you see which item is next in the queue without removing it
//		void erase(int s, float val);
//			removes the item s if it has a greater value than val
//		void erase(int s);
//			removes the item s
//		void clear();
//			cleans up the priority queue
//		int size();
//			returns how big the priority queue is
//		string toString();
//			formats the priority queue for printing
//
// -------------------------------------------------------------------------------------------
#include <vector>
#include "State.h"
#include "PQ.h"
#include "ai.h"
#include <sstream>
#include <iostream>

PQ::PQ()
{
	// constructor
	slist.resize(0);
	slist.clear();
}
PQ::~PQ()
{
	// destructor
	slist.resize(0);
	slist.clear();
}
void PQ::push(int s)
{

	int loc = 0;
	bool found = false;
	// check size of queue so don't hit poitner issues
	if (slist.size() <= 1)
	{
		if(slist.size() == 1)
		{

			int b = slist[0];
			// add s to the right spot in the vector
			if(AI::closed[b]->getfn() > AI::closed[s]->getfn())
				slist.insert(slist.begin(), s);
			else
				slist.push_back(s);
		}
		else
			slist.push_back(s);
		found = true;
	}
	// if multiple items in queue now
	else
	{
		// go through all items in the list
		for (vector<int>::iterator i = slist.begin(); i != slist.end(); i++)
		{
			if (i != slist.end())
			{
				int c = *i;
				// check if tihs is the right spot to insert
				if (AI::closed[c]->getfn() > AI::closed[s]->getfn())
				{

					slist.insert(slist.begin() + loc, s);
					found = true;
					break;
				}
			}
			loc++;

		}
	}
	if (!found)
	{
		// add to the end if doen't belong earlier on
		slist.push_back(s);
	}
}

int PQ::peek()
{
	// gets the first item in the list
	int s = slist[0];
	return s;
}
void PQ::erase(int s, float val)
{
	// go through each item in the queue
	int loc = 0;
	for (vector<int>::iterator i = slist.begin(); i != slist.end(); i++)
	{
		if (i != slist.end())
		{
			int c = *i;
			// if this is the item we're looking for and it's less than the new value, then delete
			if (s == c and AI::closed[c]->getgn() > val)// only delete if greater than new value
			{
				slist.erase(slist.begin() + loc, slist.begin() + loc + 1);
				break;
			}
		}
		loc++;
	}
}
void PQ::erase(int s)
{
	int loc = 0;
	// check if 0 or 1 item in queue to help with pointer issues
	if (slist.size() <= 1)
	{
		if(slist.size() == 1)
		{
			int b = slist[0];
			if(b == s) // found item, so delete it
				slist.erase(slist.begin());
		}
	}
	else
	{
		// go throuh all items in list
		for (vector<int>::iterator i = slist.begin(); i != slist.end(); i++)
		{
			if (i != slist.end())
			{
				int c = *i;
				if (s == c)
				{
					if(loc == 0) // delete item if found
						slist.erase(slist.begin());
					else
						slist.erase(slist.begin() + loc, slist.begin() + loc + 1);
					break;
				}
			}
			loc++;
		}
	}
}
string PQ::toString()
{
	// format string for printing
	stringstream retstring;
	retstring << "Priority Queue holds:\n";
	for (vector<int>::iterator i = slist.begin(); i!=slist.end();i++)
	{
		if(i!=slist.end())
		{
			int c = *i;
			retstring << "State=" << AI::closed[c]->toString() << "\n";
		}
	}
	retstring << "End of priority queue\n";
	return retstring.str();
}
int PQ::size()
{
	return slist.size();
}
void PQ::clear()
{
	slist.resize(0);
	slist.clear();
}
