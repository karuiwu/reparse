#ifndef _JUNEKI_ENGLISH_DEPENDENCY_PARSER_AUTOMATASTATE
#define _JUNEKI_ENGLISH_DEPENDENCY_PARSER_AUTOMATASTATE

#include <math.h>

enum AutomataAction {
	TRANSITION = 1, TAKEPARENT = 2, FLIP = 3, HALT = 4, NULLACTION
};

class StateHistory {
public:
	AutomataAction action;
	int actionState;

	StateHistory() {
		action = NULLACTION;
		actionState = -1;
	}
	StateHistory(AutomataAction a, int state) {
		action = a;
		actionState = state;
	}
};

/** Head Automaton class. Represents head automaton that can be used while parsing to extract richer features during a parse.
 * By Juneki
 */
class State {
public:
	double cost;
	bool leftState;
	bool rightState;

	bool hasLeftParent;
	int leftParent;
	int rightParent;

	bool flipped;
	bool halted;

	int POStag;
	std::vector<int> inputSymbolBuffer;
	std::vector<StateHistory> history;

	int uniqueID;

//Main operation functions
public:
	/** We take another child as input and update our state.
	 *
	 */
	State* transition(State* childNode) {
		if (!flipped) {
			leftState = true;
		} else {
			rightState = true;
		}
		inputSymbolBuffer.push_back(childNode->POStag);

		history.push_back(StateHistory(TRANSITION,hash()));
		return this;
	}

	/** We take a node to be our parent and update our score.
	 *
	 */
	State* takeParent(State* parentNode) {
		if (parentNode->flipped) {
			leftParent = parentNode->POStag;
			hasLeftParent = true;
		} else {
			rightParent = parentNode->POStag;
		}

		history.push_back(StateHistory(TAKEPARENT,hash()));
		return this;
	}

	/** Called when there are no more right children.
	 *
	 */
	State* halt() {
		halted = true;

		history.push_back(StateHistory(HALT,hash()));
		return this;
	}

	/** Changes the automaton to begin taking right children.
	 *
	 */
	State* flip() {

		flipped = true;

		// I am encoding everything as an int. Jason designated the special flip symbol as a #. I'll designate it as 33.
		inputSymbolBuffer.push_back(33);

		history.push_back(StateHistory(FLIP,hash()));
		return this;
	}

public:
	// Clears the state
	void clearState() {
		cost = 0;
		leftState = false;
		rightState = false;

		hasLeftParent = false;
		leftParent = 0;
		rightParent = 0;

		flipped = false;
		halted = false;

		POStag = 99;
		inputSymbolBuffer.clear();
		history.clear();

	}

// constructors and destructor. Getters and Setters. Operator Overloads.
public:
	State() {

		uniqueID = 0;

		clearState();
	}
	~State() {
	}
//	State(State& item) {
//
//	}

	void operator =(const State &item) {
		cost = item.cost;
		leftState = item.leftState;
		rightState = item.rightState;

		hasLeftParent = item.hasLeftParent;
		leftParent = item.leftParent;
		rightParent = item.rightParent;

		flipped = item.flipped;
		halted = item.halted;
		POStag = item.POStag;
		inputSymbolBuffer = item.inputSymbolBuffer;

		history = item.history;
		uniqueID = item.uniqueID;

	}

public:
	//Produces an integer that represents the current state we are in. This is passed off to zpar to help it train and parse.
	int hash() const {
		// Automata state options
		bool OPTIONNULL = false;
		bool OPTION2STATES = true;
		bool OPTION4TUPLE = false;
		bool OPTIONINPUTSEQUENCE_POS = false;
		bool OPTIONINPUTSEQUENCE_NGRAM = false;
		bool OPTIONINPUTSEQUENCE_LAST = true;

		int hash = 0;
		if (OPTIONNULL) {
		} else if (OPTION2STATES) {
			hash += leftState;
			hash += 10 * rightState;
		} else if (OPTION4TUPLE) {
			hash += leftState;
			hash += 10 * hasLeftParent;
			hash += 100 * flipped;
			hash += 1000 * rightState;
		} else if (OPTIONINPUTSEQUENCE_POS) {
			int offset = pow(100, inputSymbolBuffer.size());

			hash += (leftParent + 1) * offset * 10000;
			hash += (rightParent) * offset * 100;

			int i = 0;
			for (; i < inputSymbolBuffer.size(); i++) {
				hash += offset * inputSymbolBuffer.at(i);
				offset /= 100;
			}
			hash += offset * POStag;
		} else if (OPTIONINPUTSEQUENCE_NGRAM) {
			int NGRAM = 3;
			int offset = pow(100, NGRAM);

			hash += (leftParent + 1) * offset * 100;

			int i = inputSymbolBuffer.size() - NGRAM;
			if (i < 0) {
				i = 0;
			}
			for (; i < inputSymbolBuffer.size(); i++) {
				hash += offset * inputSymbolBuffer.at(i);
				offset /= 100;
			}
			hash += offset * POStag;
		} else if (OPTIONINPUTSEQUENCE_LAST) {
			hash += leftParent;
			hash += 100 * POStag;
			if (!inputSymbolBuffer.empty()) {
				hash += 10000 * inputSymbolBuffer.back();
			}
		}

		return hash;
	}

	void clearHistory() {
		history.clear();
	}

	// print and debug functions
	void print() const {

		std::cerr << "[" << "{" << uniqueID << "}" << leftState << "|"
				<< rightState << "*" << flipped << "* " << "("; //<< hash() << ") ]";

		for (int i = 0; i < inputSymbolBuffer.size(); i++) {
			std::cerr << inputSymbolBuffer.at(i) << " ";
		}
		std::cerr << "|" << POStag << " : " << hash() << ") ]";

	}

};

#endif
