// deplinkparser.cpp
// dependency link parser
// Juneki Hong 2014
// juneki@jhu.edu

#include "deplinkparser.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

const CWord g_emptyWord("");
const CTaggedWord<CTag, TAG_SEPARATOR> g_emptyTaggedWord;
const CTag g_noneTag = CTag::NONE;

#define cast_weights static_cast<CWeight*>(m_weights)
#define refer_or_allocate_tuple2(x, o1, o2) { if (amount == 0) x.refer(o1, o2); else x.allocate(o1, o2); }
#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }
#define _conll_or_empty(x) (x == "_" ? "" : x)

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepLinkParser::work(const bool bTrain, const CTwoStringVector &sentence,
		CDependencyLinkParse *retval, const CDependencyLinkParse &correct,
		int nBest, SCORE_TYPE *scores) {

#ifdef LINKS
#ifdef DEBUG
	clock_t total_start_time = clock();
#endif
	static int index;
	const int length = sentence.size();

	const CStateItem *pGenerator;
	static CStateItem pCandidate(&m_lCache);

	// used only for training
	static bool bCorrect;// used in learning for early update
	static bool bContradictsRules;
	static CStateItem correctState(&m_lCache);
	static CPackedScoreType<SCORE_TYPE, action::MAX> packed_scores;

	ASSERT((unsigned )length < MAX_SENTENCE_SIZE,
			"The size of the sentence is larger than the system configuration.");

	TRACE("Initialising the decoding process...");
	// initialise word cache
	bContradictsRules = false;
	m_lCache.clear();
	for (index = 0; index < length; ++index) {
		m_lCache.push_back(
				CTaggedWord<CTag, TAG_SEPARATOR>(sentence[index].first,
						sentence[index].second));

		// filter std::cout training examples with rules
		if (bTrain && m_weights->rules()) {

#ifdef SINGLE_ROOT
#ifdef DEPENDENCIES
			// the root
			if (correct[index].head == DEPENDENCY_LINK_NO_HEAD
					&& canBeRoot(m_lCache[index].tag.code()) == false)
#endif
#ifdef LINKS
			if (correct[index].heads.empty() && canBeRoot(m_lCache[index].tag.code()) == false)
#endif
			{
				TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " can be root.");
				bContradictsRules = true;
			}
#endif

#ifdef SINGLE_PARENT
#ifdef DEPENDENCIES
			// head left
			if (correct[index].head < index
					&& hasLeftHead(m_lCache[index].tag.code()) == false)
#endif
#ifdef LINKS
			bool headsCheck_L = false;
			bool headsCheck_R = false;
			for(unsigned i = 0; i< correct[index].heads.size(); i++){
				if(correct[index].heads[i] < index){
					headsCheck_L = true;
				}
				if(correct[index].heads[i] > index){
					headsCheck_R = true;
				}
			}
			if(headsCheck_L && hasLeftHead(m_lCache[index].tag.code()) == false)
#endif
			{
				TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has left head.");
				bContradictsRules = true;
			}

#ifdef DEPENDENCES
			// head right
			if (correct[index].head > index
					&& hasRightHead(m_lCache[index].tag.code()) == false)
#endif
#ifdef LINKS
			if(headsCheck_R && hasRightHead(m_lCache[index].tag.code()) == false)
#endif
			{
				TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has right head.");
				bContradictsRules = true;
			}
#endif

		}
	}

	// initialise agenda
	m_Agenda->clear();
	pCandidate.clear();// restore state using clean
	m_Agenda->pushCandidate(&pCandidate);// and push it back
	m_Agenda->nextRound();// as the generator item
	if (bTrain){
		correctState.clear();
	}

	// verifying supertags
	if (m_supertags) {
		ASSERT(m_supertags->getSentenceSize() == (unsigned )length,
				"Sentence size does not match supertags size");
	}

#ifdef LABELED
	unsigned long label;
	m_lCacheLabel.clear();
	if (bTrain) {
		for (index=0; index<length; ++index) {

			//TODO I need to make m_lCacheLabel take vectors of labels instead of a single label.
			if(correct[index].labels.empty()) {
				m_lCacheLabel.push_back(CDependencyLabel());
			}
			else {
				m_lCacheLabel.push_back(CDependencyLabel(correct[index].labels[0]));
			}

			//TODO
			int head = -1;
			if(!correct[index].heads.empty()) {
				head = correct[index].heads[0];
			}

			if (m_weights->rules() && !canAssignLabel(m_lCache, head, index, m_lCacheLabel[index])) {
				TRACE("Rule contradiction: " << correct[index].label << " on link head " << m_lCache[correct[index].head].tag.code() << " dep " << m_lCache[index].tag.code());
				bContradictsRules = true;
			}
		}
	}
#endif

	// skip the training example if contradicts
	if (bTrain && m_weights->rules() && bContradictsRules) {
		std::cout << "Skipping training example because it contradicts rules..."
		<< std::endl;
		return;
	}

	TRACE("Decoding started");
	// loop with the next word to process in the sentence
	for (index = 0; index < length * 2; ++index) {

		if (bTrain)
		bCorrect = false;

		pGenerator = m_Agenda->generatorStart();
		// iterate generators
		for (int j = 0; j < m_Agenda->generatorSize(); ++j) {
			int nextWord = pGenerator->NextWord;

			// for the state items that already contain all words
			m_Beam->clear();
			packed_scores.reset();

			this->CDepParser::getOrUpdateStackScore(pGenerator, packed_scores, action::NO_ACTION);

			if (pGenerator->size() == length) {
				assert(pGenerator->stacksize() != 0);
				if (pGenerator->stacksize() > 1) {
					reduce(pGenerator, packed_scores);
				} else {
					poproot(pGenerator, packed_scores);
				}
			}

			// for the state items that still need more words
			else {

#ifdef LINKS
				// if pGenerator has already arc-righted
				// then shift
//				pGenerator->lastAction();

				// In a link parser, after a connect right operation, we shift.
				if(pGenerator->lastAction() == action::CONNECT_RIGHT)
				{
					shift(pGenerator, packed_scores);
				}
				else
#endif
				if (!pGenerator->afterreduce()) { // there are many ways when there are many arcrighted items on the stack and the root need arcleft. force this.
					if ((pGenerator->size() < length - 1 || pGenerator->stackempty())
							&&// keep only one global root
							(pGenerator->stackempty() || m_supertags == 0 || m_supertags->canShift(pGenerator->size()))
							&&// supertags
							(pGenerator->stackempty() || !m_weights->rules() || canBeRoot(m_lCache[pGenerator->size()].tag.code()) || hasRightHead(
											m_lCache[pGenerator->size()].tag.code()))// rules
					) {
						shift(pGenerator, packed_scores);
					}
				}
				if (!pGenerator->stackempty()) {
					if ((pGenerator->size() < length - 1
									|| pGenerator->headstacksize() == 1) && // one root
							(m_supertags == 0
									|| m_supertags->canArcRight(
											pGenerator->stacktop(),
											pGenerator->size())) &&// supertags conform to this action
							(!m_weights->rules()
									|| hasLeftHead(
											m_lCache[pGenerator->size()].tag.code()))// rules
					) {
						connectright(pGenerator, packed_scores);
					}
				}
				if ((!m_bCoNLL && !pGenerator->stackempty())
						|| (m_bCoNLL && pGenerator->stacksize() > 1) // make sure that for conll the first item is not popped
				) {
					if ((pGenerator->head(pGenerator->stacktop()).empty())
//							&& // JUNEKI: no reduce
//							(!noReduce)
					) {
						reduce(pGenerator, packed_scores);
					}
//					else if (mustReduce) // JUNEKI: must reduce
//					{
//						reduce(pGenerator, packed_scores);
//					}

					else {
						if ((m_supertags == 0
										|| m_supertags->canArcLeft(pGenerator->size(),
												pGenerator->stacktop())) && // supertags
								(!m_weights->rules()
										|| hasRightHead(
												m_lCache[pGenerator->stacktop()].tag.code()))// rules

//								&&// JUNEKI: no arc left
//								(!noLeftArc)
						) {
							connectleft(pGenerator, packed_scores);
						}
					}
				}
			}

			// insert item
			for (int i = 0; i < m_Beam->size(); ++i) {
				pCandidate = *pGenerator;
				pCandidate.score = m_Beam->item(i)->score;
				pCandidate.Move(m_Beam->item(i)->action);
				m_Agenda->pushCandidate(&pCandidate);

			}

			if (bTrain && *pGenerator == correctState) {
				bCorrect = true;
			}
			pGenerator = m_Agenda->generatorNext();
		}
		// when we are doing training, we need to consider the standard move and update
		if (bTrain) {
#ifdef EARLY_UPDATE
			if (!bCorrect) {
				TRACE("Error at the "<<correctState.size()<<"th word; total is "<<correct.size())
				updateScoresForStates(m_Agenda->bestGenerator(), &correctState,
						1, -1);
				return;
			}
#endif

#ifdef LABELED
			correctState.StandardMoveStep(correct, m_lCacheLabel);
#else
			correctState.StandardMoveStep(correct);
#endif
		}

		m_Agenda->nextRound(); // move round
	}

	if (bTrain) {
		correctState.StandardFinish(); // pop the root that is left
		// then make sure that the correct item is stack top finally
		if (*(m_Agenda->bestGenerator()) != correctState) {
			TRACE("The best item is not the correct one")
			updateScoresForStates(m_Agenda->bestGenerator(), &correctState, 1,
					-1);
			return;
		}
	}

	TRACE("Outputing sentence");
	m_Agenda->sortGenerators();
	for (int i = 0; i < std::min(m_Agenda->generatorSize(), nBest); ++i) {
		pGenerator = m_Agenda->generator(i);
		if (pGenerator) {
			pGenerator->GenerateLinkTree(sentence, retval[i]);

			if (scores)
			scores[i] = pGenerator->score;
		}
	}TRACE("Done, the highest score is: " << m_Agenda->bestGenerator()->score );TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);

#endif
}

#ifdef LINKS
/*---------------------------------------------------------------
 *
 * connectleft
 *
 *--------------------------------------------------------------*/

void CDepLinkParser::connectleft(const CStateItem *item,
		const CPackedScoreType<SCORE_TYPE, action::MAX> &scores) {
	static action::CScoredAction scoredaction;
	static unsigned label;
#ifdef LABELED
	for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
		if ( !m_weights->rules() || canAssignLabel(m_lCache, item->size(), item->stacktop(), label) ) {
			scoredaction.action = action::encodeAction(action::CONNECT_LEFT, label);
			scoredaction.score = item->score + scores[scoredaction.action];
			//+scores[action::ARC_LEFT];
			m_Beam->insertItem(&scoredaction);

		}
	}
#else
	scoredaction.action = action::CONNECT_LEFT;
	scoredaction.score = item->score + scores[scoredaction.action];
	m_Beam->insertItem(&scoredaction);
#endif
}

/*---------------------------------------------------------------
 *
 * connectright
 *
 *--------------------------------------------------------------*/

void CDepLinkParser::connectright(const CStateItem *item,
		const CPackedScoreType<SCORE_TYPE, action::MAX> &scores) {
	static action::CScoredAction scoredaction;
	static unsigned label;
#ifdef LABELED
	for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
		if ( !m_weights->rules() || canAssignLabel(m_lCache, item->stacktop(), item->size(), label) ) {
			scoredaction.action = action::encodeAction(action::CONNECT_RIGHT, label);
			scoredaction.score = item->score + scores[scoredaction.action];
			//+scores[action::ARC_RIGHT];
			m_Beam->insertItem(&scoredaction);
		}
	}
#else
	scoredaction.action = action::CONNECT_RIGHT;
	scoredaction.score = item->score + scores[scoredaction.action];
	m_Beam->insertItem(&scoredaction);
#endif
}

/*---------------------------------------------------------------
 *
 * initCoNLLCache
 *
 *---------------------------------------------------------------*/

template<typename LinkInputOrOutput>
void CDepLinkParser::initLinkCache(const LinkInputOrOutput &sentence) {

//	std::cout << "size: " << sentence.size() << "\n";
//	std::cout << "sentence:\n" << sentence;
//	std::cout << "\n";

	m_lCacheCoNLLLemma.resize(sentence.size());
	m_lCacheCoNLLCPOS.resize(sentence.size());
	m_lCacheCoNLLFeats.resize(sentence.size());
	for (unsigned i = 0; i < sentence.size(); ++i) {
		m_lCacheCoNLLLemma[i].load(_conll_or_empty(sentence.at(i).lemma));
		m_lCacheCoNLLCPOS[i].load(_conll_or_empty(sentence.at(i).ctag));
		m_lCacheCoNLLFeats[i].clear();
		if (sentence.at(i).feats != "_")
		readCoNLLFeats(m_lCacheCoNLLFeats[i], sentence.at(i).feats);
	}
}

/*---------------------------------------------------------------
 *
 * parse_conll - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse
 *
 *--------------------------------------------------------------*/

void CDepLinkParser::parse_links(const LinkInput &sentence, LinkOutput *retval,
		int nBest, SCORE_TYPE *scores) {

	static CDependencyLinkParse empty;
	static CTwoStringVector input;
	static CDependencyLinkParse outout[AGENDA_SIZE];

	assert(m_bLinks);

//	initCoNLLCache(sentence);
	initLinkCache(sentence);

	/**
	 * Edited by JK
	 */
	//conllSentence = sentence;
	//end
	sentence.toTwoStringVector(input);

	//TODO
	for (int i = 0; i < nBest; ++i) {
		// clear the outout sentences
		retval[i].clear();
		outout[i].clear();
		if (scores)
		scores[i] = 0;//pGenerator->score;
	}

	work(false, input, outout, empty, nBest, scores);

	for (int i = 0; i < std::min(nBest, m_Agenda->generatorSize()); ++i) {
		// now make the conll format stype outout
		retval[i].fromLinkInput(sentence);
		retval[i].copyDependencyHeads(outout[i]);
	}
}

/*---------------------------------------------------------------
 *
 * train_links - similar to train_conll that works with links
 *
 *---------------------------------------------------------------*/
void CDepLinkParser::train_links(const LinkOutput &correct, int round) {

	static CTwoStringVector sentence;
	static CDependencyLinkParse outout;
	static CDependencyLinkParse reference;

	assert(m_bLinks);
	assert(IsProjectiveDependencyTree(correct));

	initLinkCache(correct);
	correct.toDependencyTree(reference);
	UnparseSentence(&reference, &sentence);

//	std::cout << "correct: \n" << correct << "\n";
//	std::cout << "outout: \n" << outout << "\n";
//	std::cout << "reference: \n" << reference << "\n";

	// The following code does update for each processing stage
	m_nTrainingRound = round;
	work(true, sentence, &outout, reference, 1, 0);

}

#endif

