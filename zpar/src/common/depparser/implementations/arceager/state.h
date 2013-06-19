// Copyright (C) University of Oxford 2010
#ifndef _ENGLISH_DEPENDENCY_PARSER_STATEITEM
#define _ENGLISH_DEPENDENCY_PARSER_STATEITEM

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

	bool PRINTACTION = false;

	//CStateItem* previousCStateItem;

protected:
	std::vector<int> m_Stack;     // stack of words that are currently processed
	std::vector<int> m_HeadStack;
	int m_nNextWord;                         // index for the next word

	int m_lHeads[MAX_SENTENCE_SIZE];         // the lexical head for each word
	int m_lDepsL[MAX_SENTENCE_SIZE]; // the leftmost dependency for each word (just for cache, temporary info)
	int m_lDepsR[MAX_SENTENCE_SIZE]; // the rightmost dependency for each word (just for cache, temporary info)
	int m_lDepNumL[MAX_SENTENCE_SIZE];       // the number of left dependencies
	int m_lDepNumR[MAX_SENTENCE_SIZE];       // the number of right dependencies
	CSetOfTags<CDependencyLabel> m_lDepTagL[MAX_SENTENCE_SIZE]; // the set of left tags
	CSetOfTags<CDependencyLabel> m_lDepTagR[MAX_SENTENCE_SIZE]; // the set of right tags
	int m_lSibling[MAX_SENTENCE_SIZE];       // the sibling towards head
#ifdef LABELED
	unsigned long m_lLabels[MAX_SENTENCE_SIZE]; // the label of each dependency link
#endif
	unsigned long m_nLastAction;                  // the last stack action
	const std::vector<CTaggedWord<CTag, TAG_SEPARATOR> >* m_lCache;

public:
	SCORE_TYPE score; // score of stack - predicting how potentially this is the correct one

	std::vector<int> previous_m_Stack;
	std::vector<int> previous_m_HeadStack;
	int previous_m_nNextWord;
	std::map<int, std::vector<int> > m_Children;

	std::vector<int> Stack;
	int NextWord;

	//Edited by J
	int Heads[MAX_SENTENCE_SIZE];
	int QueueStackReduceState[MAX_SENTENCE_SIZE];

	void saveCurrentStacksToPrevious() {
		previous_m_Stack = std::vector<int>(m_Stack);
		previous_m_HeadStack = std::vector<int>(m_HeadStack);
		previous_m_nNextWord = m_nNextWord;
	}

public:
	// constructors and destructor
	CStateItem(const std::vector<CTaggedWord<CTag, TAG_SEPARATOR> >*cache = 0) :
			m_lCache(cache) {
		clear();

		// Edited by J
		// Just a test to see if I can mess with m_Children before parsing time
//			m_Children[2].push_back(1);

		//end

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
		if (m_nNextWord != item.m_nNextWord)
			return false;
		for (i = 0; i < m_nNextWord; ++i) {
			if (m_lHeads[i] != item.m_lHeads[i])
				return false;
		}
#ifdef LABELED
		for ( i=0; i<m_nNextWord; ++i )
		if ( m_lLabels[i] != item.m_lLabels[i] )
		return false;
#endif
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
	inline int size() const {
		return m_nNextWord;
	}
#ifdef LABELED
	inline int label( const int &index ) const {assert(index<=m_nNextWord); return m_lLabels[index];}
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

//		int tempParent = 1;
//		int tempChild = 2;
//
//		std::map<int, std::vector<int> >::iterator it = m_Children.find(
//				tempParent);
//		if (it == m_Children.end()) {
//			m_Children.insert(
//					std::map<int, std::vector<int> >::value_type(tempParent,
//							std::vector<int>()));
//		}
//		m_Children[tempParent].push_back(tempChild);

		ClearNext();
	}

	void operator =(const CStateItem &item) {
		m_Stack = item.m_Stack;

		/* Edited by JK */
		Stack = item.m_Stack;
		NextWord = item.m_nNextWord;
		//end

		m_HeadStack = item.m_HeadStack;
		m_nNextWord = item.m_nNextWord;
		/**
		 * Edited by JK
		 */
		m_Children = item.m_Children;
		m_nLastAction = item.m_nLastAction;
		m_lCache = item.m_lCache;
		score = item.score;
		for (int i = 0; i <= m_nNextWord; ++i) { // only copy active word (including m_nNext)
			m_lHeads[i] = item.m_lHeads[i];
			m_lDepsL[i] = item.m_lDepsL[i];
			m_lDepsR[i] = item.m_lDepsR[i];
			m_lDepNumL[i] = item.m_lDepNumL[i];
			m_lDepNumR[i] = item.m_lDepNumR[i];
			m_lDepTagL[i] = item.m_lDepTagL[i];
			m_lDepTagR[i] = item.m_lDepTagR[i];
			m_lSibling[i] = item.m_lSibling[i];

			// Edited by J
			QueueStackReduceState[i] = item.QueueStackReduceState[i];

#ifdef LABELED
			m_lLabels[i] = item.m_lLabels[i];
#endif
		}
	}

//-----------------------------------------------------------------------------

public:
	// the arc left action links the current stack top to the next word with popping
#ifdef LABELED
	void ArcLeft(unsigned long lab) {
#else
	void ArcLeft() {
#endif

		/**
		 * Edited by JK
		 */
		if (PRINTACTION) {
			std::cout << "ACTION: ARCLEFT\n";
//			std::cout << "m_nNextWord: " << m_nNextWord << std::endl << std::endl;
			cout << "\n";
		}

		//end
		assert(m_Stack.size() > 0);
		assert(m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD);
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
		//end

		m_Stack.pop_back();
		m_HeadStack.pop_back();
		m_lHeads[left] = m_nNextWord;

#ifdef LABELED
		m_lLabels[left] = lab;
		m_lDepTagL[m_nNextWord].add(lab);

		// JUNEKI: Just poking around dependency labels objects.
		// CSetOfTags<CDependencyLabel> depTagLabel_left = m_lDepTagL[m_nNextWord];
		// depTagLabel_left is the arc label to the next word.
		// std::cout << "LEFT: " << depTagLabel_left << std::endl << std::endl;
		//end

#endif
		m_lSibling[left] = m_lDepsL[m_nNextWord];
		m_lDepsL[m_nNextWord] = left;
		m_lDepNumL[m_nNextWord]++;
#ifdef LABELED
		m_nLastAction=action::encodeAction(action::ARC_LEFT, lab);
#else
		m_nLastAction = action::encodeAction(action::ARC_LEFT);
#endif

		/**
		 * Edited by JK
		 */
//		std::cout << "m_Stack: ";
//		for (std::vector<int>::const_iterator i = m_Stack.begin();
//				i != m_Stack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl;
//
//		std::cout << "m_HeadStack: ";
//		for (std::vector<int>::const_iterator i = m_HeadStack.begin();
//				i != m_HeadStack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl;
//
//		std::cout << "m_lHeads: ";
//		for (int i = 0; i < MAX_SENTENCE_SIZE/2; i++) {
//			std::cout << m_lHeads[i] << " ";
//		}
//		std::cout << std::endl;
//
//		int nextWordChild = -1;
//		for(int i = 0; i<MAX_SENTENCE_SIZE; i++){
//			if (m_lHeads[i] == m_nNextWord){
//				nextWordChild = i;
//			}
//		}
//#ifdef LABELED
//		std::cout << std::endl;
//		std::cout << "m_lLabels: ";
//		for(int i = 0; i<MAX_SENTENCE_SIZE/2; i++){
//			std::cout << m_lLabels[i] << m_lDepTagL[i] << " ";
//		}
//		std::cout << std::endl;
//#endif
//		std::cout << "(" <<"child: " << nextWordChild << "," << "parent: " <<m_lHeads[m_nNextWord]<< ")";
//		std::cout << std::endl;
//		std::cout << std::endl;
		//end
	}

	// the arc right action links the next word to the current stack top with pushing
#ifdef LABELED
	void ArcRight(unsigned long lab) {
#else
	void ArcRight() {
#endif

		/**
		 * Edited by JK
		 */
		if (PRINTACTION) {
			std::cout << "ACTION: ARCRIGHT\n";
//			std::cout << "m_nNextWord: " << m_nNextWord << std::endl << std::endl;
			cout << "\n";
		}
		//end
		assert(m_Stack.size() > 0);
		static int left;
		left = m_Stack.back();
		m_Stack.push_back(m_nNextWord);
		m_lHeads[m_nNextWord] = left;

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
		//end

#ifdef LABELED
		m_lLabels[m_nNextWord] = lab;
		m_lDepTagR[left].add(lab);

		// JUNEKI: Just poking around dependency labels objects.
//		CSetOfTags<CDependencyLabel> depTagLabel_right = m_lDepTagR[m_nNextWord];
//		std::cout << "RIGHT: " << depTagLabel_right << std::endl;
//		std::cout << std::endl;

#endif
		m_lSibling[m_nNextWord] = m_lDepsR[left];
		m_lDepsR[left] = m_nNextWord;
		m_lDepNumR[left]++;
		m_nNextWord++;
		ClearNext();
#ifdef LABELED
		m_nLastAction=action::encodeAction(action::ARC_RIGHT, lab);
#else
		m_nLastAction = action::encodeAction(action::ARC_RIGHT);
#endif

		/**
		 * Edited by JK
		 */
//		std::cout << "m_Stack: ";
//		for (std::vector<int>::const_iterator i = m_Stack.begin();
//				i != m_Stack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl;
//
//		std::cout << "m_HeadStack: ";
//		for (std::vector<int>::const_iterator i = m_HeadStack.begin();
//				i != m_HeadStack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl;
//		std::cout << "m_lHeads: ";
//		for (int i = 0; i < MAX_SENTENCE_SIZE/2; i++) {
//			std::cout << m_lHeads[i] << " ";
//		}
//		std::cout << std::endl;
//
//		int nextWordChild = -1;
//		for(int i = 0; i<MAX_SENTENCE_SIZE; i++){
//			if (m_lHeads[i] == m_nNextWord){
//				nextWordChild = i;
//			}
//		}
//#ifdef LABELED
//		std::cout << std::endl;
//		std::cout << "m_lLabels: ";
//		for(int i = 0; i<MAX_SENTENCE_SIZE/2; i++){
//			std::cout << m_lLabels[i] << m_lDepTagR[i] << " ";
//		}
//		std::cout << std::endl;
//#endif
//		std::cout << "(" <<"child: " << nextWordChild << "," << "parent: " <<m_lHeads[m_nNextWord]<< ")";
//
//		std::cout << std::endl;
//		std::cout << std::endl;
		//end
	}

	// the shift action does pushing
	void Shift() {

		/**
		 * Edited by JK
		 */
		if (PRINTACTION) {
			std::cout << "ACTION: SHIFT\n";
//			std::cout << "m_nNextWord: " << m_nNextWord << std::endl;
			cout << "\n";
		}
		//end
		m_Stack.push_back(m_nNextWord);
		m_HeadStack.push_back(m_nNextWord); // look at this next
		m_nNextWord++;
		ClearNext();
		m_nLastAction = action::encodeAction(action::SHIFT);

		/**
		 * Edited by JK
		 */
		// 1 means that the word is now on the stack. I take off -1 because m_nNextWord was previously incremented ++.
		QueueStackReduceState[m_nNextWord - 1] = 1;

//		std::cout << "m_Stack: ";
//		for (std::vector<int>::const_iterator i = m_Stack.begin();
//				i != m_Stack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl;
//
//		std::cout << "m_HeadStack: ";
//		for (std::vector<int>::const_iterator i = m_HeadStack.begin();
//				i != m_HeadStack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl << std::endl;
		//end
	}

	// the reduce action does popping
	void Reduce() {

		/**
		 * Edited by JK
		 */

		if (PRINTACTION) {
			std::cout << "ACTION: REDUCE\n\n";
		}
		// 2 means reduced or popped
		QueueStackReduceState[m_Stack.back()] = 2;

		// end

		assert(m_lHeads[m_Stack.back()] != DEPENDENCY_LINK_NO_HEAD);
		m_Stack.pop_back();
		m_nLastAction = action::encodeAction(action::REDUCE);

		/**
		 * Edited by JK
		 */
//		std::cout << "m_Stack: ";
//		for (std::vector<int>::const_iterator i = m_Stack.begin();
//				i != m_Stack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl;
//
//		std::cout << "m_HeadStack: ";
//		for (std::vector<int>::const_iterator i = m_HeadStack.begin();
//				i != m_HeadStack.end(); ++i)
//			std::cout << *i << ' ';
//		std::cout << std::endl << std::endl;
		//end
	}

	// this is used for the convenience of scoring and updating
	void PopRoot() {
		assert(
				m_Stack.size() == 1
						&& m_lHeads[m_Stack.back()] == DEPENDENCY_LINK_NO_HEAD); // make sure only one root item in stack
#ifdef LABELED
								m_lLabels[m_Stack.back()] = CDependencyLabel::ROOT;
#endif
		m_nLastAction = action::encodeAction(action::POP_ROOT);
		m_Stack.pop_back(); // pop it
	}

	// the clear next action is used to clear the next word, used with forwarding the next word index
	void ClearNext() {
		m_lHeads[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
		m_lDepsL[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
		m_lDepsR[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
		m_lDepNumL[m_nNextWord] = 0;
		m_lDepTagL[m_nNextWord].clear();
		m_lDepNumR[m_nNextWord] = 0;
		m_lDepTagR[m_nNextWord].clear();
		m_lSibling[m_nNextWord] = DEPENDENCY_LINK_NO_HEAD;
#ifdef LABELED
		m_lLabels[m_nNextWord] = CDependencyLabel::NONE;
#endif
	}

	// the move action is a simple call to do action according to the action code
	void Move(const unsigned long &ac) {
		saveCurrentStacksToPrevious();
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
		case action::ARC_LEFT:
#ifdef LABELED
			ArcLeft(action::getLabel(ac));
#else
			ArcLeft();
#endif
			return;
		case action::ARC_RIGHT:
#ifdef LABELED
			ArcRight(action::getLabel(ac));
#else
			ArcRight();
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

	// returns true is the next word advances -- by shift or arcright.
#ifdef LABELED
	bool StandardMoveStep( const CDependencyParse &tree, const std::vector<CDependencyLabel>&m_lCacheLabel ) {
#else
	bool StandardMoveStep(const CDependencyParse &tree) {
#endif
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
					ArcLeft();                       // link it to the next word
#endif
					return false;
				} else {
					Reduce();
					return false;
				}
			}
		}
		// the second case is that no words on the stack links nextword, and nextword does not link to stack word
		if (tree[m_nNextWord].head == DEPENDENCY_LINK_NO_HEAD || // the root or
				tree[m_nNextWord].head > m_nNextWord) { // head on the right
			Shift();
			return true;
		}
		// the last case is that the next words links to stack word
		else {                                        // head on the left
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
			} else {                           // must depend on non-immediate h
				Reduce();
				return false;
			}
		}
	}

	// we want to pop the root item after the whole tree done
	// on the one hand this seems more natural
	// on the other it is easier to score
	void StandardFinish() {
		assert(m_Stack.size() == 0);
	}

	unsigned FollowMove(const CStateItem *item) {
		static int top;
		// if the next words are same then don't check head because it might be a finished sentence (m_nNextWord==sentence.sz)
		if (m_nNextWord == item->m_nNextWord) {
//for (int i=0; i<m_Stack.size(); ++i) std::cout << m_Stack[i] << " "; std::cout << std::endl;
//for (int i=0; i<item->m_Stack.size(); ++i) std::cout << item->m_Stack[i] << " "; std::cout << std::endl;
			assert(m_Stack.size() > item->m_Stack.size());
			top = m_Stack.back();
			if (item->m_lHeads[top] == m_nNextWord)
#ifdef LABELED
				return action::encodeAction(action::ARC_LEFT, item->m_lLabels[top]);
#else
				return action::ARC_LEFT;
#endif
			else if (item->m_lHeads[top] != DEPENDENCY_LINK_NO_HEAD)
				return action::encodeAction(action::REDUCE);
			else
				return action::encodeAction(action::POP_ROOT);
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
		if (item->head(m_nNextWord) == DEPENDENCY_LINK_NO_HEAD || // the root or
				item->head(m_nNextWord) > m_nNextWord) { // head on the right
			return action::encodeAction(action::SHIFT);
		}
		// the last case is that the next words links to stack word
		else {                                        // head on the left
			assert(m_Stack.size() > 0);
			top = m_Stack.back();
			if (item->head(m_nNextWord) == top) { // the next word deps on stack top
#ifdef LABELED
					return action::encodeAction(action::ARC_RIGHT, item->m_lLabels[m_nNextWord]);
#else
				return action::ARC_RIGHT;
#endif
			} else {                           // must depend on non-immediate h
				return action::encodeAction(action::REDUCE);
			}
		}
	}

	void GenerateTree(const CTwoStringVector &input,
			CDependencyParse &output) const {
		output.clear();
		for (int i = 0; i < size(); ++i)
#ifdef LABELED
			output.push_back( CLabeledDependencyTreeNode( input.at(i).first , input.at(i).second , m_lHeads[i] , CDependencyLabel(m_lLabels[i]).str() ) );
#else
			output.push_back(
					CDependencyTreeNode(input.at(i).first, input.at(i).second,
							m_lHeads[i]));
#endif
	}

	/** Edited by J
	 *
	 */
	int m_lHeads_lookup(int index) const {
		return m_lHeads[index];
	}

	bool hasParent(int index) const {
		return m_lHeads_lookup(index) != -1 && m_lHeads_lookup(index) != 0;
	}

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
		m_Children.insert(
				std::map<int, std::vector<int> >::value_type(parent,
						std::vector<int>()));
		m_Children[parent].push_back(child);
	}

//end

};

#endif
