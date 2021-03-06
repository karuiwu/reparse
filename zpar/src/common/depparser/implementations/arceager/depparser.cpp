// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * depparser.cpp - the action based parser implementation       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

#include "depparser.h"
#include "depparser_weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;
//using namespace TARGET_LANGUAGE::depparser;

const CWord g_emptyWord("");
const CTaggedWord<CTag, TAG_SEPARATOR> g_emptyTaggedWord;
const CTag g_noneTag = CTag::NONE;

#define cast_weights static_cast<CWeight*>(m_weights)
#define refer_or_allocate_tuple2(x, o1, o2) { if (amount == 0) x.refer(o1, o2); else x.allocate(o1, o2); }
#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }
#define _conll_or_empty(x) (x == "_" ? "" : x)

/*===============================================================
 *
 * CDepParser - the depparser for TARGET_LANGUAGE 
 *
 *==============================================================*/

/*---------------------------------------------------------------
 * 
 * getOrUpdateStackScore - manipulate the score from stack 
 *
 *---------------------------------------------------------------*/

void CDepParser::getOrUpdateStackScore(const CStateItem *item,
		CPackedScoreType<SCORE_TYPE, action::MAX> &retval,
		const unsigned &action, SCORE_TYPE amount, int round) {

	const int &st_index = item->stackempty() ? -1 : item->stacktop(); // stack top

#ifdef DEPENDENCIES
	const int &sth_index = st_index == -1 ? -1 : item->head(st_index); // stack top head
	const int &sthh_index = sth_index == -1 ? -1 : item->head(sth_index);// stack top head
	const int &stld_index = st_index == -1 ? -1 : item->leftdep(st_index);// leftmost dep of stack
	const int &strd_index = st_index == -1 ? -1 : item->rightdep(st_index);// rightmost dep st
	const int &stl2d_index = stld_index == -1 ? -1 : item->sibling(stld_index);// left 2ndmost dep of stack
	const int &str2d_index = strd_index == -1 ? -1 : item->sibling(strd_index);// right 2ndmost dep st
#endif

#ifdef LINKS
	std::vector<int> sth_indices; // stack top head
	if(st_index != -1){
		sth_indices = item->head(st_index);
	}

	std::vector<int> sthh_indices; // stack top head
	if(!sth_indices.empty()){
		std::vector<int> temp_sthh_indices;
		for(unsigned i=0; i<sth_indices.size(); i++){
			std::vector<int> heads = item->head(sth_indices[i]);
			for(unsigned j=0; j<heads.size(); j++){
				temp_sthh_indices.push_back(heads[j]);
			}
		}
		sthh_indices = temp_sthh_indices;
		//sthh_indices = item->head(sth_index);
	}

	std::vector<int> stld_indices; // leftmost dep of stack
	if(st_index != -1){
		stld_indices = item->leftdep(st_index);
	}

	std::vector<int> strd_indices; // rightmost dep st
	if(st_index != -1){
		strd_indices = item->rightdep(st_index);
	}

	std::vector<int> stl2d_indices; // left 2ndmost dep of stack
	if(!stld_indices.empty()){
		std::vector<int> temp_stl2d_indices;
		for(unsigned i=0; i<stld_indices.size(); i++){
			std::vector<int> sibs = item->sibling(stld_indices[i]);
			for(unsigned j=0; j<sibs.size(); j++){
				temp_stl2d_indices.push_back(sibs[j]);
			}
		}
		stl2d_indices = temp_stl2d_indices;
//		stl2d_indices = item->sibling(stld_indices);
	}
	std::vector<int> str2d_indices; // right 2ndmost dep st
	if(!strd_indices.empty()){
		std::vector<int> temp_str2d_indices;
		for(unsigned i=0; i<strd_indices.size(); i++){
			std::vector<int> sibs = item->sibling(strd_indices[i]);
			for(unsigned j=0; j<sibs.size(); j++){
				temp_str2d_indices.push_back(sibs[j]);
			}
		}
		str2d_indices = temp_str2d_indices;
//		str2d_indices = item->sibling(strd_indices);
	}

//	const std::vector<int> &sth_index = (st_index == -1 ? std::vector<int>() : item->head(st_index)); // stack top head
//	const std::vector<int> &sthh_index = (sth_index == -1 ? std::vector<int>() : item->head(sth_index));// stack top head
//	const std::vector<int> &stld_index = (st_index == -1 ? std::vector<int>() : item->leftdep(st_index));// leftmost dep of stack
//	const std::vector<int> &strd_index = (st_index == -1 ? std::vector<int>() : item->rightdep(st_index));// rightmost dep st
//	const std::vector<int> &stl2d_index = (stld_index == -1 ? std::vector<int>() : item->sibling(stld_index));// left 2ndmost dep of stack
//	const std::vector<int> &str2d_index = (strd_index == -1 ? std::vector<int>() : item->sibling(strd_index));// right 2ndmost dep st
#endif

	const int &n0_index =
			(unsigned) item->size() == m_lCache.size() ? -1 : item->size(); // next
	assert(n0_index < static_cast<int>(m_lCache.size())); // the next index shouldn't exceed sentence

#ifdef DEPENDENCIES
	const int &n0ld_index = n0_index == -1 ? -1 : item->leftdep(n0_index); // leftmost dep of next
	const int &n0l2d_index = n0ld_index == -1 ? -1 : item->sibling(n0ld_index);// leftmost dep of next
#endif


#ifdef LINKS
	std::vector<int> n0ld_indices;
	if(n0_index != -1){
		n0ld_indices = item->leftdep(n0_index);
	}
	std::vector<int> n0l2d_indices;
	if(!n0ld_indices.empty()){
		std::vector<int> temp_n0l2d_indices;
		for(unsigned i=0; i<n0ld_indices.size(); i++){
			std::vector<int> sibs = item->sibling(n0ld_indices[i]);
			for(unsigned j=0; j<sibs.size(); j++){
				temp_n0l2d_indices.push_back(sibs[j]);
			}
		}
		n0l2d_indices = temp_n0l2d_indices;
//		n0l2d_indices = item->sibling(n0ld_index);
	}

//	const int &n0ld_index = n0_index == -1 ? -1 : item->leftdep(n0_index)[0]; // leftmost dep of next
//	const int &n0l2d_index = n0ld_index == -1 ? -1 : item->sibling(n0ld_index)[0];// leftmost dep of next
#endif

	const int &ht_index = item->headstackempty() ? -1 : item->headstacktop(); // headstack
	const int &ht2_index =
			item->headstacksize() < 2 ?
					-1 : item->headstackitem(item->headstacksize() - 2); // headstack 2nd
	static int n1_index;
	static int n2_index;
	static int n3_index;
	n1_index = (unsigned) (n0_index + 1) < m_lCache.size() ? n0_index + 1 : -1;
	n2_index = (unsigned) (n0_index + 2) < m_lCache.size() ? n0_index + 2 : -1;
	n3_index = (unsigned) (n0_index + 3) < m_lCache.size() ? n0_index + 3 : -1;


	// Get the tagged word from the all the specified indices
#ifdef DEPENDENCIES
	const CTaggedWord<CTag, TAG_SEPARATOR> &st_word_tag =
			st_index == -1 ? g_emptyTaggedWord : m_lCache[st_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &sth_word_tag =
			sth_index == -1 ? g_emptyTaggedWord : m_lCache[sth_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &sthh_word_tag =
			sthh_index == -1 ? g_emptyTaggedWord : m_lCache[sthh_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &stld_word_tag =
			stld_index == -1 ? g_emptyTaggedWord : m_lCache[stld_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &strd_word_tag =
			strd_index == -1 ? g_emptyTaggedWord : m_lCache[strd_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &stl2d_word_tag =
			stl2d_index == -1 ? g_emptyTaggedWord : m_lCache[stl2d_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &str2d_word_tag =
			str2d_index == -1 ? g_emptyTaggedWord : m_lCache[str2d_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &n0_word_tag =
			n0_index == -1 ? g_emptyTaggedWord : m_lCache[n0_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &n0ld_word_tag =
			n0ld_index == -1 ? g_emptyTaggedWord : m_lCache[n0ld_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &n0l2d_word_tag =
			n0l2d_index == -1 ? g_emptyTaggedWord : m_lCache[n0l2d_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &n1_word_tag =
			n1_index == -1 ? g_emptyTaggedWord : m_lCache[n1_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &n2_word_tag =
			n2_index == -1 ? g_emptyTaggedWord : m_lCache[n2_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &ht_word_tag =
			ht_index == -1 ? g_emptyTaggedWord : m_lCache[ht_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &ht2_word_tag =
			ht2_index == -1 ? g_emptyTaggedWord : m_lCache[ht2_index];
#endif

	// Get the tagged words from all the specified indices
	// TODO take the first off of every list for now. Return the full array later.
#ifdef LINKS

	const CTaggedWord<CTag, TAG_SEPARATOR> &st_word_tag =
			st_index == -1 ? g_emptyTaggedWord : m_lCache[st_index];

	//TODO
	const CTaggedWord<CTag, TAG_SEPARATOR> &sth_word_tag =
			sth_indices.empty() ? g_emptyTaggedWord : m_lCache[sth_indices[0]];
	const CTaggedWord<CTag, TAG_SEPARATOR> &sthh_word_tag =
			sthh_indices.empty() ? g_emptyTaggedWord : m_lCache[sthh_indices[0]];
	const CTaggedWord<CTag, TAG_SEPARATOR> &stld_word_tag =
			stld_indices.empty() ? g_emptyTaggedWord : m_lCache[stld_indices[0]];
	const CTaggedWord<CTag, TAG_SEPARATOR> &strd_word_tag =
			strd_indices.empty() ? g_emptyTaggedWord : m_lCache[strd_indices[0]];
	const CTaggedWord<CTag, TAG_SEPARATOR> &stl2d_word_tag =
			stl2d_indices.empty() ? g_emptyTaggedWord : m_lCache[stl2d_indices[0]];
	const CTaggedWord<CTag, TAG_SEPARATOR> &str2d_word_tag =
			str2d_indices.empty() ? g_emptyTaggedWord : m_lCache[str2d_indices[0]];

	const CTaggedWord<CTag, TAG_SEPARATOR> &n0_word_tag =
			n0_index == -1 ? g_emptyTaggedWord : m_lCache[n0_index];

	const CTaggedWord<CTag, TAG_SEPARATOR> &n0ld_word_tag =
			n0ld_indices.empty() ? g_emptyTaggedWord : m_lCache[n0ld_indices[0]];
	const CTaggedWord<CTag, TAG_SEPARATOR> &n0l2d_word_tag =
			n0l2d_indices.empty() ? g_emptyTaggedWord : m_lCache[n0l2d_indices[0]];

	const CTaggedWord<CTag, TAG_SEPARATOR> &n1_word_tag =
			n1_index == -1 ? g_emptyTaggedWord : m_lCache[n1_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &n2_word_tag =
			n2_index == -1 ? g_emptyTaggedWord : m_lCache[n2_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &ht_word_tag =
			ht_index == -1 ? g_emptyTaggedWord : m_lCache[ht_index];
	const CTaggedWord<CTag, TAG_SEPARATOR> &ht2_word_tag =
			ht2_index == -1 ? g_emptyTaggedWord : m_lCache[ht2_index];
#endif

	const CWord &st_word = st_word_tag.word;
	const CWord &sth_word = sth_word_tag.word;
	const CWord &sthh_word = sthh_word_tag.word;
	const CWord &stld_word = stld_word_tag.word;
	const CWord &strd_word = strd_word_tag.word;
	const CWord &stl2d_word = stl2d_word_tag.word;
	const CWord &str2d_word = str2d_word_tag.word;
	const CWord &n0_word = n0_word_tag.word;
	const CWord &n0ld_word = n0ld_word_tag.word;
	const CWord &n0l2d_word = n0l2d_word_tag.word;
	const CWord &n1_word = n1_word_tag.word;
	const CWord &n2_word = n2_word_tag.word;
	const CWord &ht_word = ht_word_tag.word;
	const CWord &ht2_word = ht2_word_tag.word;

	const CTag &st_tag = st_word_tag.tag;
	const CTag &sth_tag = sth_word_tag.tag;
	const CTag &sthh_tag = sthh_word_tag.tag;
	const CTag &stld_tag = stld_word_tag.tag;
	const CTag &strd_tag = strd_word_tag.tag;
	const CTag &stl2d_tag = stl2d_word_tag.tag;
	const CTag &str2d_tag = str2d_word_tag.tag;
	const CTag &n0_tag = n0_word_tag.tag;
	const CTag &n0ld_tag = n0ld_word_tag.tag;
	const CTag &n0l2d_tag = n0l2d_word_tag.tag;
	const CTag &n1_tag = n1_word_tag.tag;
	const CTag &n2_tag = n2_word_tag.tag;
	const CTag &ht_tag = ht_word_tag.tag;
	const CTag &ht2_tag = ht2_word_tag.tag;

#ifdef DEPENDENCIES
	const int &st_label =
	st_index == -1 ? CDependencyLabel::NONE : item->label(st_index);
	const int &sth_label =
	sth_index == -1 ? CDependencyLabel::NONE : item->label(sth_index);
	const int &stld_label =
	stld_index == -1 ? CDependencyLabel::NONE : item->label(stld_index);
	const int &strd_label =
	strd_index == -1 ? CDependencyLabel::NONE : item->label(strd_index);
	const int &stl2d_label =
	stl2d_index == -1 ?
	CDependencyLabel::NONE : item->label(stl2d_index);
	const int &str2d_label =
	str2d_index == -1 ?
	CDependencyLabel::NONE : item->label(strd_index);
	const int &n0ld_label =
	n0ld_index == -1 ? CDependencyLabel::NONE : item->label(n0ld_index);
	const int &n0l2d_label =
	n0l2d_index == -1 ?
	CDependencyLabel::NONE : item->label(n0l2d_index);
#endif

	//TODO These are implemented, but not fully tested.
#ifdef LINKS
//	sth_indices
//	sthh_indices
//	stld_indices
//	strd_indices
//	stl2d_indices
//	str2d_indices
//
//	n0ld_indices
//	n0l2d_indices


	std::vector<unsigned long> st_labels;
	if(st_index != -1){
		st_labels = item->label(st_index);
	}
	std::vector<unsigned long> sth_labels;
	if(!sth_indices.empty()){
		std::vector<unsigned long> temp_sth_labels;
		for(unsigned i=0; i<sth_indices.size(); i++){
			std::vector<unsigned long> sth = item->label(sth_indices[i]);
			for(unsigned j=0; j < sth.size(); j++){
				temp_sth_labels.push_back(sth[j]);
			}
		}
		sth_labels = temp_sth_labels;
//		sthh_labels = item->label(sth_index);
	}
	std::vector<unsigned long> stld_labels;
	if(st_index != -1){
		std::vector<unsigned long> temp_stld_labels;
		for(unsigned i=0; i < stld_indices.size(); i++){
			std::vector<unsigned long> ld_labels = item->label(stld_indices[i]);
			for(unsigned j=0; j < ld_labels.size(); j++){
				temp_stld_labels.push_back(ld_labels[j]);
			}
		}
		stld_labels = temp_stld_labels;
		//		stld_labels = item->leftdep(st_index);
	}
	std::vector<unsigned long> strd_labels;
	if(st_index != -1){
		std::vector<unsigned long> temp_strd_labels;
		for(unsigned i=0; i < strd_indices.size(); i++){
			std::vector<unsigned long> rd_labels = item->label(strd_indices[i]);
			for(unsigned j=0; j < rd_labels.size(); j++){
				temp_strd_labels.push_back(rd_labels[j]);
			}
		}
		strd_labels = temp_strd_labels;
		//strd_labels = item->rightdep(st_index);
	}

	std::vector<unsigned long> stl2d_labels;
	if(!stld_indices.empty()){
		std::vector<unsigned long> temp_stl2d_labels;
		for(unsigned i=0; i < stl2d_indices.size(); i++){
			std::vector<unsigned long> ld_labels = item->label(stl2d_indices[i]);
			for(unsigned j=0; j < ld_labels.size(); j++){
				temp_stl2d_labels.push_back(ld_labels[j]);
			}
		}
		stl2d_labels = temp_stl2d_labels;
		//stld_indices = item->sibling(stld_index);
	}

	std::vector<unsigned long> str2d_labels;
	if(!strd_indices.empty()){
		std::vector<unsigned long> temp_str2d_labels;
		for(unsigned i=0; i < str2d_indices.size(); i++){
			std::vector<unsigned long> rd_labels = item->label(str2d_indices[i]);
			for(unsigned j=0; j < rd_labels.size(); j++){
				temp_str2d_labels.push_back(rd_labels[j]);
			}
		}
		str2d_labels = temp_str2d_labels;
		//str2d_indices = item->sibling(strd_index);
	}

	std::vector<unsigned long> n0ld_labels;
	if(n0_index != -1){
		std::vector<unsigned long> temp_n0ld_labels;
		for(unsigned i=0; i<n0ld_indices.size(); i++){
			std::vector<unsigned long> ld_labels = item->label(n0ld_indices[i]);
			for(unsigned j=0; j<ld_labels.size(); j++){
				temp_n0ld_labels.push_back(ld_labels[j]);
			}
		}
		n0ld_labels = temp_n0ld_labels;
	}
	std::vector<unsigned long> n0l2d_labels;
	if(!n0ld_indices.empty()){
		std::vector<unsigned long> temp_n0l2d_labels;
		for(unsigned i=0; i<n0l2d_indices.size(); i++){
			std::vector<unsigned long> l2d_labels = item->label(n0l2d_indices[i]);
			for(unsigned j=0; j<l2d_labels.size(); j++){
				temp_n0l2d_labels.push_back(l2d_labels[j]);
			}
		}
		n0l2d_labels = temp_n0l2d_labels;
	}

//	const int &st_label = (item->label(st_index)).empty() ? CDependencyLabel::NONE : (int)(item->label(st_index))[0];
//	const int &sth_label = (item->label(sth_index)).empty() ? CDependencyLabel::NONE : (int)(item->label(sth_index))[0];
//	const int &stld_label = (item->label(stld_indices)).empty() ? CDependencyLabel::NONE : (int)(item->label(stld_index))[0];
//	const int &strd_label = (item->label(strd_indices)).empty() ? CDependencyLabel::NONE : (int)(item->label(strd_index))[0];
//	const int &stl2d_label = (item->label(stl2d_indices)).empty() ? CDependencyLabel::NONE : (int)(item->label(stl2d_index))[0];
//	const int &str2d_label = (item->label(strd_indices)).empty() ? CDependencyLabel::NONE : (int)(item->label(strd_index))[0];
//	const int &n0ld_label = (item->label(n0ld_indices)).empty() ? CDependencyLabel::NONE : (int)(item->label(n0ld_index))[0];
//	const int &n0l2d_label = (item->label(n0l2d_indices)).empty() ? CDependencyLabel::NONE : (int)(item->label(n0l2d_index))[0];
#endif

	static int st_n0_dist;
	st_n0_dist = encodeLinkDistance(st_index, n0_index);

	const int st_rarity = st_index == -1 ? 0 : item->rightarity(st_index);
	const int st_larity = st_index == -1 ? 0 : item->leftarity(st_index);
	const int n0_larity = n0_index == -1 ? 0 : item->leftarity(n0_index);

	const CSetOfTags<CDependencyLabel> &st_rtagset =
			st_index == -1 ?
					CSetOfTags<CDependencyLabel>() :
					item->righttagset(st_index);
	const CSetOfTags<CDependencyLabel> &st_ltagset =
			st_index == -1 ?
					CSetOfTags<CDependencyLabel>() : item->lefttagset(st_index);
	const CSetOfTags<CDependencyLabel> &n0_ltagset =
			n0_index == -1 ?
					CSetOfTags<CDependencyLabel>() : item->lefttagset(n0_index);

	static CTwoTaggedWords st_word_tag_n0_word_tag;
	static CTwoWords st_word_n0_word;
	if (amount == 0) {
		st_word_tag_n0_word_tag.refer(&st_word_tag, &n0_word_tag);
		st_word_n0_word.refer(&st_word, &n0_word);
	} else {
		st_word_tag_n0_word_tag.allocate(st_word_tag, n0_word_tag);
		st_word_n0_word.allocate(st_word, n0_word);
	}

	static CTuple2<CWord, CTag> word_tag;
	static CTuple2<CWord, int> word_int;
	static CTuple2<CTag, int> tag_int;
	static CTuple3<CWord, CTag, CTag> word_tag_tag;
	static CTuple3<CWord, CWord, CTag> word_word_tag;
	static CTuple3<CWord, CWord, int> word_word_int;
	static CTuple3<CTag, CTag, int> tag_tag_int;
	static CTuple2<CWord, CSetOfTags<CDependencyLabel> > word_tagset;
	static CTuple2<CTag, CSetOfTags<CDependencyLabel> > tag_tagset;

//	// single
	if (st_index != -1) {
		cast_weights->m_mapSTw.getOrUpdateScore( retval, st_word, action, m_nScoreIndex, amount, round);
		cast_weights->m_mapSTt.getOrUpdateScore( retval, st_tag, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTwt.getOrUpdateScore( retval, st_word_tag, action, m_nScoreIndex, amount, round);
	}

	if (n0_index != -1) {
		cast_weights->m_mapN0w.getOrUpdateScore( retval, n0_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN0t.getOrUpdateScore( retval, n0_tag, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN0wt.getOrUpdateScore( retval, n0_word_tag, action, m_nScoreIndex, amount, round);
	}

	if (n1_index != -1) {
		cast_weights->m_mapN1w.getOrUpdateScore( retval, n1_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN1t.getOrUpdateScore( retval, n1_tag, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN1wt.getOrUpdateScore( retval, n1_word_tag, action, m_nScoreIndex, amount, round);
	}

	if (n2_index != -1) {
		cast_weights->m_mapN2w.getOrUpdateScore( retval, n2_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN2t.getOrUpdateScore( retval, n2_tag, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN2wt.getOrUpdateScore( retval, n2_word_tag, action, m_nScoreIndex, amount, round);
	}

#ifdef DEPENDENCIES
	if (sth_index != -1)
#endif
#ifdef LINKS
	if (!sth_indices.empty())
#endif
	{
		cast_weights->m_mapSTHw.getOrUpdateScore( retval, sth_word, action, m_nScoreIndex, amount, round);
		cast_weights->m_mapSTHt.getOrUpdateScore( retval, sth_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapSTi.getOrUpdateScore( retval, st_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO st_labels[0]
		cast_weights->m_mapSTi.getOrUpdateScore( retval, st_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

#ifdef DEPENDENCIES
	if (sthh_index != -1)
#endif
#ifdef LINKS
	if (!sthh_indices.empty())
#endif
	{
		cast_weights->m_mapSTHHw.getOrUpdateScore( retval, sthh_word, action, m_nScoreIndex, amount, round);
		cast_weights->m_mapSTHHt.getOrUpdateScore( retval, sthh_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapSTHi.getOrUpdateScore( retval, sth_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO sth_labels[0]
		cast_weights->m_mapSTHi.getOrUpdateScore( retval, sth_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

#ifdef DEPENDENCIES
	if (stld_index != -1)
#endif
#ifdef LINKS
	if (!stld_indices.empty())
#endif
	{
		cast_weights->m_mapSTLDw.getOrUpdateScore( retval, stld_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTLDt.getOrUpdateScore( retval, stld_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapSTLDi.getOrUpdateScore( retval, stld_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO stld_labels[0]
		cast_weights->m_mapSTLDi.getOrUpdateScore( retval, stld_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

#ifdef DEPENDENCIES
	if (strd_index != -1)
#endif
#ifdef LINKS
	if (!strd_indices.empty())
#endif
	{
		cast_weights->m_mapSTRDw.getOrUpdateScore( retval, strd_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTRDt.getOrUpdateScore( retval, strd_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapSTRDi.getOrUpdateScore( retval, strd_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO strd_labels[0]
		cast_weights->m_mapSTRDi.getOrUpdateScore( retval, strd_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

#ifdef DEPENDENCIES
	if (n0ld_index != -1)
#endif
#ifdef LINKS
	if (!n0ld_indices.empty())
#endif
	{
		cast_weights->m_mapN0LDw.getOrUpdateScore( retval, n0ld_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN0LDt.getOrUpdateScore( retval, n0ld_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapN0LDi.getOrUpdateScore( retval, n0ld_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO n0ld_labels[0]
		cast_weights->m_mapN0LDi.getOrUpdateScore( retval, n0ld_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

#ifdef DEPENDENCIES
	if (stl2d_index != -1)
#endif
#ifdef LINKS
	if (!stl2d_indices.empty())
#endif
	{
		cast_weights->m_mapSTL2Dw.getOrUpdateScore( retval, stl2d_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTL2Dt.getOrUpdateScore( retval, stl2d_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapSTL2Di.getOrUpdateScore( retval, stl2d_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO stl2d_labels[0]
		cast_weights->m_mapSTL2Di.getOrUpdateScore( retval, stl2d_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

#ifdef DEPENDENCIES
	if (str2d_index != -1)
#endif
#ifdef LINKS
	if (!str2d_indices.empty())
#endif
	{
		cast_weights->m_mapSTR2Dw.getOrUpdateScore( retval, str2d_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTR2Dt.getOrUpdateScore( retval, str2d_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapSTR2Di.getOrUpdateScore( retval, str2d_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO str2d_labels[0]
		cast_weights->m_mapSTR2Di.getOrUpdateScore( retval, str2d_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

#ifdef DEPENDENCIES
	if (n0l2d_index != -1)
#endif
#ifdef LINKS
	if (!n0l2d_indices.empty())
#endif
	{
		cast_weights->m_mapN0L2Dw.getOrUpdateScore( retval, n0l2d_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN0L2Dt.getOrUpdateScore( retval, n0l2d_tag, action, m_nScoreIndex, amount, round );
#ifdef DEPENDENCIES
		cast_weights->m_mapN0L2Di.getOrUpdateScore( retval, n0l2d_label, action, m_nScoreIndex, amount, round);
#endif
#ifdef LINKS
		//TODO n0l2d_labels[0]
		cast_weights->m_mapN0L2Di.getOrUpdateScore( retval, n0l2d_labels[0], action, m_nScoreIndex, amount, round);
#endif
	}

	// s0 and n0
	if (st_index != -1) {
		cast_weights->m_mapSTwtN0wt.getOrUpdateScore( retval, st_word_tag_n0_word_tag, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple3(word_word_tag, &st_word, &n0_word, &st_tag);
		cast_weights->m_mapSTwtN0w.getOrUpdateScore( retval, word_word_tag, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple3(word_word_tag, &st_word, &n0_word, &n0_tag);
		cast_weights->m_mapSTwN0wt.getOrUpdateScore( retval, word_word_tag, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple3(word_tag_tag, &st_word, &st_tag, &n0_tag);
		cast_weights->m_mapSTwtN0t.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple3(word_tag_tag, &n0_word, &st_tag, &n0_tag);
		cast_weights->m_mapSTtN0wt.getOrUpdateScore( retval, word_tag_tag, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTwN0w.getOrUpdateScore( retval, st_word_n0_word, action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTtN0t.getOrUpdateScore( retval, CTagSet<CTag, 2>(encodeTags(st_tag,n0_tag)), action, m_nScoreIndex, amount, round );
	}

	if (st_index != -1 && n0_index != -1) {
		cast_weights->m_mapN0tN1t.getOrUpdateScore( retval, CTagSet<CTag, 2>(encodeTags(n0_tag,n1_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN0tN1tN2t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(n0_tag,n1_tag,n2_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTtN0tN1t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n1_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTtN0tN0LDt.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,n0_tag,n0ld_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapN0tN0LDtN0L2Dt.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(n0_tag,n0ld_tag,n0l2d_tag)), action, m_nScoreIndex, amount, round );
	}
	if (st_index != -1) {
		cast_weights->m_mapSTHtSTtN0t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(sth_tag,st_tag,n0_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTHHtSTHtSTt.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(sthh_tag, sth_tag,st_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTtSTLDtN0t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,n0_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTtSTLDtSTL2Dt.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,stld_tag,stl2d_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTtSTRDtN0t.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,n0_tag)), action, m_nScoreIndex, amount, round );
		cast_weights->m_mapSTtSTRDtSTR2Dt.getOrUpdateScore( retval, CTagSet<CTag, 3>(encodeTags(st_tag,strd_tag,str2d_tag)), action, m_nScoreIndex, amount, round );
	}

	// distance
	if (st_index != -1 && n0_index != -1) {
		refer_or_allocate_tuple2(word_int, &st_word, &st_n0_dist);
		cast_weights->m_mapSTwd.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_int, &st_tag, &st_n0_dist);
		cast_weights->m_mapSTtd.getOrUpdateScore( retval, tag_int, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple2(word_int, &n0_word, &st_n0_dist);
		cast_weights->m_mapN0wd.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple2(tag_int, &n0_tag, &st_n0_dist);
		cast_weights->m_mapN0td.getOrUpdateScore( retval, tag_int, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple3(word_word_int, &st_word, &n0_word, &st_n0_dist);
		cast_weights->m_mapSTwN0wd.getOrUpdateScore( retval, word_word_int, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple3(tag_tag_int, &st_tag, &n0_tag, &st_n0_dist);
		cast_weights->m_mapSTtN0td.getOrUpdateScore( retval, tag_tag_int, action, m_nScoreIndex, amount, round );
	}

	// st arity
	if (st_index != -1) {
		refer_or_allocate_tuple2(word_int, &st_word, &st_rarity);
		cast_weights->m_mapSTwra.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_int, &st_tag, &st_rarity);
		cast_weights->m_mapSTtra.getOrUpdateScore( retval, tag_int, action, m_nScoreIndex, amount, round );
		refer_or_allocate_tuple2(word_int, &st_word, &st_larity);
		cast_weights->m_mapSTwla.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_int, &st_tag, &st_larity);
		cast_weights->m_mapSTtla.getOrUpdateScore( retval, tag_int, action, m_nScoreIndex, amount, round );
	}

	// n0 arity
	if (n0_index != -1) {
		refer_or_allocate_tuple2(word_int, &n0_word, &n0_larity);
		cast_weights->m_mapN0wla.getOrUpdateScore( retval, word_int, action, m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_int, &n0_tag, &n0_larity);
		cast_weights->m_mapN0tla.getOrUpdateScore( retval, tag_int, action, m_nScoreIndex, amount, round );
	}

	// st labelset
	if (st_index != -1) {
		refer_or_allocate_tuple2(word_tagset, &st_word, &st_rtagset);
		cast_weights->m_mapSTwrp.getOrUpdateScore( retval, word_tagset, action, m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_tagset, &st_tag, &st_rtagset);
		cast_weights->m_mapSTtrp.getOrUpdateScore( retval, tag_tagset, action, m_nScoreIndex, amount, round );

		refer_or_allocate_tuple2(word_tagset, &st_word, &st_ltagset);
		cast_weights->m_mapSTwlp.getOrUpdateScore( retval, word_tagset, action, m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_tagset, &st_tag, &st_ltagset);
		cast_weights->m_mapSTtlp.getOrUpdateScore( retval, tag_tagset, action, m_nScoreIndex, amount, round );
	}

	// n0 labelset
	if (n0_index != -1) {
		refer_or_allocate_tuple2(word_tagset, &n0_word, &n0_ltagset);
		cast_weights->m_mapN0wlp.getOrUpdateScore( retval, word_tagset, action, m_nScoreIndex, amount, round);
		refer_or_allocate_tuple2(tag_tagset, &n0_tag, &n0_ltagset);
		cast_weights->m_mapN0tlp.getOrUpdateScore( retval, tag_tagset, action, m_nScoreIndex, amount, round );
	}

	// Juneki: Link Automata feature. Just the state of the automata at the top of the stack and queue.
	// Lets see if I can add some of my own features to the parser

	if (st_index != -1) {
		const State* st_index_automata = item->automataLookup(st_index);

//		for (int i = 0; i < st_index_automata->history.size(); i++) {
//			StateHistory h = st_index_automata->history[i];
//			int hash = h.actionState;
//			int cost = h.cost;
//			cast_weights->automata_mapST.getOrUpdateScore( retval, hash, action, m_nScoreIndex, amount, round);
//		}

		cast_weights->automata_mapST.getOrUpdateScore( retval, *st_index_automata, action, m_nScoreIndex, amount, round);
//		st_index_automata->print();
//		std::cout << "\n";

	}
	if (n0_index != -1) {
		const State* n0_index_automata = item->automataLookup(n0_index);
//		for (int i = 0; i < n0_index_automata->history.size(); i++) {
//			StateHistory h = n0_index_automata->history[i];
//			int hash = h.actionState;
//			int cost = h.cost;
//			cast_weights->automata_mapN0.getOrUpdateScore( retval, hash, action, m_nScoreIndex, amount, round);
//		}

		cast_weights->automata_mapN0.getOrUpdateScore( retval, *n0_index_automata, action, m_nScoreIndex, amount, round);
//		n0_index_automata->print();
//		std::cout << "\n";

	}

	//end

	if (m_bCoNLL) {

		static unsigned i;

		if (st_index != -1) {
			if (!m_lCacheCoNLLLemma[st_index].empty())
				cast_weights->m_mapSTl.getOrUpdateScore( retval, m_lCacheCoNLLLemma[st_index], action, m_nScoreIndex, amount, round);
				if (m_lCacheCoNLLCPOS[st_index] != CCoNLLCPOS()) cast_weights->m_mapSTc.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[st_index], action, m_nScoreIndex, amount, round);
				for (i=0; i<m_lCacheCoNLLFeats[st_index].size(); ++i)
				cast_weights->m_mapSTf.getOrUpdateScore( retval, m_lCacheCoNLLFeats[st_index][i], action, m_nScoreIndex, amount, round);
			} // if (st_index!=-1)

		if (n0_index != -1) {
			if (!m_lCacheCoNLLLemma[n0_index].empty())
				cast_weights->m_mapN0l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n0_index], action, m_nScoreIndex, amount, round);
				if (m_lCacheCoNLLCPOS[n0_index] != CCoNLLCPOS()) cast_weights->m_mapN0c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n0_index], action, m_nScoreIndex, amount, round);
				for (i=0; i<m_lCacheCoNLLFeats[n0_index].size(); ++i)
				cast_weights->m_mapN0f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n0_index][i], action, m_nScoreIndex, amount, round);
			} // if (n0_index!=-1)

		if (n1_index != -1) {
			if (!m_lCacheCoNLLLemma[n1_index].empty())
				cast_weights->m_mapN1l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n1_index], action, m_nScoreIndex, amount, round);
				if (m_lCacheCoNLLCPOS[n1_index] != CCoNLLCPOS()) cast_weights->m_mapN1c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n1_index], action, m_nScoreIndex, amount, round);
				for (i=0; i<m_lCacheCoNLLFeats[n1_index].size(); ++i)
				cast_weights->m_mapN1f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n1_index][i], action, m_nScoreIndex, amount, round);
			} // if (n1_index!=-1)

			// Edited by J
			// See if I can add the n2_index as a feature for the parser
//		if (n2_index != -1) {
//			if (!m_lCacheCoNLLLemma[n2_index].empty())
//				cast_weights->m_mapN2l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n2_index], action, m_nScoreIndex, amount, round);
//				if (m_lCacheCoNLLCPOS[n2_index] != CCoNLLCPOS()) cast_weights->m_mapN2c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n2_index], action, m_nScoreIndex, amount, round);
//				for (i=0; i<m_lCacheCoNLLFeats[n2_index].size(); ++i)
//				cast_weights->m_mapN2f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n2_index][i], action, m_nScoreIndex, amount, round);
//			}

//		if(n3_index != -1){
//			if(!m_lCacheCoNLLLemma[n3_index].empty())
//				cast_weights->m_mapN3l.getOrUpdateScore( retval, m_lCacheCoNLLLemma[n3_index], action, m_nScoreIndex, amount, round);
//				if (m_lCacheCoNLLCPOS[n3_index] != CCoNLLCPOS()) cast_weights->m_mapN3c.getOrUpdateScore( retval, m_lCacheCoNLLCPOS[n3_index], action, m_nScoreIndex, amount, round);
//				for (i=0; i<m_lCacheCoNLLFeats[n3_index].size(); ++i)
//				cast_weights->m_mapN2f.getOrUpdateScore( retval, m_lCacheCoNLLFeats[n3_index][i], action, m_nScoreIndex, amount, round);
//			}

		}

	}

	/*---------------------------------------------------------------
	 *
	 * getGlobalScore - get the score of a parse tree
	 *
	 * Inputs: parse graph
	 *
	 *---------------------------------------------------------------*/

SCORE_TYPE CDepParser::getGlobalScore(const CDependencyParse &parsed) {
	THROW("depparser.cpp: getGlobalScore unsupported");
}

/*---------------------------------------------------------------
 *
 * updateScores - update the score std::vector 
 *
 * This method is different from updateScoreVector in that
 * 1. It is for external call
 * 2. The tagging sequences for parsed and correct may differ
 *
 * Inputs: the parsed and the correct example
 *
 *---------------------------------------------------------------*/

void CDepParser::updateScores(const CDependencyParse & parsed,
		const CDependencyParse & correct, int round) {
	THROW("depparser.cpp: updateScores unsupported");
}

/*---------------------------------------------------------------
 *
 * updateScoreForState - update a single positive or negative output
 *
 *--------------------------------------------------------------*/

inline void CDepParser::updateScoreForState(const CStateItem &from,
		const CStateItem *outout, const SCORE_TYPE &amount) {
	static CStateItem item(&m_lCache);

	static unsigned action;
	static CPackedScoreType<SCORE_TYPE, action::MAX> empty;
	item = from;
	while (item != *outout) {
		action = item.FollowMove(outout);
		getOrUpdateStackScore(&item, empty, action, amount, m_nTrainingRound);
#ifdef LABELED
//      if ( action::getLabel(action) )
//         getOrUpdateStackScore( &item, empty, action::getUnlabeledAction(action), amount, m_nTrainingRound );
#endif
		item.Move(action);
	}
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CDepParser::updateScoresForStates(const CStateItem *outout,
		const CStateItem *correct, SCORE_TYPE amount_add,
		SCORE_TYPE amount_subtract) {

	// do not update those steps where they are correct
	static CStateItem item(&m_lCache);
	static unsigned action, correct_action;
	item.clear();
	while (item != *outout) {
		action = item.FollowMove(outout);
		correct_action = item.FollowMove(correct);
		if (action == correct_action)
			item.Move(action);
		else
			break;
	}

	// for the necessary information for the correct and outout parsetree
	updateScoreForState(item, correct, amount_add);
	updateScoreForState(item, outout, amount_subtract);

	m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * reduce - helper function
 *
 *--------------------------------------------------------------*/

void CDepParser::reduce(const CStateItem *item,
		const CPackedScoreType<SCORE_TYPE, action::MAX> &scores) {
	static action::CScoredAction scoredaction;
	// update stack score
	scoredaction.action = action::REDUCE;
	scoredaction.score = item->score + scores[scoredaction.action];
	m_Beam->insertItem(&scoredaction);
}

#ifdef DEPENDENCIES
/*---------------------------------------------------------------
 *
 * arcleft - helping function
 *
 *--------------------------------------------------------------*/

void CDepParser::arcleft(const CStateItem *item,
		const CPackedScoreType<SCORE_TYPE, action::MAX> &scores) {
	static action::CScoredAction scoredaction;
	static unsigned label;
#ifdef LABELED
	for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
		if ( !m_weights->rules() || canAssignLabel(m_lCache, item->size(), item->stacktop(), label) ) {
			scoredaction.action = action::encodeAction(action::ARC_LEFT, label);
			scoredaction.score = item->score + scores[scoredaction.action];
			//+scores[action::ARC_LEFT];
			m_Beam->insertItem(&scoredaction);

		}
	}
#else
	scoredaction.action = action::ARC_LEFT;
	scoredaction.score = item->score + scores[scoredaction.action];
	m_Beam->insertItem(&scoredaction);
#endif
}

/*---------------------------------------------------------------
 *
 * arcright - helping function
 *
 *--------------------------------------------------------------*/

void CDepParser::arcright(const CStateItem *item,
		const CPackedScoreType<SCORE_TYPE, action::MAX> &scores) {
	static action::CScoredAction scoredaction;
	static unsigned label;
#ifdef LABELED
	for (label=CDependencyLabel::FIRST; label<CDependencyLabel::COUNT; ++label) {
		if ( !m_weights->rules() || canAssignLabel(m_lCache, item->stacktop(), item->size(), label) ) {
			scoredaction.action = action::encodeAction(action::ARC_RIGHT, label);
			scoredaction.score = item->score + scores[scoredaction.action];
			//+scores[action::ARC_RIGHT];
			m_Beam->insertItem(&scoredaction);
		}
	}
#else
	scoredaction.action = action::ARC_RIGHT;
	scoredaction.score = item->score + scores[scoredaction.action];
	m_Beam->insertItem(&scoredaction);
#endif
}

#endif

/*---------------------------------------------------------------
 *
 *   - help function
 *
 *--------------------------------------------------------------*/

void CDepParser::shift(const CStateItem *item,
		const CPackedScoreType<SCORE_TYPE, action::MAX> &scores) {
	static action::CScoredAction scoredaction;
	// update stack score
	scoredaction.action = action::SHIFT;
	scoredaction.score = item->score + scores[scoredaction.action];
	m_Beam->insertItem(&scoredaction);
}

/*---------------------------------------------------------------
 *
 * poproot - help function
 *
 *--------------------------------------------------------------*/

void CDepParser::poproot(const CStateItem *item,
		const CPackedScoreType<SCORE_TYPE, action::MAX> &scores) {
	static action::CScoredAction scoredaction;
	// update stack score
	scoredaction.action = action::POP_ROOT;
	scoredaction.score = item->score + scores[scoredaction.action];
	m_Beam->insertItem(&scoredaction);
}

//#ifdef DEPENDENCIES
/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CDependencyParse 
 *
 *--------------------------------------------------------------*/

void CDepParser::work(const bool bTrain, const CTwoStringVector &sentence,
		CDependencyParse *retval, const CDependencyParse &correct, int nBest,
		SCORE_TYPE *scores) {

#ifdef DEPENDENCIES

	/**
	 * Edited by JK
	 */
//	if (bTrain) {
//		//skips the ROOT
//		featureCollection->writeToMap(conllSentenceTrain);
//	}
	//end
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
			// the root
			if (correct[index].head == DEPENDENCY_LINK_NO_HEAD
					&& canBeRoot(m_lCache[index].tag.code()) == false) {
				TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " can be root.");
				bContradictsRules = true;
			}
#endif

#ifdef SINGLE_PARENT
			// head left
			if (correct[index].head < index
					&& hasLeftHead(m_lCache[index].tag.code()) == false) {
				TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has left head.");
				bContradictsRules = true;
			}
			// head right
			if (correct[index].head > index
					&& hasRightHead(m_lCache[index].tag.code()) == false) {
				TRACE("Rule contradiction: " << m_lCache[index].tag.code() << " has right head.");
				bContradictsRules = true;
			}
#endif

		}
	}

	/**
	 * Edited by JK
	 */
//	if (!bTrain) {
//		oracle->readInSentence(conllSentence);
//	}
	//end
	// initialise agenda
	m_Agenda->clear();
	pCandidate.clear();// restore state using clean
	m_Agenda->pushCandidate(&pCandidate);// and push it back
	m_Agenda->nextRound();// as the generator item
	if (bTrain)
	correctState.clear();

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
			m_lCacheLabel.push_back(CDependencyLabel(correct[index].label));
			if (m_weights->rules() && !canAssignLabel(m_lCache, correct[index].head, index, m_lCacheLabel[index])) {
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

		//JUNEKI: Will the parser still try to parse when this is commented out?
		// none can this find with pruning ???
//		if (m_Agenda->generatorSize() == 0) {
//			WARNING("parsing failed");
//			return;
//		}

		pGenerator = m_Agenda->generatorStart();
		// iterate generators
		for (int j = 0; j < m_Agenda->generatorSize(); ++j) {
//			/**
//			 * Edited by JK
//			 */
//			std::vector<int, allocator<int> >::const_iterator stackIter =
//					pGenerator->Stack.end();
//			int stackWord = -1;
//			if (!pGenerator->Stack.empty()) {
//				stackWord = *(stackIter - 1);
//			}

			int nextWord = pGenerator->NextWord;

//			if (!bTrain) {
//				int parent = 2;
//				int child = 1;
//				tempPGeneratorPointer->removeArc(parent, child);
//				tempPGeneratorPointer->makeArc(parent,child);
//			}

			// These boolean values check to see if we can do certain actions during a parse.
			// This allows us to implement generalized arc eager dependency parsing of tree inputs.

//			bool topOfStack_isParentOf_topOfBuffer = pGenerator->isParent(
//					stackWord, nextWord);
//			bool topOfBuffer_isParentOf_topOfStack = pGenerator->isParent(
//					nextWord, stackWord);
//
//			bool topOfStack_hasRightChildOnBuffer =
//					pGenerator->childOnBufferCheck(stackWord);
//			//if next word on buffer has children on the stack
//			bool topOfBuffer_hasLeftChildOnStack =
//					pGenerator->childOnStackCheck(nextWord);
//			bool topOfStack_hasParent = pGenerator->hasParent(stackWord);
//			bool topOfBuffer_hasParent = pGenerator->hasParent(nextWord);
//
//			bool noLeftArc = topOfStack_hasParent
//					|| topOfStack_hasRightChildOnBuffer;
//
//			bool noRightArc = topOfBuffer_hasParent
//					|| topOfBuffer_hasLeftChildOnStack;
//
//			bool noReduce = topOfStack_hasRightChildOnBuffer;
//			bool mustReduce = topOfStack_hasParent
//					|| topOfBuffer_hasLeftChildOnStack;
//
//			bool noShift = topOfBuffer_hasLeftChildOnStack;
//			bool mustShift = topOfStack_isParentOf_topOfBuffer;

//			if (!bTrain) {
//				cout << "topOfStack_isParentOf_topOfBuffer "
//						<< topOfStack_isParentOf_topOfBuffer << "\n";
//				cout << "topOfBuffer_isParentOf_topOfStack "
//						<< topOfBuffer_isParentOf_topOfStack << "\n";
//				cout << "noLeftArc: " << noLeftArc << "\n";
//				cout << "noRightArc: " << noRightArc << "\n";
//				cout << "noReduce: " << noReduce << "\n";
//				cout << "mustReduce: " << mustReduce << "\n";
//				cout << "noShift: " << noShift << "\n";
//			cout << "\n";
//			}

			//end

			// for the state items that already contain all words
			m_Beam->clear();
			packed_scores.reset();
			getOrUpdateStackScore(pGenerator, packed_scores, action::NO_ACTION);
			if (pGenerator->size() == length) {
				assert(pGenerator->stacksize() != 0);
				if (pGenerator->stacksize() > 1) {
					reduce(pGenerator, packed_scores);
				} else {
					poproot(pGenerator, packed_scores);

				}
			}
			// Edited by J
			// Here, we check to see if we already have arcs that have been made that we need to take again.
//			else if (topOfStack_isParentOf_topOfBuffer) {
//#ifdef DEPENDENCIES
//				arcright(pGenerator, packed_scores);
//#endif
//#ifdef LINKS
//				connectright(pGenerator, packed_scores);
//#endif
//			}
//
//			else if (topOfBuffer_isParentOf_topOfStack) {
//#ifdef DEPENDENCIES
//				arcleft(pGenerator, packed_scores);
//#endif
//#ifdef LINKS
//				connectleft(pGenerator, packed_scores);
//#endif
//
//			}
			//end

			// for the state items that still need more words
			else {
				if (!pGenerator->afterreduce()) { // there are many ways when there are many arcrighted items on the stack and the root need arcleft. force this.
					if ((pGenerator->size() < length - 1
									|| pGenerator->stackempty())
							&&// keep only one global root
							(pGenerator->stackempty() || m_supertags == 0
									|| m_supertags->canShift(pGenerator->size()))
							&&// supertags
							(pGenerator->stackempty() || !m_weights->rules()
									|| canBeRoot(
											m_lCache[pGenerator->size()].tag.code())
									|| hasRightHead(
											m_lCache[pGenerator->size()].tag.code()))// rules
//							&&// JUNEKI: no shift
//							(!noShift)
					) {
						shift(pGenerator, packed_scores);
					}
//					else if (mustShift) { // JUNEKI: must shift
//						shift(pGenerator, packed_scores);
//					}
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

//							&&// JUNEKI: no right arc
//							(!noRightArc)
					) {
//#ifdef DEPENDENCIES
						arcright(pGenerator, packed_scores);
//#endif
//#ifdef LINKS
//						connectright(pGenerator, packed_scores);
//#endif

					}
				}
				if ((!m_bCoNLL && !pGenerator->stackempty())
						|| (m_bCoNLL && pGenerator->stacksize() > 1) // make sure that for conll the first item is not popped
				) {
					if ((pGenerator->head(pGenerator->stacktop())
									!= DEPENDENCY_LINK_NO_HEAD)
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
//#ifdef DEPENDENCIES
							arcleft(pGenerator, packed_scores);
//#endif
//#ifdef LINKS
//							connectleft(pGenerator, packed_scores);
//#endif

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

				/**
				 * Edited by JK
				 */
				if (!bTrain) {
					std::vector<int> buffer;
					//length-1 to account for the presence of ROOT
					for (int k = pCandidate.NextWord; k < length - 1; ++k) {
						buffer.push_back(k);
					}
//					if (!pCandidate.Stack.empty()
//							&& pCandidate.Stack.back()
//									== DEPENDENCY_LINK_NO_HEAD) {
//						oracle->noReduce = true;
//					}
//					std::vector<int> actions = oracle->nextAction(
//							pCandidate.Stack, buffer);
					int action;
#ifdef LABELED
					action = action::getUnlabeledAction(m_Beam->item(i)->action);
#else
					action = m_Beam->item(i)->action & INT_MAX;
#endif

//					if (oracle->isOracleAction(actions, action)) {
//						featureCollection->writeClass("1");
//						//std::cout << "1\t";
//					} else {
//						featureCollection->writeClass("0");
//						//std::cout << "0\t";
//					}

					if (DEBUG && false) {
						std::cout << "Children: " << std::endl;
						siblings_t children = pCandidate.m_Children;
						for (siblings_t::const_iterator it = children.begin();
								it != children.end(); it++) {
							std::cout << "(" << it->first << ", " << "{ ";
							for (std::vector<int>::const_iterator vec_it =
									it->second.begin();
									vec_it != it->second.end(); vec_it++) {
								std::cout << *vec_it << " ";
							}
							std::cout << "}) ";
						}
						std::cout << std::endl;
					}

//					featureCollection->makeFeatures(pCandidate.Stack,
//							std::vector<int>(), pCandidate.m_Children,
//							m_lCacheCoNLLCPOS);
//
//					featureCollection->printFeatures();
					//featureCollection->clear();

					// Feature 0: POS Tags
				}
				//end
			}

			if (bTrain && *pGenerator == correctState) {
				bCorrect = true;
			}
			pGenerator = m_Agenda->generatorNext();
		}

		// JUNEKI: m_Agenda is the "global" beam. Each element in the beam is a CStateItem.
		// JUNEKI: CStateItems are instances of a dependency parser.
		// JUNEKI: pGenerator is a CStateItem
		// JUNEKI: m_Beam is the "local" beam of possible actions to take for a particular dependency parser.

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
			pGenerator->GenerateTree(sentence, retval[i]);
			if (scores)
			scores[i] = pGenerator->score;
		}
	}TRACE("Done, the highest score is: " << m_Agenda->bestGenerator()->score );TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC);

#endif

}

/*---------------------------------------------------------------
 *
 * parse - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse 
 *
 *--------------------------------------------------------------*/

void CDepParser::parse(const CTwoStringVector &sentence,
		CDependencyParse *retval, int nBest, SCORE_TYPE *scores) {

	static CDependencyParse empty;

	assert(!m_bCoNLL);

	for (int i = 0; i < nBest; ++i) {
		// clear the outout sentences
		retval[i].clear();
		if (scores)
			scores[i] = 0; //pGenerator->score;
	}

	work(false, sentence, retval, empty, nBest, scores);

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train(const CDependencyParse &correct, int round) {

	static CTwoStringVector sentence;
	static CDependencyParse outout;

	assert(!m_bCoNLL);
	assert(IsProjectiveDependencyTree(correct));
	UnparseSentence(&correct, &sentence);

	// The following code does update for each processing stage
	m_nTrainingRound = round;
	work(true, sentence, &outout, correct, 1, 0);

}

/*---------------------------------------------------------------
 *
 * initCoNLLCache
 *
 *---------------------------------------------------------------*/

template<typename CCoNLLInputOrOutput>
void CDepParser::initCoNLLCache(const CCoNLLInputOrOutput &sentence) {
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

//#ifdef DEPENDENCIES
/*---------------------------------------------------------------
 *
 * parse_conll - do dependency parsing to a sentence
 *
 * Returns: makes a new instance of CDependencyParse 
 *
 *--------------------------------------------------------------*/

void CDepParser::parse_conll(const CCoNLLInput &sentence, CCoNLLOutput *retval,
		int nBest, SCORE_TYPE *scores) {

	static CDependencyParse empty;
	static CTwoStringVector input;
	static CDependencyParse outout[AGENDA_SIZE];

	assert(m_bCoNLL);

	initCoNLLCache(sentence);
	sentence.toTwoStringVector(input);

	for (int i = 0; i < nBest; ++i) {
		// clear the outout sentences
		retval[i].clear();
		outout[i].clear();
		if (scores)
			scores[i] = 0; //pGenerator->score;
	}

	work(false, input, outout, empty, nBest, scores);

	for (int i = 0; i < std::min(nBest, m_Agenda->generatorSize()); ++i) {
		// now make the conll format stype outout
		retval[i].fromCoNLLInput(sentence);
		retval[i].copyDependencyHeads(outout[i]);
	}

}

/*---------------------------------------------------------------
 *
 * train_conll - train the models with an example
 *
 *---------------------------------------------------------------*/

void CDepParser::train_conll(const CCoNLLOutput &correct, int round) {

	static CTwoStringVector sentence;
	static CDependencyParse outout;
	static CDependencyParse reference;

	assert(m_bCoNLL);
	assert(IsProjectiveDependencyTree(correct));

	initCoNLLCache(correct);
	correct.toDependencyTree(reference);
	UnparseSentence(&reference, &sentence);

	// The following code does update for each processing stage
	m_nTrainingRound = round;
	work(true, sentence, &outout, reference, 1, 0);

}
//#endif

