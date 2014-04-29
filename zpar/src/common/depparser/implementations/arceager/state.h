// Copyright (C) University of Oxford 2010
#ifndef _ENGLISH_DEPENDENCY_PARSER_STATEITEM
#define _ENGLISH_DEPENDENCY_PARSER_STATEITEM

#include "automataState.h"

namespace TARGET_LANGUAGE {
namespace depparser {

/*===============================================================
 *
 * CStateItem - the search state item, representing a partial
 *              candidate with shift reduce. 
 *
 * Note abstd::cout members: there are two types of item properties;
 * The first is the stack, which changes during the process; 
 * The second is the incoming sentence, including m_lHeads, 
 * m_lDepsL and m_lDepsR, m_lDepNumL, m_lDepNumR etc, which 
 * records the properties of each input word so far.
 *
 * A state item is partial and do not include information abstd::cout
 * all words from the input sentence. Though m_lHeads, m_lDepsL 
 * and m_lDepsR are fixed arrays with MAX_SENTENCE_SIZE length, 
 * not all the elements from the arrays are used. The ACTIVE 
 * elements are from the input index 0 to m_nNextWord, inclusive. 
 * And a state item only captures information abstd::cout the active 
 * sub section from input.
 *
 * The property for each input word need to be initialised. 
 * m_lHeads m_lDepsL etc could be initialised within the 
 * clear() method. However, because the parsing process is
 * incremental, they can also be initialised lasily. 
 * Apart from the avoidance of unecessary assignments, one 
 * benefit of lazy initialisation is that we only need to copy
 * the active indexies when doing a copy operation. Therefore, 
 * this implementation takes the approach. 
 * The initialisation of these values are in clear(), shift()
 * and arcright()
 *
 *==============================================================*/

class CStateItem {

public:
	enum STACK_STATUS {
		OFF_STACK = 0, ON_STACK_SHIFT, ON_STACK_ARCRIGHT
	};

protected:
	std::vector<int> m_Stack;     // stack of words that are currently processed
	std::vector<int> m_HeadStack;
	int m_nNextWord;                         // index for the next word

#ifdef DEPENDENCIES
	int m_lHeads[MAX_SENTENCE_SIZE];         // the lexical head for each word
	int m_lDepsL[MAX_SENTENCE_SIZE];// the leftmost dependency for each word (just for cache, temporary info)
	int m_lDepsR[MAX_SENTENCE_SIZE];// the rightmost dependency for each word (just for cache, temporary info)
	int m_lSibling[MAX_SENTENCE_SIZE];// the sibling towards head
#ifdef LABELED
	unsigned long m_lLabels[MAX_SENTENCE_SIZE]; // the label of each dependency link
#endif
#endif

#ifdef LINKS
	std::vector<std::vector<int> > m_lHeads;
	std::vector<std::vector<int> > m_lDepsL;
	std::vector<std::vector<int> > m_lDepsR;
	std::vector<std::vector<int> > m_lSibling;
#ifdef LABELED
	std::vector<std::vector<unsigned long> > m_lLabels;
#endif
#endif

	CSetOfTags<CDependencyLabel> m_lDepTagL[MAX_SENTENCE_SIZE]; // the set of left tags
	CSetOfTags<CDependencyLabel> m_lDepTagR[MAX_SENTENCE_SIZE]; // the set of right tags
	int m_lDepNumL[MAX_SENTENCE_SIZE]; // the number of left dependencies
	int m_lDepNumR[MAX_SENTENCE_SIZE]; // the number of right dependencies

	unsigned long m_nLastAction;                  // the last stack action
	const std::vector<CTaggedWord<CTag, TAG_SEPARATOR> >* m_lCache;

public:
	SCORE_TYPE score; // score of stack - predicting how potentially this is the correct one

	std::map<int, std::vector<int> > m_Children;

	std::vector<int> Stack;
	int NextWord;

	//Edited by J

	// Keeps track of the states of all the tokens in the sentence.
	// Each token on the sentence will have an assigned number designating whether it is on the stack or has been reduced off.
	int QueueStackReduceState[MAX_SENTENCE_SIZE];

	// Contains all of the head automata for every node in a sentence.
	State linkAutomata[MAX_SENTENCE_SIZE];

	//End

public:
	// constructors and destructor
	CStateItem(const std::vector<CTaggedWord<CTag, TAG_SEPARATOR> >*cache = 0) :
			m_lCache(cache) {

#ifdef LINKS
		for(unsigned i=0; i < MAX_SENTENCE_SIZE; i++){
			m_lHeads.push_back(std::vector<int>());
			m_lDepsL.push_back(std::vector<int>());
			m_lDepsR.push_back(std::vector<int>());
			m_lSibling.push_back(std::vector<int>());
#ifdef LABELED
			m_lLabels.push_back(std::vector<unsigned long>());
#endif
		}
#endif


		clear();

		// Edited by J

		// I want to give the automata for each token in the sentence to receive the respective POS tag. In integer form.
//		if (m_lCache != 0) {
//			for (int i = 0; i < m_lCache->size() && i < 10; i++) {
//				CTaggedWord<CTag, TAG_SEPARATOR> taggedWord = m_lCache->at(i);
//				const CTag &tag = taggedWord.tag;
//				std::string tagString = tag.str();
//
//				int sum = 0;
//				int offset = 1;
//				for (int j = 0; j < tagString.size() && j < 3; j++) {
//					int temp = tagString[j];
//					sum += temp * offset;
//					offset *= 100;
//				}
//				linkAutomata[i].setPOStag(sum);
//			}
//		}

	}
	~CStateItem() {
	}
	CStateItem(CStateItem& item) :
			m_lCache(0) {
		std::cerr << "CStateItem does not support copy constructor!";
		std::cerr.flush();
		assert(1 == 0);
	}

public:
	// comparison
	inline bool operator <(const CStateItem &item) const {
		return score < item.score;
	}
	inline bool operator >(const CStateItem &item) const {
		return score > item.score;
	}
	inline bool operator ==(const CStateItem &item) const {
		int i;
		if (m_nNextWord != item.m_nNextWord) {
			return false;
		}

		for (i = 0; i < m_nNextWord; ++i) {

			// check if the heads all match up.
#ifdef DEPENDENCIES
			if (m_lHeads[i] != item.m_lHeads[i]) {
				return false;
			}
#endif
#ifdef LINKS
			// for links, check if all the link heads match up.
			if(m_lHeads[i].size() != item.m_lHeads[i].size()) {
				return false;
			}

			unsigned j;
			for(j = 0; j < m_lHeads[i].size(); j++) {
				if(m_lHeads[i][j] != item.m_lHeads[i][j]) {
					return false;
				}
			}
#endif

			// check if all the labels match up
#ifdef LABELED
#ifdef DEPENDENCIES
			if ( m_lLabels[i] != item.m_lLabels[i] ) {
				return false;
			}
#endif
#ifdef LINKS
			// for links, check if all the link labels match up.
			if(m_lLabels[i].size() != item.m_lLabels[i].size()) {
				return false;
			}

			for(j = 0; j < m_lLabels[i].size(); j++) {
				if(m_lLabels[i][j] != item.m_lLabels[i][j]) {
					return false;
				}
			}
#endif
#endif
		}
		if (m_Stack.size() != item.m_Stack.size())
			return false;
		if (m_Stack.size() > 0 && m_Stack.back() != item.m_Stack.back())
			return false;

		// I think that the stacks don't have to be compared
		// might be proved by translating tree to stack
		assert(m_Stack == item.m_Stack);
		assert(m_HeadStack == item.m_HeadStack);
		return true;
	}
	inline bool operator !=(const CStateItem &item) const {
		return !((*this) == item);
	}

	// propty
	inline int stacksize() const {
		return m_Stack.size();
	}
	inline bool stackempty() const {
		return m_Stack.empty();
	}
	inline int stacktop() const {
		assert(!m_Stack.empty());
		return m_Stack.back();
	}
	inline int stackbottom() const {
		assert(!m_Stack.empty());
		return m_Stack.front();
	}
	inline int stackitem(const unsigned &index) const {
		assert(index < m_Stack.size());
		return m_Stack[index];
	}
	inline bool headstackempty() const {
		return m_HeadStack.empty();
	}
	inline int headstacktop() const {
		assert(!m_HeadStack.empty());
		return m_HeadStack.back();
	}
	inline int headstackitem(const unsigned &index) const {
		assert(index < m_HeadStack.size());
		return m_HeadStack[index];
	}
	inline int headstacksize() const {
		return m_HeadStack.size();
	}

	inline bool afterreduce() const {
#ifdef LABELED
		return action::getUnlabeledAction(m_nLastAction)==action::REDUCE;
#else
		return m_nLastAction == action::REDUCE;
#endif
	}

	inline unsigned long lastAction() const {
#ifdef LABELED
		return action::getUnlabeledAction(m_nLastAction);
#else
		return m_nLastAction;
#endif
	}

	// useful functions for dependencies.
#ifdef DEPENDENCIES
	inline int head(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lHeads[index];
	}
	inline int leftdep(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepsL[index];
	}
	inline int rightdep(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepsR[index];
	}
	inline int sibling(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lSibling[index];
	}

#ifdef LABELED
	inline int label( const int &index ) const {assert(index<=m_nNextWord); return m_lLabels[index];}
#endif

#endif

	inline int leftarity(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepNumL[index];
	}
	inline int rightarity(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepNumR[index];
	}

	inline const CSetOfTags<CDependencyLabel> &lefttagset(
			const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepTagL[index];
	}
	inline const CSetOfTags<CDependencyLabel> &righttagset(
			const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepTagR[index];
	}

	inline int size() const {
		return m_nNextWord;
	}

	// useful functions for links
#ifdef LINKS
	std::vector<int> head(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lHeads[index];
	}
	std::vector<int> leftdep(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepsL[index];
	}
	std::vector<int> rightdep(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lDepsR[index];
	}
	std::vector<int> sibling(const int &index) const {
		assert(index <= m_nNextWord);
		return m_lSibling[index];
	}

#ifdef LABELED
	std::vector<unsigned long> label( const int &index ) const {
		assert(index<=m_nNextWord);
		return m_lLabels[index];
	}
#endif

#endif

	void clear() {
		m_nNextWord = 0;
		m_Stack.clear();
		m_HeadStack.clear();
		score = 0;
		m_nLastAction = action::NO_ACTION;

		/**
		 * Edited by JK
		 */
		m_Children.clear();
		linkAutomata[m_nNextWord].clearState();
		//end


		ClearNext();
	}

	void operator =(const CStateItem &item) {
		m_Stack = item.m_Stack;

		/* Edited by JK */
		//TODO I should remove Stack and NextWord. At the time, I didn't know that we just needed m_Stack, and m_nNextWord.
		Stack = item.m_Stack;
		NextWord = item.m_nNextWord;
		//end

		m_HeadStack = item.m_HeadStack;
		m_nNextWord = item.m_nNextWord;

		/**
		 * Edited by JK
		 */
		m_Children = item.m_Children;
		//end

		m_nLastAction = item.m_nLastAction;
		m_lCache = item.m_lCache;
		score = item.score;
		for (int i = 0; i <= m_nNextWord; ++i) { // only copy active word (including m_nNext)

#ifdef DEPENDENCIES
			m_lHeads[i] = item.m_lHeads[i];
			m_lDepsL[i] = item.m_lDepsL[i];
			m_lDepsR[i] = item.m_lDepsR[i];

			m_lSibling[i] = item.m_lSibling[i];
#ifdef LABELED
			m_lLabels[i] = item.m_lLabels[i];
#endif
#endif

#ifdef LINKS
			// copy the entire vectors over. Representing all of the heads for each index.
			copyVector(m_lHeads[i], item.m_lHeads[i]);
			copyVector(m_lDepsL[i], item.m_lDepsL[i]);
			copyVector(m_lDepsR[i], item.m_lDepsR[i]);

			copyVector(m_lSibling[i], item.m_lSibling[i]);

#ifdef LABELED
			copyVector(m_lLabels[i], item.m_lLabels[i]);
#endif
#endif

			m_lDepNumL[i] = item.m_lDepNumL[i];
			m_lDepNumR[i] = item.m_lDepNumR[i];
			m_lDepTagL[i] = item.m_lDepTagL[i];
			m_lDepTagR[i] = item.m_lDepTagR[i];

			// Edited by J
			QueueStackReduceState[i] = item.QueueStackReduceState[i];
			linkAutomata[i] = item.linkAutomata[i];
		}
	}

//-----------------------------------------------------------------------------

public:
// the arc left action links the current stack top to the next word with popping
#ifdef LABELED

#ifdef LINKS
	void ConnectLeft(unsigned long lab) {
#else
	void ArcLeft(unsigned long lab) {
#endif

#else
#ifdef LINKS
	void ConnectLeft()
#else
	void ArcLeft() {
#endif
#endif

#ifdef PRINTACTIONS_JUNEKI
		printAction(1);
		printStacks();
#endif

		assert(m_Stack.size() > 0);

#ifdef DEPENDENCIES
		assert(m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD);
#endif
#ifdef LINKS
		assert(!m_lHeads[m_Stack.back()].empty());
#endif

		static int left;
		left = m_Stack.back();

		/**
		 * Edited by JK
		 */
		std::map<int, std::vector<int> >::iterator it = m_Children.find(
				m_nNextWord);
		if (it == m_Children.end()) {
			m_Children.insert(
					std::map<int, std::vector<int> >::value_type(m_nNextWord,
							std::vector<int>()));
		}
		m_Children[m_nNextWord].push_back(left);

		// 2 means reduced or popped
		QueueStackReduceState[left] = 2;

		//head automata operations. The stack node cannot take any more right children. The queue node takes a left child.
		setAutomataPOS(m_nNextWord);
		linkAutomata[left].halt();
		linkAutomata[left].takeParent(&linkAutomata[m_nNextWord]);
		linkAutomata[m_nNextWord].transition(&linkAutomata[left]);

		//end

		m_Stack.pop_back();
		m_HeadStack.pop_back();

		// set the head of "left" to be the next word.
#ifdef DEPENDENCIES
		m_lHeads[left] = m_nNextWord;
#endif
#ifdef LINKS
		m_lHeads[left].push_back(m_nNextWord);
#endif

		// set the label corresponding to this arc
#ifdef LABELED
#ifdef DEPENDENCIES
		m_lLabels[left] = lab;
#endif
#ifdef LINKS
		m_lLabels[left].push_back(lab);
#endif
		m_lDepTagL[m_nNextWord].add(lab);

		// JUNEKI: Just poking around dependency labels objects.
		// CSetOfTags<CDependencyLabel> depTagLabel_left = m_lDepTagL[m_nNextWord];
		// depTagLabel_left is the arc label to the next word.
		// std::cout << "LEFT: " << depTagLabel_left << std::endl << std::endl;
		//end

#endif

#ifdef DEPENDENCIES
		m_lSibling[left] = m_lDepsL[m_nNextWord];
		m_lDepsL[m_nNextWord] = left;
#endif
#ifdef LINKS
		copyVector(m_lSibling[left], m_lDepsL[m_nNextWord]);
		m_lDepsL[m_nNextWord].push_back(left);
#endif
		m_lDepNumL[m_nNextWord]++;

#ifdef LABELED

#ifdef LINKS
		m_nLastAction=action::encodeAction(action::CONNECT_LEFT,lab);
#else
		m_nLastAction=action::encodeAction(action::ARC_LEFT, lab);
#endif

#else

#ifdef LINKS
		m_nLastAction = action::encodeAction(action::CONNECT_LEFT);
#else
		m_nLastAction = action::encodeAction(action::ARC_LEFT);
#endif
#endif

#ifdef PRINTACTIONS_JUNEKI
		printStacks();
		printLinkAutomatonStates();
		printEndLine();
#endif

	}

	// the arc right action links the next word to the current stack top with pushing
#ifdef LABELED

#ifdef LINKS
	void ConnectRight(unsigned long lab) {
#else
		void ArcRight(unsigned long lab) {
#endif

#else

#ifdef LINKS
	void ConnectRight() {
#else
	void ArcRight() {
#endif
#endif

#ifdef PRINTACTIONS_JUNEKI
		printAction(2);
		printStacks();
#endif

		assert(m_Stack.size() > 0);
		static int left;
		left = m_Stack.back();

//#ifdef DEPENDENCIES
		//TODO
		// Separate out the implicit shift action from right arc
		m_Stack.push_back(m_nNextWord);
//#endif

#ifdef DEPENDENCIES
		m_lHeads[m_nNextWord] = left;
#endif
#ifdef LINKS
		m_lHeads[m_nNextWord].push_back(left);
#endif

		/**
		 * Edited by JK
		 */
		std::map<int, std::vector<int> >::iterator it = m_Children.find(left);
		if (it == m_Children.end()) {
			m_Children.insert(
					std::map<int, std::vector<int> >::value_type(left,
							std::vector<int>()));
		}
		m_Children[left].push_back(m_nNextWord);

		// 1 means the word is now on the stack
		QueueStackReduceState[m_nNextWord] = 1;

		//head automata operations. Arc Right. The queue node cannot take any more left children. The stack node takes a right child.
		setAutomataPOS(m_nNextWord);

		linkAutomata[left].transition(&linkAutomata[m_nNextWord]);
		linkAutomata[m_nNextWord].takeParent(&linkAutomata[left]);
//		linkAutomata[m_nNextWord].flip();
		//end

#ifdef DEPENDENCIES
#ifdef LABELED
		m_lLabels[m_nNextWord] = lab;
		m_lDepTagR[left].add(lab);
#endif
		m_lSibling[m_nNextWord] = m_lDepsR[left];
		m_lDepsR[left] = m_nNextWord;
		m_lDepNumR[left]++;

		m_nNextWord++;
		ClearNext();
#endif

#ifdef LINKS
#ifdef LABELED
		m_lLabels[m_nNextWord].push_back(lab);
		m_lDepTagR[left].add(lab);
#endif
		for(unsigned i = 0; i < m_lDepsR[left].size(); i++) {
			m_lSibling[m_nNextWord].push_back(m_lDepsR[left][i]);
		}

		m_lDepsR[left].push_back(m_nNextWord);
		m_lDepNumR[left]++;

		//TODO Want to change the action not to move the next word after an arc right
		m_nNextWord++;
		ClearNext();

#endif

#ifdef LABELED

#ifdef LINKS
		m_nLastAction=action::encodeAction(action::CONNECT_RIGHT, lab);
#else
		m_nLastAction=action::encodeAction(action::ARC_RIGHT, lab);
#endif

#else

#ifdef LINKS
		m_nLastAction = action::encodeAction(action::CONNECT_RIGHT);
#else
		m_nLastAction = action::encodeAction(action::ARC_RIGHT);
#endif
#endif

#ifdef PRINTACTIONS_JUNEKI
		printStacks();
		printLinkAutomatonStates();
		printEndLine();
#endif

	}

	// the shift action does pushing
	void Shift() {
		//Edited by J
#ifdef PRINTACTIONS_JUNEKI
		printAction(3);
		printStacks();
#endif

		// 1 means that the word is now on the stack.
		QueueStackReduceState[m_nNextWord] = 1;
		//head automata. Shift operation. The right hand side will not take any more left children
		setAutomataPOS(m_nNextWord);
		if (!m_Stack.empty()) {
			linkAutomata[m_Stack.back()].clearHistory();
		}
		linkAutomata[m_nNextWord].clearHistory();
		linkAutomata[m_nNextWord].flip();
		//end

		m_Stack.push_back(m_nNextWord);
		m_HeadStack.push_back(m_nNextWord); // look at this next
		m_nNextWord++;
		ClearNext();
		m_nLastAction = action::encodeAction(action::SHIFT);

#ifdef PRINTACTIONS_JUNEKI
		printStacks();
		printLinkAutomatonStates();
		printEndLine();
#endif

	}

	// the reduce action does popping
	void Reduce() {

#ifdef PRINTACTIONS_JUNEKI
		printAction(4);
		printStacks();
#endif

#ifdef DEPENDENCIES
		assert(m_lHeads[m_Stack.back()] != DEPENDENCY_LINK_NO_HEAD);
#endif
#ifdef LINKS
		assert(!m_lHeads[m_Stack.back()].empty());
#endif

		m_Stack.pop_back();
		m_nLastAction = action::encodeAction(action::REDUCE);

		/**
		 * Edited by JK
		 */
		// 2 means reduced or popped
		QueueStackReduceState[m_Stack.back()] = 2;
		//head automata. Reduce operation. The stack node will not take any more right children.
		if (!m_Stack.empty()) {
			linkAutomata[m_Stack.back()].clearHistory();
		}
		linkAutomata[m_nNextWord].clearHistory();

		linkAutomata[m_Stack.back()].halt();

#ifdef PRINTACTIONS_JUNEKI
		printStacks();
		printLinkAutomatonStates();
		printEndLine();
#endif
		//end
	}

	// this is used for the convenience of scoring and updating
	void PopRoot() {

#ifdef DEPENDENCIES
		assert(m_Stack.size() == 1 && m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD); // make sure only one root item in stack
#ifdef LABELED
		m_lLabels[m_Stack.back()] = CDependencyLabel::ROOT;
#endif
		m_nLastAction = action::encodeAction(action::POP_ROOT);
		m_Stack.pop_back(); // pop it
#endif

#ifdef LINKS
		assert(m_Stack.size() == 1 && m_lHeads[m_Stack.back()].empty()); // make sure only one root item in stack
#ifdef LABELED
		m_lLabels[m_Stack.back()].push_back(CDependencyLabel::ROOT);
#endif
		m_nLastAction = action::encodeAction(action::POP_ROOT);
		m_Stack.pop_back(); // pop it
#endif

	}

	// the clear next action is used to clear the next word, used with forwarding the next word index
	void ClearNext() {

#ifdef DEPENDENCIES
		m_lHeads[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
		m_lDepsL[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
		m_lDepsR[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
		m_lSibling[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
#endif

#ifdef LINKS
		m_lHeads[m_nNextWord].clear();
		m_lDepsL[m_nNextWord].clear();
		m_lDepsR[m_nNextWord].clear();
		m_lSibling[m_nNextWord].clear();
#endif

		m_lDepNumL[m_nNextWord] = 0;
		m_lDepNumR[m_nNextWord] = 0;
		m_lDepTagL[m_nNextWord].clear();
		m_lDepTagR[m_nNextWord].clear();

		//Edited by J
		linkAutomata[m_nNextWord].clearState();
		//end

#ifdef LABELED
#ifdef DEPENDENCIES
		m_lLabels[m_nNextWord] = CDependencyLabel::NONE;
#endif
#ifdef LINKS
		m_lLabels[m_nNextWord].clear();
#endif
#endif
	}

	// the move action is a simple call to do action according to the action code
	void Move(const unsigned long &ac) {
//		saveCurrentStacksToPrevious();

#ifdef LABELED
		switch (action::getUnlabeledAction(ac)) {
#else
		switch (ac) {
#endif
		case action::NO_ACTION:
			return;
		case action::SHIFT:
			Shift();
			return;
		case action::REDUCE:
			Reduce();
			return;

#ifdef LINKS
			case action::CONNECT_LEFT:
#ifdef LABELED
			ConnectLeft(action::getLabel(ac));
#else
			ConnectLeft();
#endif

#else
		case action::ARC_LEFT:
#ifdef LABELED
			ArcLeft(action::getLabel(ac));
#else
			ArcLeft();
#endif
#endif
			return;

#ifdef LINKS
			case action::CONNECT_RIGHT:
#ifdef LABELED
			ConnectRight(action::getLabel(ac));
#else
			ConnectRight();
#endif
#else
		case action::ARC_RIGHT:
#ifdef LABELED
			ArcRight(action::getLabel(ac));
#else
			ArcRight();
#endif
#endif
			return;
		case action::POP_ROOT:
			PopRoot();
			return;
		default:
			THROW("unknown action: " << ac << '.');
		}
	}

//-----------------------------------------------------------------------------

public:

	// returns true is the next word advances -- by shift or arcright.-
#ifdef DEPENDENCIES
#ifdef LABELED
	bool StandardMoveStep( const CDependencyParse &tree, const std::vector<CDependencyLabel>&m_lCacheLabel )
#else
	bool StandardMoveStep(const CDependencyParse &tree)
#endif
	{
		static int top;
		// when the next word is tree.size() it means that the sentence is done already
		if (m_nNextWord == static_cast<int>(tree.size())) {
			assert(m_Stack.size() > 0);
			if (m_Stack.size() > 1) {
				Reduce();
				return false;
			} else {
				PopRoot();
				return false;
			}
		}
		// the first case is that there is some words on the stack linking to nextword
		if (m_Stack.size() > 0) {
			top = m_Stack.back();
			while (!(m_lHeads[top] == DEPENDENCY_LINK_NO_HEAD))
			top = m_lHeads[top];
			if (tree[top].head == m_nNextWord) { // if a local head deps on nextword first
				if (top == m_Stack.back()) {
#ifdef LABELED
					assert(m_lCacheLabel[top].str() == tree[top].label);
					ArcLeft(m_lCacheLabel[top].code()); // link it to the next word
#else
					ArcLeft();       // link it to the next word
#endif
					return false;
				} else {
					Reduce();
					return false;
				}
			}
		}
		// the second case is that no words on the stack links nextword, and nextword does not link to stack word
		if (tree[m_nNextWord].head == DEPENDENCY_LINK_NO_HEAD ||// the root or
				tree[m_nNextWord].head > m_nNextWord) { // head on the right
			Shift();
			return true;
		}
		// the last case is that the next words links to stack word
		else {                               // head on the left
			assert(m_Stack.size() > 0);
			top = m_Stack.back();
			if (tree[m_nNextWord].head == top) { // the next word deps on stack top

#ifdef LABELED
				assert(m_lCacheLabel[m_nNextWord].str()==tree[m_nNextWord].label);
				ArcRight(m_lCacheLabel[m_nNextWord].code());
#else            
				ArcRight();
#endif            
				return true;
			} else {           // must depend on non-immediate h
				Reduce();
				return false;
			}
		}
	}
#endif

	// returns true is the next word advances -- by shift or arcright.-
#ifdef LINKS
#ifdef LABELED
	bool StandardMoveStep( const CDependencyLinkParse &tree, const std::vector<CDependencyLabel>&m_lCacheLabel )
#else
	bool StandardMoveStep(const CDependencyLinkParse &tree)
#endif
	{
		static int top;
		// when the next word is tree.size() it means that the sentence is done already
		if (m_nNextWord == static_cast<int>(tree.size())) {
			assert(m_Stack.size() > 0);
			if (m_Stack.size() > 1) {
				Reduce();
				return false;
			} else {
				PopRoot();
				return false;
			}
		}
		// the first case is that there is some words on the stack linking to nextword
		if (m_Stack.size() > 0) {
			top = m_Stack.back();
			std::list<int> topList(m_lHeads[top].begin(), m_lHeads[top].end());

			// Breadth-First search the top list. If the head of a top is the next word, then connect left.
			while (!(topList.empty()))
			{
				top = topList.front();
				topList.pop_front();

				std::vector<int> tempTops = m_lHeads[top];
				for(unsigned i = 0; i < tempTops.size(); i++){
					int candidateTop = tempTops[i];

					if(candidateTop == m_nNextWord) { // if a local head deps on nextword first
						if(top == m_Stack.back()){
#ifdef LABELED
							assert(m_lCacheLabel[top].str() == tree[top].labels[i]);
							ConnectLeft(m_lCacheLabel[top].code());
#else
							ConnectLeft();
#endif
							return false;
						} else{
							Reduce();
							return false;
						}
					}
					topList.push_back(tempTops[i]);
				}
			}
		}

		// the second case is that no words on the stack links nextword, and nextword does not link to stack word
		bool shouldShift = false;
		if(tree[m_nNextWord].heads.empty()){// the root or
			shouldShift = true;
		}
		else{
			for(unsigned i = 0; i < tree[m_nNextWord].heads.size(); i++){
				if(tree[m_nNextWord].heads[i] > m_nNextWord){// head on the right
					shouldShift = true;
				}
			}
		}

		if(shouldShift){
			Shift();
			return true;
		}
		// the last case is that the next words links to stack word
		else {                               // head on the left
			assert(m_Stack.size() > 0);
			top = m_Stack.back();

			for(unsigned i = 0; i < tree[m_nNextWord].heads.size(); i++){
				if(tree[m_nNextWord].heads[i] == top){
#ifdef LABELED
					assert(m_lCacheLabel0m_nNextWord].str()==tree[m_nNextWord].labels[i]);
					ConnectRight(m_lCacheLabel[m_nNextWord].code());
#else
					ConnectRight();
#endif
					return true;
				} else {         // must depend on non-immediate h
					Reduce();
					return false;
				}
			}
		}
	}
#endif


	// we want to pop the root item after the whole tree done
	// on the one hand this seems more natural
	// on the other it is easier to score
	void StandardFinish() {
		assert(m_Stack.size() == 0);
	}

#ifdef DEPENDENCIES
	unsigned FollowMove(const CStateItem *item) {
		static int top;
		// if the next words are same then don't check head because it might be a finished sentence (m_nNextWord==sentence.sz)
		if (m_nNextWord == item->m_nNextWord) {
			assert(m_Stack.size() > item->m_Stack.size());
			top = m_Stack.back();

			if (item->m_lHeads[top] == m_nNextWord) {
#ifdef LABELED
				return action::encodeAction(action::ARC_LEFT, item->m_lLabels[top]);
#else
				return action::ARC_LEFT;
#endif
			} else if (item->m_lHeads[top] != DEPENDENCY_LINK_NO_HEAD) {
				return action::encodeAction(action::REDUCE);
			} else {
				return action::encodeAction(action::POP_ROOT);
			}
		}
		// the first case is that there is some words on the stack linking to nextword
		if (m_Stack.size() > 0) {
			top = m_Stack.back();
			while (!(m_lHeads[top] == DEPENDENCY_LINK_NO_HEAD))
			top = m_lHeads[top];
			if (item->head(top) == m_nNextWord) { // if a local head deps on nextword first
				if (top == m_Stack.back()) {
#ifdef LABELED
					return action::encodeAction(action::ARC_LEFT, item->m_lLabels[top]);
#else
					return action::ARC_LEFT;
#endif
				} else {
					return action::encodeAction(action::REDUCE);
				}
			}
		}
		// the second case is that no words on the stack links nextword, and nextword does not link to stack word
		if (item->head(m_nNextWord) == DEPENDENCY_LINK_NO_HEAD ||// the root or
				item->head(m_nNextWord) > m_nNextWord) { // head on the right
			return action::encodeAction(action::SHIFT);
		}
		// the last case is that the next words links to stack word
		else {                               // head on the left
			assert(m_Stack.size() > 0);
			top = m_Stack.back();
			if (item->head(m_nNextWord) == top) { // the next word deps on stack top
#ifdef LABELED
				return action::encodeAction(action::ARC_RIGHT, item->m_lLabels[m_nNextWord]);
#else
				return action::ARC_RIGHT;
#endif
			} else {           // must depend on non-immediate h
				return action::encodeAction(action::REDUCE);
			}
		}
	}
#endif




#ifdef LINKS
	unsigned FollowMove(const CStateItem *item) {
		static int top;
		// if the next words are same then don't check head because it might be a finished sentence (m_nNextWord==sentence.sz)
		if (m_nNextWord == item->m_nNextWord) {
			assert(m_Stack.size() > item->m_Stack.size());
			top = m_Stack.back();

			bool contains = false;
			int containsIndex = -1;
			for(unsigned i = 0; i < item->m_lHeads[top].size(); i++) {
				if (item->m_lHeads[top][i] == m_nNextWord) {
					contains = true;
					containsIndex = i;
					break;
				}
			}

			if(contains) {
#ifdef LABELED
				return action::encodeAction(action::CONNECT_LEFT, item->m_lLabels[top][containsIndex]);
#else
				return action::CONNECT_LEFT;
#endif
			} else if (!item->m_lHeads[top].empty()) {
				return action::encodeAction(action::REDUCE);
			} else {
				return action::encodeAction(action::POP_ROOT);
			}
		}



		// the first case is that there is some words on the stack linking to nextword
		if (m_Stack.size() > 0) {
			top = m_Stack.back();

			std::list<int> topList(m_lHeads[top].begin(), m_lHeads[top].end());

			// Because we have multiheadedness, we have to do a more exhaustive search for m_nNextWord
			// Breadth-First Search of the tops.
			while(!topList.empty()){
				top = topList.front();
				topList.pop_front();

				std::vector<int> tempTops = item->m_lHeads[top];
				for(unsigned i = 0; i < tempTops.size(); i++){
					int candidateTop = tempTops[i];

					if(candidateTop == m_nNextWord){
						if(top == m_Stack.back()){
#ifdef LABELED
							return action::encodeAction(action::CONNECT_LEFT, item->m_lLabels[top][i]);
#else
							return action::CONNECT_LEFT
#endif
						} else{
							return action::encodeAction(action::REDUCE);
						}
					}
					topList.push_back(tempTops[i]);
				}
			}
		}


		// the second case is that no words on the stack links nextword, and nextword does not link to stack word
		const std::vector<int> heads_nextWord = item->head(m_nNextWord);
		bool shouldShift = false;
		if(heads_nextWord.empty()) { // the root or
			shouldShift = true;
		}
		if(!shouldShift){
			for(unsigned i = 0; i < heads_nextWord.size(); i++) {
				if(heads_nextWord[i] > m_nNextWord) {  // head on the right
					shouldShift = true;
				}
			}
		}
		if (shouldShift) {
			return action::encodeAction(action::SHIFT);
		}
		// the last case is that the next words links to stack word
		else {                               // head on the left
			assert(m_Stack.size() > 0);
			top = m_Stack.back();
			bool shouldRightArc = false;
			int rightArcIndex = -1;

			for(unsigned i = 0; i < heads_nextWord.size(); i++){
				if(heads_nextWord[i] == top){
					shouldRightArc = true;
					rightArcIndex = i;
					break;
				}
			}

			if (shouldRightArc) { // the next word deps on stack top
#ifdef LABELED
				return action::encodeAction(action::CONNECT_RIGHT, item->m_lLabels[m_nNextWord][rightArcIndex]);
#else
				return action::CONNECT_RIGHT;
#endif
			} else {           // must depend on non-immediate h
				return action::encodeAction(action::REDUCE);
			}
		}
	}
#endif


#ifdef DEPENDENCIES
	void GenerateTree(const CTwoStringVector &input,
			CDependencyParse &output) const {
		output.clear();

		for (int i = 0; i < size(); ++i) {
#ifdef LABELED
			output.push_back( CLabeledDependencyTreeNode( input.at(i).first , input.at(i).second , m_lHeads[i] , CDependencyLabel(m_lLabels[i]).str() ) );
#else
			output.push_back(
					CDependencyTreeNode(input.at(i).first, input.at(i).second,
							m_lHeads[i]));
#endif
		}
	}
#endif

#ifdef LINKS
	void GenerateLinkTree(const CTwoStringVector &input, CDependencyLinkParse &output) const {
		output.clear();

		for(int i = 0; i < size(); ++i) {

//			std::vector<int> heads = m_lHeads[i];
//			heads.push_back(m_lHeads);

#ifdef LABELED
//			output.push_back( CLabeledDepenencyLinkTreeNode( input.at(i).first , input.at(i).second , m_lHeads[i], CDependencyLabel(m_lLabels[i]).str()));

			std::vector<std::string> labels;
			for(unsigned j = 0; j < m_lLabels[i].size(); j++) {
				labels.push_back(CDependencyLabel(m_lLabels[i][j]).str());
			}

			output.push_back( CLabeledDependencyLinkTreeNode( input.at(i).first , input.at(i).second , m_lHeads[i], labels));

#else
//TODO
//			output.push_back( CDependencyLinkTreeNode(input.at(i).first, input.at(i).second, m_lHeads[i]));
			output.push_back( CDependencyLinkTreeNode(input.at(i).first, input.at(i).second, m_lHeads[i]));
#endif
		}
	}
#endif

#ifdef DEPENDENCIES
	/** Edited by J
	 *
	 */
	int m_lHeads_lookup(int index) const {
		return m_lHeads[index];
	}

	bool hasParent(int index) const {
		return m_lHeads_lookup(index) != -1 && m_lHeads_lookup(index) != 0;
	}
#endif

//locationCode: 0 = buffer. 1 = stack. 2 = reduced
	bool childCheck(int index, int locationCode) const {
		std::map<int, std::vector<int> >::const_iterator bufferWordLookup =
				m_Children.find(index);

		if (bufferWordLookup == m_Children.end()) {
			return false;
		}

		std::pair<int, std::vector<int> > topBufferChildren =
				*(bufferWordLookup);
		for (std::vector<int>::const_iterator topBufferChildrenIter =
				topBufferChildren.second.begin();
				topBufferChildrenIter != topBufferChildren.second.end();
				topBufferChildrenIter++) {

			int child = *topBufferChildrenIter;
			if (QueueStackReduceState[child] == locationCode) {
				return true;
			}
		}
		return false;
	}

	bool childOnBufferCheck(int index) const {
		return childCheck(index, 0);
	}

	bool childOnStackCheck(int index) const {
		return childCheck(index, 1);
	}

	bool childReducedCheck(int index) const {
		return childCheck(index, 2);
	}

	bool isParent(int parentIndex, int childIndex) const {

		std::map<int, std::vector<int> >::const_iterator stackWordLookup =
				m_Children.find(parentIndex);
		if (parentIndex != -1 && stackWordLookup != m_Children.end()) {
			std::pair<int, std::vector<int> > ChildrenList = *(stackWordLookup);

			for (std::vector<int>::const_iterator ChildrenListIter =
					ChildrenList.second.begin();
					ChildrenListIter != ChildrenList.second.end();
					ChildrenListIter++) {

				if (*ChildrenListIter == childIndex) {
					return true;
				}
			}
		}
		return false;
	}

	void mChildrenInsert(int parent, int child) {
		std::map<int, std::vector<int> >::const_iterator childLookup =
				m_Children.find(parent);

		// If we haven't found the parent in our map.
		if (childLookup == m_Children.end()) {
			std::vector<int> newChildList;
			newChildList.push_back(child);
			m_Children.insert(
					std::pair<int, std::vector<int> >(parent, newChildList));
			// m_Children.insert(std::map<int, std::vector<int> >::value_type(parent,std::vector<int>()));m_Children[parent].push_back(child);

		}
		// else, if we have found the parent in our map
		else {

			std::vector<int> childrenList = childLookup->second;
			// iterate over the children, checking if we already have the arc.
			for (std::vector<int>::const_iterator childIter =
					childrenList.begin(); childIter != childrenList.end();
					childIter++) {

				// arc already exists
				if (child == *(childIter)) {
					return;
				}
			}
//			childrenList.second.push_back(child);
			m_Children[parent].push_back(child);
		}
	}

	void printVector(std::vector<int> v) {
		for (std::vector<int>::const_iterator iter = v.begin(); iter != v.end();
				iter++) {
			std::cout << *(iter) << " ";
		}
		std::cout << "\n";
	}

	void removeArc(int parent, int child) {
		std::map<int, std::vector<int> >::const_iterator childLookup =
				m_Children.find(parent);

		// if the parent exists in our child map
		if (childLookup != m_Children.end()) {
			std::pair<int, std::vector<int> > childrenList = *(childLookup);

			// we look through the children and look to see if it matches the child.
			for (std::vector<int>::iterator childrenListLookup =
					childrenList.second.begin();
					childrenListLookup != childrenList.second.end();
					childrenListLookup++) {
				if (child == *(childrenListLookup)) {

//					std::cout << "TEST\n";
//					std::cout << childrenList.first << "!!!!\n";
//					printVector(childrenList.second);
//					std::cout << "---\n";

					childrenList.second.erase(childrenListLookup);

//					printVector(childrenList.second);
//					std::cout << "---\n";

					return;
				}
			}

		}
	}

// will make an arc between parent and child.
	void makeArc(int parent, int child) {

		//TODO delete all of the crossing arcs to maintain projectivity
		// iterate over each parent-child mapping
		std::map<int, std::vector<int> >::iterator childMapIter;
		for (childMapIter = m_Children.begin();
				childMapIter != m_Children.end(); childMapIter++) {

			// take the parent first.
			int tempParent = childMapIter->first;
			std::vector<int> childVector = childMapIter->second;

			bool parentBefore = tempParent < parent && tempParent < child;
			bool parentAfter = tempParent > parent && tempParent > child;
			bool parentMiddle = !(parentBefore || parentAfter
					|| tempParent == parent || tempParent == child);

			std::vector<int>::iterator childVectorIter;
			// iterate over all the children of the parent.
			for (childVectorIter = (*childMapIter).second.begin();
					childVectorIter != (*childMapIter).second.end();) {
				int tempChild = *childVectorIter;
				bool childBefore = tempChild < parent && tempChild < child;
				bool childAfter = tempChild > parent && tempChild > child;
				bool childMiddle = !(childBefore || childAfter
						|| tempChild == parent || tempChild == child);

				// make the check to see if the arc needs to be deleted.
				bool deleteCondition = (tempChild == child);
				bool deleteCondition_crossing1 = ((parentBefore && childMiddle)
						|| (parentAfter && childMiddle));
				bool deleteCondition_crossing2 = (parentMiddle && childBefore)
						|| (parentMiddle && childAfter);

				//TODO handle how not to add the 33 -> 34 edge

//				bool deleteCondition3 = tempChild == child; // we cannot have more than 1 incoming arrow

				//delete the arc
				if (deleteCondition || deleteCondition_crossing1
						|| deleteCondition_crossing2) {
//					printVector(childVector);

//					cout << "we want to delete this: " << tempParent << " -> "
//							<< *(childVectorIter) << "\n";

					childVectorIter = childMapIter->second.erase(
							childVectorIter);

//					printVector(childVector);

				} else {
					++childVectorIter;
				}
			}
		}
		mChildrenInsert(parent, child);
	}

	/** Prints out the action that the parser is taking. Helps to see what is going on.
	 *
	 */
	void printAction(int code, bool flag = true) {

		if (!flag) {
			return;
		}

		switch (code) {
		case 1:

#ifdef LINKS
			cerr << "CONNECT LEFT\n";
#else
			cerr << "ARC LEFT" << "\n";
#endif
			break;
		case 2:
#ifdef LINKS
			cerr << "CONNECT RIGHT\n";
#else
			cerr << "ARC RIGHT" << "\n";
#endif
			break;
		case 3:
			cerr << "SHIFT" << "\n";
			break;
		case 4:
			cerr << "REDUCE" << "\n";
			break;
		}
	}

	/** Prints out the state of some of the stacks of this parser. Just to help see whats going on.
	 *
	 */
	void printStacks(bool flag = true) {
		if (!flag) {
			return;
		}

		//MAX_SENTENCE_SIZE
		cerr << "[ ";
		for (std::vector<int>::const_iterator i = m_Stack.begin();
				i != m_Stack.end(); ++i)
			cerr << *i << ' ';
		cerr << "| ";
		cerr << m_nNextWord << " ...  ] \n";

//		cerr << "m_HeadStack: ";
//		for (std::vector<int>::const_iterator i = m_HeadStack.begin();
//				i != m_HeadStack.end(); ++i)
//			cerr << *i << ' ';
//		cerr << std::endl;
//
//		cerr << "m_lHeads: ";
//		for (int i = 0; i < 10; i++) {
//			cerr << m_lHeads[i] << " ";
//		}
//		cerr << std::endl;

		cerr << std::endl;
	}

	void printLinkAutomatonStates(bool flag = true) {
		if (!flag) {
			return;
		}

//		for (int i = 0; i < MAX_SENTENCE_SIZE; i++) {
//			linkAutomata[i].uniqueID = i;
//		}

		for (unsigned int i = 0; i < 10 && i < m_lCache->size(); i++) {
			State s = linkAutomata[i];
			s.print();
			cerr << "<" << (m_lCache->at(i)).word.str() << "|"
					<< (m_lCache->at(i)).tag.str() << ">";
			cerr << ",";

			cerr << "\n";
		}
		cerr << "\n";
	}

	void printEndLine(bool flag = true) {
		if (!flag) {
			return;
		}
		cerr << "---\n\n";
	}

// Link Automata inline access operations.
	inline const State* automataLookup(int const index) const {
		return &linkAutomata[index];
	}

// Takes a POS tag at a given index and converts it into an int. We then give it to the the link automata.
	void setAutomataPOS(int index) {
		static map<string, int> POSmap;
		static int POSmapIndexValue = 1;

		CTaggedWord<CTag, TAG_SEPARATOR> taggedWord = m_lCache->at(index);
		const CTag &tag = taggedWord.tag;
		std::string tagString = tag.str();

		map<string, int>::iterator it = POSmap.find(tagString);
		if (it == POSmap.end()) {
			POSmap[tagString] = POSmapIndexValue++;
		}
		int value = POSmap[tagString];

//		int sum = 0;
//		int offset = 1;
//		for (int j = 0; j < tagString.size() && j < 3; j++) {
//			int temp = tagString[j]-65;
//			sum += temp * offset;
//			offset *= 100;
//		}
		linkAutomata[index].POStag = value;
	}

	void copyVector(std::vector<long unsigned int> v,
			const std::vector<long unsigned int> item) {
		v.clear();
		for (unsigned i = 0; i < item.size(); i++) {
			v.push_back(item[i]);
		}
	}

	void copyVector(std::vector<int> v, const std::vector<int> item) {
		v.clear();
		for (unsigned i = 0; i < item.size(); i++) {
			v.push_back(item[i]);
		}
	}

//end

}
;

}
// namespace depparser
}
// namespace target language

#endif
