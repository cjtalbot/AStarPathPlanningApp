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
#ifndef PQ_H_
#define PQ_H_

#include <vector>
#include "State.h"
#include <string>

class PQ
{

	public:
		vector<int> slist;
		PQ();
		~PQ();
		void push(int s);
		int peek();
		void erase(int s, float val);
		void erase(int s);
		void clear();
		int size();
		string toString();

};


#endif /* PQ_H_ */
