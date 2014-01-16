#include "automataState.h"

StateHistory::StateHistory(AutomataAction a, int state, int cost) {
	action = a;
	actionState = state;
	this->cost = cost;
}

State::State() {
//		uniqueID = 0;
//		costFunction f = costFunction();
//		function = &f;
	clearState();
}

State::~State() {

}

// Clears the state
void State::clearState() {

#ifdef AUTOMATA_1ST_FEATURES
	leftState = false;
	rightState = false;
	flipped = false;
	hasLeftParent = false;
#endif

#ifdef AUTOMATA_2ND_FEATURES
	halted = false;
//	leftParent = 0;
//	rightParent = 0;
	POStag = 99;
#endif

//	inputSymbolBuffer.clear();
//	history.clear();

	recomputeHash();

}

SCORE_TYPE State::transition(State* childNode) {

	SCORE_TYPE cost = 1;

#ifdef AUTOMATA_1ST_FEATURES
	if (!flipped) {
		leftState = true;
	} else {
		rightState = true;
	}
#endif

	//	inputSymbolBuffer.push_back(childNode->POStag);

//	history.push_back(StateHistory(TRANSITION, hash(), cost));

//		cost = function->getCost(TRANSITION, this);
	recomputeHash();
	return cost;
}

SCORE_TYPE State::takeParent(State* parentNode) {
	SCORE_TYPE cost = 1;

#ifdef AUTOMATA_1ST_FEATURES
	if (parentNode->flipped) {
//		leftParent = parentNode->POStag;
		hasLeftParent = true;
	} else {
//		rightParent = parentNode->POStag;
	}
#endif

#ifdef AUTOMATA_2ND_FEATURES
//	if (parentNode->flipped) {
//		leftParent = parentNode->POStag;
//	} else {
//		rightParent = parentNode->POStag;
//	}
#endif

//	history.push_back(StateHistory(TAKEPARENT, hash(), cost));

//		cost = function->getCost(TAKEPARENT, this);
	recomputeHash();
	return cost;
}

SCORE_TYPE State::halt() {
	SCORE_TYPE cost = 1;
#ifdef AUTOMATA_2ND_FEATURES
	halted = true;
#endif

//	history.push_back(StateHistory(HALT, hash(), cost));

//		cost = function->getCost(HALT, this);
	recomputeHash();
	return cost;
}

SCORE_TYPE State::flip() {
	SCORE_TYPE cost = 1;
#ifdef AUTOMATA_1ST_FEATURES
	flipped = true;
#endif

	// I am encoding everything as an int. Jason designated the special flip symbol as a #. I'll designate it as 33.
//	inputSymbolBuffer.push_back(33);

//	history.push_back(StateHistory(FLIP, hash(), cost));

//		cost = function->getCost(FLIP, this);
	recomputeHash();
	return cost;
}

unsigned long int State::hash() const {
	return hashCache;
}

void State::recomputeHash() {
	unsigned long int hash = 0;

#ifdef AUTOMATA_1ST_FEATURES
	hash += leftState;
	hash += 10 * hasLeftParent;
	hash += 100 * flipped;
	hash += 1000 * rightState;
#endif

#ifdef AUTOMATA_2ND_FEATURES
	hash += 10000 * halted;
//	hash += 1000000 * leftParent;
//	hash += 100000000 * rightParent;
	hash += 1000000 * POStag;
#endif

//	else if (OPTIONINPUTSEQUENCE_POS) {
//		int offset = pow(100, inputSymbolBuffer.size());
//
//		hash += (leftParent + 1) * offset * 10000;
//		hash += (rightParent) * offset * 100;
//
//		int i = 0;
//		for (; i < inputSymbolBuffer.size(); i++) {
//			hash += offset * inputSymbolBuffer.at(i);
//			offset /= 100;
//		}
//		hash += offset * POStag;
//	}
//	else if (OPTIONINPUTSEQUENCE_NGRAM) {
//		int NGRAM = 3;

//			hash += (leftState);
//			hash += (hasLeftParent) * 10;
//			hash += (flipped) * 100;
//			hash += (rightState) * 1000;

//			hash += (leftParent + 1) * offset * 100;
//		int offset = pow(100, NGRAM) * 1000;

//		int i = inputSymbolBuffer.size() - NGRAM;
//		if (i < 0) {
//			i = 0;
//		}
//		for (; i < inputSymbolBuffer.size(); i++) {
//			hash += offset * inputSymbolBuffer.at(i);
//			offset /= 100;
//		}

//			hash += 100000 * POStag;

//	} else if (OPTIONINPUTSEQUENCE_LAST) {
//		hash += leftParent;
//		hash += 100 * POStag;
//		if (!inputSymbolBuffer.empty()) {
//			hash += 10000 * inputSymbolBuffer.back();
//		}
//	}
	hashCache = hash;
}

void State::clearHistory() {
//	history.clear();
}

void State::print() const {

	std::cerr << "[" << "{" << uniqueID << "}";
#ifdef AUTOMATA_1ST_FEATURES
	std::cerr << leftState << "|" << rightState
	<< "*" << flipped << "* ";
#endif
#ifdef AUTOMATA_2ND_FEATURES

	std::cerr << halted << "|";
//	std::cerr << leftParent << "|";
//	std::cerr << rightParent << "|";
	std::cerr << POStag;
#endif
	std::cerr << "(" << hash() << ") ]";

//	for (int i = 0; i < inputSymbolBuffer.size(); i++) {
//		std::cerr << inputSymbolBuffer.at(i) << " ";
//	}
//	std::cerr << "|" << POStag << " : " << hash() << ") ]";

}

void State::operator =(const State &item) {
#ifdef AUTOMATA_1ST_FEATURES
	leftState = item.leftState;
	rightState = item.rightState;
	flipped = item.flipped;
	hasLeftParent = item.hasLeftParent;
#endif
#ifdef AUTOMATA_2ND_FEATURES
//	leftParent = item.leftParent;
//	rightParent = item.rightParent;
	halted = item.halted;
	POStag = item.POStag;

#endif
//	inputSymbolBuffer = item.inputSymbolBuffer;

//	history = item.history;
	hashCache = item.hashCache;
	uniqueID = item.uniqueID;
}

bool State::operator <(const State &item) const {
	bool result = true;

#ifdef AUTOMATA_1ST_FEATURES
	result = result && (leftState < item.leftState) && (rightState < item.rightState) && (hasLeftParent < item.hasLeftParent) && (flipped < item.flipped);
#endif

#ifdef AUTOMATA_2ND_FEATURES
	result = result && (halted < item.halted) && (POStag < item.POStag);
#endif
//			&& leftParent < item.leftParent && rightParent < item.rightParent
//			&& inputSymbolBuffer < item.inputSymbolBuffer;
	;
}

bool State::operator ==(const State &item) const {
	bool result = true;

#ifdef AUTOMATA_1ST_FEATURES
	result = result && (leftState == item.leftState) && (rightState == item.rightState) && (hasLeftParent == item.hasLeftParent) && (flipped == item.flipped);
#endif

#ifdef AUTOMATA_2ND_FEATURES
	result = result && (halted == item.halted) && (POStag == item.POStag);
//	 leftParent == item.leftParent
//	&& rightParent == item.rightParent
#endif
//			&& inputSymbolBuffer == item.inputSymbolBuffer
//			&& history == item.history
	;
}

bool State::operator !=(const State &item) const {
	return !(State::operator ==(item));
}

std::ostream & operator <<(std::ostream &os, const State &s) {

#ifdef AUTOMATA_1ST_FEATURES
	os << s.leftState << " " << s.rightState << " " << s.flipped << " " << s.hasLeftParent;
//	<< s.halted;
#endif

#ifdef AUTOMATA_2ND_FEATURES
	os << " " << s.halted << " " << s.POStag;
#endif
	return os;
}

std::istream& operator >>(std::istream& is, State& s) {

#ifdef AUTOMATA_1ST_FEATURES
	is >> s.leftState;
	is >> s.rightState;
	is >> s.flipped;
	is >> s.hasLeftParent;
#endif

#ifdef AUTOMATA_2ND_FEATURES
	is >> s.halted;
	is >> s.POStag;
#endif

	return is;
}

