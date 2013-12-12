#ifndef _JUNEKI_ENGLISH_DEPENDENCY_PARSER_AUTOMATASTATE
#define _JUNEKI_ENGLISH_DEPENDENCY_PARSER_AUTOMATASTATE

#include <vector>
#include <iostream>
#include <math.h>
//#include "depparser_macros.h"

//#include "costFunction.h"

//namespace TARGET_LANGUAGE {
//
//namespace depparser {

enum AutomataAction {
	TRANSITION = 1, TAKEPARENT = 2, FLIP = 3, HALT = 4, NULLACTION
};

//TODO this typedef comes from depparser_macros.h. Figure out how to get the compiler to recognize it.
typedef long SCORE_TYPE;

//TODO figure out file IO. read in action costs from it.
//class costFunction;

class StateHistory {
public:
	AutomataAction action;
	int actionState;
	SCORE_TYPE cost;

	StateHistory(AutomataAction a, int state, int cost);
};

/** Head Automaton class. Represents head automaton that can be used while parsing to extract richer features during a parse.
 * By Juneki
 */
class State {
public:

#ifdef AUTOMATA_1ST_FEATURES
	bool leftState;
	bool rightState;
	bool flipped;
	bool hasLeftParent;
#endif

#ifdef AUTOMATA_2ND_FEATURES
	bool halted;
//	int leftParent;
//	int rightParent;
	int POStag;
#endif
//
//	std::vector<int> inputSymbolBuffer;
//	std::vector<StateHistory> history;

//	costFunction* function;

	int uniqueID;
	unsigned long int hashCache;

//Main operation functions
public:
	/** We take another child as input and update our state.*/
	SCORE_TYPE transition(State* childNode);

	/** We take a node to be our parent and update our score.*/
	SCORE_TYPE takeParent(State* parentNode);

	/** Called when there are no more right children.*/
	SCORE_TYPE halt();

	/** Changes the automaton to begin taking right children.*/
	SCORE_TYPE flip();

public:
	// Clears the state
	void clearState();

// constructors and destructor. Getters and Setters. Operator Overloads.
public:
	State();

//	State(costFunction *f){
//		function = f;
//		clearState();
//	}

	~State();

	void operator =(const State &item);

	bool operator ==(const State &item) const;

	bool operator !=(const State &item) const;

	bool operator <(const State &item) const;

public:
	void recomputeHash();

	//Produces an integer that represents the current state we are in. This is passed off to zpar to help it train and parse.
	unsigned long int hash() const;

	void clearHistory();

	// print and debug functions
	void print() const;
};

std::ostream & operator <<(std::ostream &os, const State &s);
std::istream& operator>>(std::istream& is, State& s);

//}; // namespace depparser
//};

#endif
