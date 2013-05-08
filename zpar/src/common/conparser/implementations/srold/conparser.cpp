// Copyright (C) University of Oxford 2010
/****************************************************************
 *                                                              *
 * conparser.cpp - the action based parser implementation       *
 *                                                              *
 * Author: Yue Zhang                                            *
 *                                                              *
 * Computing Laboratory, Oxford. 2007.12                        *
 *                                                              *
 ****************************************************************/

#include "conparser.h"
#include "weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::conparser;

#define cast_weights static_cast<CWeight*>(m_weights)

#define refer_or_allocate_tuple3(x, o1, o2, o3) { if (amount == 0) x.refer(o1, o2, o3); else x.allocate(o1, o2, o3); }

/*===============================================================
 *
 * CConParser - the conparser for TARGET_LANGUAGE 
 *
 *==============================================================*/

/*---------------------------------------------------------------
 *
 * getOrUpdateLinkScore - get or update the score from a link.
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateLinkScore( const unsigned long &head, const unsigned long &mod, SCORE_TYPE amount, int round ) {

   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   unsigned long direction = encodeLinkDirection(head, mod);
   unsigned long dist = encodeLinkSize(head, mod);
   static CCFGSet cf;

   const CTag &ht = m_lCache[head].tag;
   const CWord &hw = m_lCache[head];
   const CTag &mt = m_lCache[mod].tag;
   const CWord &mw = m_lCache[mod];

/*
   cf.clear(); cf.add(ht); cf.add(mt); cf.add(0); cf.add(0);
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);
   cf.clear(); cf.add(ht); cf.add(mt); cf.add(direction); cf.add(0);
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);
   cf.clear(); cf.add(ht); cf.add(mt); cf.add(direction); cf.add(dist);
   nReturn += cast_weights->m_mapHtMt.getOrUpdateScore(cf, m_nScoreIndex, amount, round);

   cf.clear(); cf+=mt; cf+=0; cf+=0;
   nReturn += cast_weights->m_mapHwMt.getOrUpdateScore(std::make_pair(hw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=mt; cf+=direction; cf+=0;
   nReturn += cast_weights->m_mapHwMt.getOrUpdateScore(std::make_pair(hw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=mt; cf+=direction; cf+=dist;
   nReturn += cast_weights->m_mapHwMt.getOrUpdateScore(std::make_pair(hw, cf), m_nScoreIndex, amount, round);

   cf.clear(); cf+=ht; cf+=0; cf+=0;
   nReturn += cast_weights->m_mapHtMw.getOrUpdateScore(std::make_pair(mw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=ht; cf+=direction; cf+=0;
   nReturn += cast_weights->m_mapHtMw.getOrUpdateScore(std::make_pair(mw, cf), m_nScoreIndex, amount, round);
   cf.clear(); cf+=ht; cf+=direction;cf+=dist;
   nReturn += cast_weights->m_mapHtMw.getOrUpdateScore(std::make_pair(mw, cf), m_nScoreIndex, amount, round);
*/
   return nReturn;

}

/*---------------------------------------------------------------
 *
 * getOrUpdateSiblingLinkScore - get or update the score from two links.
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateSiblingLinkScore( const unsigned long &head, const unsigned long &mod, const unsigned long &sibling, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   unsigned long direction = encodeLinkDirection(mod, sibling);
   unsigned long dist = encodeLinkDirectionAndSize(mod, sibling);

   const CTag &ht = m_lCache[head].tag;
   const CWord &hw = m_lCache[head];
   const CTag &mt = m_lCache[mod].tag;
   const CWord &mw = m_lCache[mod];
   const CTag &st = m_lCache[sibling].tag;
   const CWord &sw = m_lCache[sibling];

   unsigned long k = encodeTags(ht, mt, st);
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k, m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k | (direction<<(CTag::SIZE*3)), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTags.getOrUpdateScore(k | (dist<<(CTag::SIZE*3)), m_nScoreIndex, amount, round);

   k = encodeTags(ht, mt);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(std::make_pair(sw, k), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(std::make_pair(sw, k|(direction<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingWordTag.getOrUpdateScore(std::make_pair(sw, k|(dist<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);

   k = encodeTags(ht, st);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(std::make_pair(mw, k), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(std::make_pair(mw, k|(direction<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapSiblingTagWord.getOrUpdateScore(std::make_pair(mw, k|(dist<<(CTag::SIZE*2))), m_nScoreIndex, amount, round);

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateArityScore - get or update score for modifier count
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateArityScore( const unsigned long &head, const unsigned long &arityleft, const unsigned long &arityright, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;return nReturn;

   const CWord &w = m_lCache[head];
   const CTag &t = m_lCache[head].tag;

   nReturn += cast_weights->m_mapHwArityL.getOrUpdateScore(std::make_pair(w, arityleft), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHtArityL.getOrUpdateScore(std::make_pair(t.code(), arityleft), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwArityR.getOrUpdateScore(std::make_pair(w, arityright), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHtArityR.getOrUpdateScore(std::make_pair(t.code(), arityright), m_nScoreIndex, amount, round);

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateHeadScore - get or update score for head label
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateHeadScore( const unsigned long &parent, const unsigned long &child, const unsigned long &head, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   const CWord &w = m_lCache[head];
   const CTag &t = m_lCache[head].tag;
   static CCFGSet cs; cs.clear();
   cs += parent; cs += child;

   nReturn += cast_weights->m_mapHw.getOrUpdateScore(std::make_pair(w, parent), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHt.getOrUpdateScore(std::make_pair(t.code(), parent), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHc.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwc.getOrUpdateScore(std::make_pair(w, cs), m_nScoreIndex, amount, round);
   
   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateHeadModifierScore - get or update score for head 
 *                                and modifier labels
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateHeadModifierScore( const unsigned long &parent, const unsigned long &child, const unsigned long &child1, const unsigned long &head, const unsigned long &mod, const bool &left, const bool &neighbour, SCORE_TYPE amount, int round ) {
   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   assert(left&&head<mod || !left&&head>mod);

   const CWord &hw = m_lCache[head];
   const CWord &mw = m_lCache[mod];
   static CTwoWords hwmw;
   if (amount!=0)
      hwmw.allocate(hw, mw);
   else
      hwmw.refer(&hw, &mw);
   static unsigned long pos;

   static CCFGSet cs; 

   if ( left && neighbour ) {
      pos = 1;
   }
   else if ( left && !neighbour ) {
      pos = 2;
   }
   else if (!left && neighbour) {
      pos = 3;
   }
   else {
      assert(!left && !neighbour);
      pos = 4;
   }
   
   cs.clear(); cs += parent; cs += child; cs += child1; cs += pos;
   nReturn += cast_weights->m_mapHcMc.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHcMw.getOrUpdateScore(std::make_pair(mw, cs), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwMc.getOrUpdateScore(std::make_pair(hw, cs), m_nScoreIndex, amount, round);
   nReturn += cast_weights->m_mapHwMw.getOrUpdateScore(std::make_pair(hwmw, cs), m_nScoreIndex, amount, round);

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateBigramScore - get or update score for bigram con
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateBigramScore( const unsigned long &parent, const unsigned long &child, const unsigned long &child1, const bool &left, SCORE_TYPE amount, int round ) {

   static SCORE_TYPE nReturn;
   nReturn = 0;
return 0;
   static CCFGSet cs; cs.clear();
   cs += parent; cs += child; cs += child1;

   if (left) nReturn += cast_weights->m_mapBigramsL.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   else nReturn += cast_weights->m_mapBigramsR.getOrUpdateScore(cs, m_nScoreIndex, amount, round);
   
   return nReturn;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateGraphScore - get or update the score from a single constituent link
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateGraphScore( const CStateItem *item, SCORE_TYPE amount, int round ) {
   return 0;
   assert(item->stack.size()>0);
   assert(item->nodes[item->stack.back()].is_constituent());
   const CStateNode &nd = item->nodes[item->stack.back()];
   if (nd.temp) return 0; // no constituent score for temp nodes

   static unsigned long i, j, k;
   static CCFGSet cf;
   static CCFGSet ct;
   cf.clear();
   ct.clear();

   const CContext *ctxt = item->context;

   assert( ctxt->s0_unbinarized.size() == ctxt->s0_unbinarized_cs.size() );
   assert( ctxt->s1_unbinarized.size() == ctxt->s1_unbinarized_cs.size() );
   const unsigned long &s0_size = ctxt->s0_unbinarized.size();
   const unsigned long &s1_size = ctxt->s1_unbinarized.size();

   if (!nd.single_child()) {
      assert( (!nd.head_left()||s0_size==1) && (nd.head_left()||s1_size==1) );
      assert( nd.left_child==ctxt->s1 && nd.right_child==ctxt->s0);
   }
   else {
      assert( nd.left_child==ctxt->s0 );
   }

   const CWord &w = m_lCache[nd.lexical_head]; // lexical head
   const CTag &t = m_lCache[nd.lexical_head].tag; // head tag

   static unsigned long n; // number of constituents in the rule
   n = nd.single_child()?s0_size:s1_size+s0_size;

   static SCORE_TYPE nReturn;

   nReturn = 0;

   //const int bracket_start = getStartingBracket(m_lCache[item->nodes[nd.single_child()?ctxt->s0_unbinarized.front():ctxt->s1_unbinarized.front()].lexical_head]);
   //const int bracket_end = getEndingBracket(m_lCache[item->nodes[ctxt->s0_unbinarized.back()].lexical_head]);
   //const unsigned long bracket = (bracket_start!=-1||bracket_end!=-1) ? (bracket_start==bracket_end) ? 1 : 2 : 0;
   //if (bracket != 0) 
   //   nReturn += cast_weights->m_mapBracket.getOrUpdateScore(encodeTorCs(bracket, nd.constituent), m_nScoreIndex, amount, round);

   // Rules
   //if (n<6) {
   //   ct = cf;
   //   j = 0;
   //   if (!nd.single_child()) {
   //      for (i=0; i<s1_size; ++i) {
   //         ct += ctxt->s1_unbinarized_cs[i];
   //      }
   //   }
   //   for (i=0; i<s0_size; ++i) {
   //      ct += ctxt->s0_unbinarized_cs[i];
   //   }
   //   nReturn += cast_weights->m_mapRules.getOrUpdateScore(ct, m_nScoreIndex, amount, round);
      //if (nd.norv) nReturn += cast_weights->m_mapRulesWithSizes.getOrUpdateScore(std::make_pair(ct, j), m_nScoreIndex, amount, round);
   //}

   return nReturn;
}

/*---------------------------------------------------------------
 * 
 * getOrUpdateStackScore - manipulate the score from stack 
 *
 *---------------------------------------------------------------*/

inline SCORE_TYPE CConParser::getOrUpdateStackScore( const CStateItem *item, const CAction &action, SCORE_TYPE amount , int round ) {

   const CContext *ctxt = item->context;
   if (ctxt->stacksize==0) return 0;

   static unsigned long j;
   
   static CCFGSet s0ts1tbt;
   s0ts1tbt.copy(ctxt->s0ts1tbt);

#ifdef _CHINESE_CFG_H
//   static unsigned long s0c_bracket_action;
//   static unsigned long s1c_bracket_action;
//   static unsigned long n0t_bracket_action;
//   static unsigned long s0cs1c_bracket_action;
//   static unsigned long s0cn0t_bracket_action;
#endif

//   static unsigned long s0cs1c_distaction;

#ifdef _CHINESE_CFG_H
//   s0c_bracket_action = encodeAction(action, ctxt->s0c_bracket);
//   s1c_bracket_action = encodeAction(action, ctxt->s1c_bracket);
//   n0t_bracket_action = encodeAction(action, ctxt->n0t_bracket);
//   s0cs1c_bracket_action = encodeAction(action, ctxt->s0cs1c_bracket);
//   s0cn0t_bracket_action = encodeAction(action, ctxt->s0cn0t_bracket);
#endif

//   s0cs1c_distaction = encodeAction(action, ctxt->s0cs1c_dist);

   static CTuple3<CWord, CConstituent, CAction> word_constituent_action; 
   static CTuple3<CTag, CConstituent, CAction> tag_constituent_action;
   static CTuple3<CTwoWords, CCFGSet, CAction> twoword_cfgset_action;
   static CTuple3<CTwoWords, CTag, CAction> twoword_tag_action;
//   static CTuple3<CWord, CTag, CAction> word_tag_action;
   static CTuple3<CWord, CCFGSet, CAction> word_cfgset_action;
   static CTuple3<CTwoWords, CTag, CActionType> twoword_tag_actiontype;
   static CTuple3<CWord, CCFGSet, CActionType> word_cfgset_actiontype;

   static CActionType actionType;
   actionType.code = action.type();

   static SCORE_TYPE nReturn;

   nReturn = 0;
//
   // action type features
//   if (ctxt->s1!=-1) {
//      nReturn += cast_weights->m_mapS0wtS1wt.getOrUpdateScore(std::make_pair(ctxt->s0wts1wt, actionType), m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(twoword_tag_actiontype, &(ctxt->s0ws1w), &(ctxt->s0t), &actionType); 
//      nReturn += cast_weights->m_mapS0wtS1w.getOrUpdateScore(twoword_tag_actiontype, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(twoword_tag_actiontype, &(ctxt->s0ws1w), &(ctxt->s1t), &actionType); 
//      nReturn += cast_weights->m_mapS0wS1wt.getOrUpdateScore(twoword_tag_actiontype, m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1w.getOrUpdateScore(std::make_pair(ctxt->s0ws1w, actionType), m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_actiontype, ctxt->s0w, &(ctxt->s0ts1t), &actionType);
//      nReturn += cast_weights->m_mapS0wtS1t.getOrUpdateScore(word_cfgset_actiontype, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_actiontype, ctxt->s1w, &(ctxt->s0ts1t), &actionType); 
//      nReturn += cast_weights->m_mapS0tS1wt.getOrUpdateScore(word_cfgset_actiontype, m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0tS1t.getOrUpdateScore(std::make_pair(ctxt->s0ts1t, actionType), m_nScoreIndex, amount, round);
//
//      for (j=0; j<ctxt->between_tag.size(); j++) {
//         s0ts1tbt.setLast(ctxt->between_tag[j]);
//         nReturn += cast_weights->m_mapBetweenTags.getOrUpdateScore(std::make_pair(s0ts1tbt, actiontype), m_nScoreIndex, amount, round);
//      }
//   }

   // S0
   nReturn += cast_weights->m_mapS0w.getOrUpdateScore(std::make_pair(*(ctxt->s0wt), action), m_nScoreIndex, amount, round);
   if (!ctxt->s0c.empty()) nReturn += cast_weights->m_mapS0c.getOrUpdateScore(std::make_pair(ctxt->s0c, action), m_nScoreIndex, amount, round);
   refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s0t), &(ctxt->s0c), &action); 
   nReturn += cast_weights->m_mapS0tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
   refer_or_allocate_tuple3(word_constituent_action, ctxt->s0w, &(ctxt->s0c), &action); 
   nReturn += cast_weights->m_mapS0wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);

   // S1
   if (ctxt->s1!=-1) {
      nReturn += cast_weights->m_mapS1w.getOrUpdateScore(std::make_pair(*(ctxt->s1wt), action), m_nScoreIndex, amount, round);
      if (!ctxt->s1c.empty()) nReturn += cast_weights->m_mapS1c.getOrUpdateScore(std::make_pair(ctxt->s1c, action), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s1t), &(ctxt->s1c), &action); 
      nReturn += cast_weights->m_mapS1tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s1w, &(ctxt->s1c), &action); 
      nReturn += cast_weights->m_mapS1wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S2
   if (ctxt->s2!=-1) {
//      nReturn += cast_weights->m_mapS2w.getOrUpdateScore(std::make_pair(*(ctxt->s2w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS2c.getOrUpdateScore(s2c_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s2t), &(ctxt->s2c), &action); 
      nReturn += cast_weights->m_mapS2tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s2w, &(ctxt->s2c), &action); 
      nReturn += cast_weights->m_mapS2wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S3
   if (ctxt->s3!=-1) {
//      nReturn += cast_weights->m_mapS3w.getOrUpdateScore(std::make_pair(*(ctxt->s3w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS3c.getOrUpdateScore(s3c_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s3t), &(ctxt->s3c), &action); 
      nReturn += cast_weights->m_mapS3tc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s3w, &(ctxt->s3c), &action); 
      nReturn += cast_weights->m_mapS3wc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // N0
   if (ctxt->n0!=-1) {
//      nReturn += cast_weights->m_mapN0w.getOrUpdateScore(std::make_pair(*(ctxt->n0w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN0t.getOrUpdateScore(n0t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN0wt.getOrUpdateScore(std::make_pair(*(ctxt->n0wt), action), m_nScoreIndex, amount, round);
   }

   // N1
   if (ctxt->n1!=-1) {
//      nReturn += cast_weights->m_mapN1w.getOrUpdateScore(std::make_pair(*(ctxt->n1w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN1t.getOrUpdateScore(n1t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN1wt.getOrUpdateScore(std::make_pair(*(ctxt->n1wt), action), m_nScoreIndex, amount, round);
   }

   // N2
   if (ctxt->n2!=-1) {
//      nReturn += cast_weights->m_mapN2w.getOrUpdateScore(std::make_pair(*(ctxt->n2w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN2t.getOrUpdateScore(n2t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN2wt.getOrUpdateScore(std::make_pair(*(ctxt->n2wt), action), m_nScoreIndex, amount, round);
   }

   // N3
   if (ctxt->n3!=-1) {
//      nReturn += cast_weights->m_mapN3w.getOrUpdateScore(std::make_pair(*(ctxt->n3w), action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN3t.getOrUpdateScore(n3t_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapN3wt.getOrUpdateScore(std::make_pair(*(ctxt->n3wt), action), m_nScoreIndex, amount, round);
   }

   // S0L
   if (ctxt->s0l!=-1) {
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s0lt), &(ctxt->s0lc), &action); 
      nReturn += cast_weights->m_mapS0Ltc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s0lw, &(ctxt->s0lc), &action); 
      nReturn += cast_weights->m_mapS0Lwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // s0h!=-1 when s0 has any sub nodes including the case of one
   if (ctxt->s0h!=-1) {
      nReturn += cast_weights->m_mapS0cS0HcS0L2c.getOrUpdateScore(std::make_pair(ctxt->s0cs0hcs0l2c, action), m_nScoreIndex, amount, round);;
   }

   // S0R
   if (ctxt->s0r!=-1) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s0rt), &(ctxt->s0rc), &action); 
      nReturn += cast_weights->m_mapS0Rtc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s0rw, &(ctxt->s0rc), &action); 
      nReturn += cast_weights->m_mapS0Rwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

  // s0h==-1 means that s0 has no sub nodes
  if (ctxt->s0h!=-1) {
      nReturn += cast_weights->m_mapS0cS0HcS0R2c.getOrUpdateScore(std::make_pair(ctxt->s0cs0hcs0r2c, action), m_nScoreIndex, amount, round);
   }

   // S0U
   if (ctxt->s0u!=-1) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s0ut), &(ctxt->s0uc), &action); 
      nReturn += cast_weights->m_mapS0Utc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s0uw, &(ctxt->s0uc), &action); 
      nReturn += cast_weights->m_mapS0Uwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }


   // S1L
   if (ctxt->s1l!=-1) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s1lt), &(ctxt->s1lc), &action); 
      nReturn += cast_weights->m_mapS1Ltc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s1lw, &(ctxt->s1lc), &action); 
      nReturn += cast_weights->m_mapS1Lwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S1R
   if (ctxt->s1r!=-1) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s1rt), &(ctxt->s1rc), &action); 
      nReturn += cast_weights->m_mapS1Rtc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s1rw, &(ctxt->s1rc), &action); 
      nReturn += cast_weights->m_mapS1Rwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // s1h==-1 means that there are no sub nodes
   if (ctxt->s1h!=-1) {
      nReturn += cast_weights->m_mapS1cS1HcS1R2c.getOrUpdateScore(std::make_pair(ctxt->s1cs1hcs1r2c, action), m_nScoreIndex, amount, round);
   }

   // S1U
   if (ctxt->s1u!=-1) {         
      refer_or_allocate_tuple3(tag_constituent_action, &(ctxt->s1ut), &(ctxt->s1uc), &action); 
      nReturn += cast_weights->m_mapS1Utc.getOrUpdateScore(tag_constituent_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_constituent_action, ctxt->s1uw, &(ctxt->s1uc), &action); 
      nReturn += cast_weights->m_mapS1Uwc.getOrUpdateScore(word_constituent_action, m_nScoreIndex, amount, round);
   }

   // S0LD
   if (ctxt->s0ld!=-1) {
//      nReturn += cast_weights->m_mapS0LDw.getOrUpdateScore(std::make_pair(*(ctxt->s0ldw), encodeAction(action, ctxt->s0ldt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0LDt.getOrUpdateScore(encodeAction(action, ctxt->s0ldt), m_nScoreIndex, amount, round);
   }

   // S0RD
   if (ctxt->s0rd!=-1) {
//      nReturn += cast_weights->m_mapS0RDw.getOrUpdateScore(std::make_pair(*(ctxt->s0rdw), encodeAction(action, ctxt->s0rdt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0RDt.getOrUpdateScore(encodeAction(action, ctxt->s0rdt), m_nScoreIndex, amount, round);
   }

   // S1LD
   if (ctxt->s1ld!=-1) {         
//      nReturn += cast_weights->m_mapS1LDw.getOrUpdateScore(std::make_pair(*(ctxt->s1ldw), encodeAction(action, ctxt->s1ldt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1LDt.getOrUpdateScore(encodeAction(action, ctxt->s1ldt), m_nScoreIndex, amount, round);
   }

   // S1RD
   if (ctxt->s1rd!=-1) {         
//      nReturn += cast_weights->m_mapS1RDw.getOrUpdateScore(std::make_pair(*(ctxt->s1rdw), encodeAction(action, ctxt->s1rdt)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS1RDt.getOrUpdateScore(encodeAction(action, ctxt->s1rdt), m_nScoreIndex, amount, round);
   }

   // S0 S1
   if (ctxt->s1!=-1) {
      refer_or_allocate_tuple3(twoword_cfgset_action, &(ctxt->s0ws1w), &(ctxt->s0cs1c), &action); 
      nReturn += cast_weights->m_mapS0wcS1wc.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s1w, &(ctxt->s0cs1c), &action); 
      nReturn += cast_weights->m_mapS0cS1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s0w, &(ctxt->s0cs1c), &action); 
      nReturn += cast_weights->m_mapS0wS1c.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1c.getOrUpdateScore(std::make_pair(ctxt->s0cs1c, action), m_nScoreIndex, amount, round);

   }

   // S0 N0
   if (ctxt->n0!=-1) {
      refer_or_allocate_tuple3(twoword_cfgset_action, &(ctxt->s0wn0w), &(ctxt->s0cn0t), &action); 
      nReturn += cast_weights->m_mapS0wN0w.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->n0w, &(ctxt->s0cn0t), &action); 
      nReturn += cast_weights->m_mapS0cN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s0w, &(ctxt->s0cn0t), &action); 
      nReturn += cast_weights->m_mapS0wN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cN0t.getOrUpdateScore(std::make_pair(ctxt->s0cn0t, action), m_nScoreIndex, amount, round);
      //if (item->nodes[ctxt->s0].norv&&ctxt->n0norv) nReturn += cast_weights->m_mapS0cmN0tm.getOrUpdateScore(std::make_pair(s0cn0t_action, encodeRhythms(ctxt->s0c, ctxt->n0m)), m_nScoreIndex, amount, round);
   }

   // S1 N0
   if (ctxt->s1!=-1 && ctxt->n0!=-1) {
      refer_or_allocate_tuple3(twoword_cfgset_action, &(ctxt->s1wn0w), &(ctxt->s1cn0t), &action); 
      nReturn += cast_weights->m_mapS1wN0w.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->n0w, &(ctxt->s1cn0t), &action); 
      nReturn += cast_weights->m_mapS1cN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s1w, &(ctxt->s1cn0t), &action); 
      nReturn += cast_weights->m_mapS1wN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1cN0t.getOrUpdateScore(std::make_pair(ctxt->s1cn0t, action), m_nScoreIndex, amount, round);
   }

   // N0 N1
//   if (ctxt->n1!=-1) {
//      refer_or_allocate_tuple3(twoword_cfgset_action, &(ctxt->n0wn1w), &(ctxt->n0tn1t), &action); 
//      nReturn += cast_weights->m_mapN0wN1w.getOrUpdateScore(twoword_cfgset_action, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_action, ctxt->n1w, &(ctxt->n0tn1t), &action); 
//      nReturn += cast_weights->m_mapN0tN1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
//      refer_or_allocate_tuple3(word_cfgset_action, ctxt->n0w, &(ctxt->n0tn1t), &action); 
//      nReturn += cast_weights->m_mapN0wN1t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapN0tN1t.getOrUpdateScore(std::make_pair(ctxt->n0tn1t, action), m_nScoreIndex, amount, round);
//   }
//   
#ifdef _CHINESE_CFG_H
/*
   if (ctxt->open_bracket_match_type!=0) {
if (!ctxt->s0c.empty() && !ctxt->s1c.empty()) {
      nReturn += cast_weights->m_mapBracketS0c.getOrUpdateScore(s0c_bracket_action, m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapBracketS0w.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, ctxt->open_bracket_match_type)), m_nScoreIndex, amount, round);
      if (ctxt->s1!=-1) {
         nReturn += cast_weights->m_mapBracketS0wS1c.getOrUpdateScore(std::make_pair(*(ctxt->s0w), s1c_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cS1w.getOrUpdateScore(std::make_pair(*(ctxt->s1w), s0c_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cS1c.getOrUpdateScore(s0cs1c_bracket_action, m_nScoreIndex, amount, round);
      }
      if (ctxt->n0!=-1) {
         nReturn += cast_weights->m_mapBracketS0cN0w.getOrUpdateScore(std::make_pair(*(ctxt->n0w), s0c_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0wN0t.getOrUpdateScore(std::make_pair(*(ctxt->s0w), n0t_bracket_action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapBracketS0cN0t.getOrUpdateScore(s0cn0t_bracket_action, m_nScoreIndex, amount, round);
      }
}
   }

   if (ctxt->s1!=-1) {
if (!ctxt->s0c.empty() && !ctxt->s1c.empty()) {
      static unsigned long i;
      for (i=0; i<ctxt->s0c_separator.size(); ++i) {
         nReturn += cast_weights->m_mapS0cSeparator.getOrUpdateScore(encodeAction(action, ctxt->s0c_separator[i]), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapS0wcSeparator.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, ctxt->s0c_separator[i])), m_nScoreIndex, amount, round);
      }
      nReturn += cast_weights->m_mapS0cSepCount.getOrUpdateScore(encodeAction(action, ctxt->s0c_sepcount), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0wcSepCount.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, ctxt->s0c_sepcount)), m_nScoreIndex, amount, round);
      for (i=0; i<ctxt->s1c_separator.size(); ++i) {
         nReturn += cast_weights->m_mapS1cSeparator.getOrUpdateScore(encodeAction(action, ctxt->s1c_separator[i]), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapS1wcSeparator.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, ctxt->s1c_separator[i])), m_nScoreIndex, amount, round);
      }
      nReturn += cast_weights->m_mapS1cSepCount.getOrUpdateScore(encodeAction(action, ctxt->s1c_sepcount), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS1wcSepCount.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, ctxt->s1c_sepcount)), m_nScoreIndex, amount, round);
      for (i=0; i<ctxt->s0cs1c_separator.size(); ++i)
         nReturn += cast_weights->m_mapS0cS1cSeparator.getOrUpdateScore(encodeAction(action, ctxt->s0cs1c_separator[i]), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS1cSepCount.getOrUpdateScore(encodeAction(action, ctxt->s0cs1c_sepcount), m_nScoreIndex, amount, round);
}
   }
*/



#endif

   if (ctxt->s1!=-1){
//      nReturn += cast_weights->m_mapS0wS1Dist.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, ctxt->s0s1_dist)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1Dist.getOrUpdateScore(encodeAction(action, ctxt->s0cs1_dist), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0S1wDist.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, ctxt->s0s1_dist)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0S1cDist.getOrUpdateScore(encodeAction(action, ctxt->s0s1c_dist), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1wDist.getOrUpdateScore(std::make_pair(ctxt->s0ws1w, s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1cDist.getOrUpdateScore(std::make_pair(*(ctxt->s0w), s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wDist.getOrUpdateScore(std::make_pair(*(ctxt->s1w), s0cs1c_distaction), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1cDist.getOrUpdateScore(s0cs1c_distaction, m_nScoreIndex, amount, round);
   }

   // S0 S1 N0
   refer_or_allocate_tuple3(word_cfgset_action, ctxt->s0w, &(ctxt->s0cs1cn0t), &action); 
   nReturn += cast_weights->m_mapS0wS1cN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   if (ctxt->s1!=-1)  {
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s1w, &(ctxt->s0cs1cn0t), &action); 
      nReturn += cast_weights->m_mapS0cS1wN0t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
   if (ctxt->n0!=-1) {
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->n0w, &(ctxt->s0cs1cn0t), &action); 
      nReturn += cast_weights->m_mapS0cS1cN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
   nReturn += cast_weights->m_mapS0cS1cN0t.getOrUpdateScore(std::make_pair(ctxt->s0cs1cn0t, action), m_nScoreIndex, amount, round); 

   nReturn += cast_weights->m_mapS0tS1tN0t.getOrUpdateScore(std::make_pair(ctxt->s0ts1tn0t, action), m_nScoreIndex, amount, round); 
//   nReturn += cast_weights->m_mapS0jS1jN0t.getOrUpdateScore(std::make_pair(ctxt->s0js1jn0t, action), m_nScoreIndex, amount, round); 

   // S0 N0 N1
   if (ctxt->n0!=-1) {
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s0w, &(ctxt->s0cn0tn1t), &action); 
      nReturn += cast_weights->m_mapS0wN0tN1t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->n0w, &(ctxt->s0cn0tn1t), &action); 
      nReturn += cast_weights->m_mapS0cN0wN1t.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      if (ctxt->n1!=-1) {
         refer_or_allocate_tuple3(word_cfgset_action, ctxt->n1w, &(ctxt->s0cn0tn1t), &action); 
         nReturn += cast_weights->m_mapS0cN0tN1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      }
      nReturn += cast_weights->m_mapS0cN0tN1t.getOrUpdateScore(std::make_pair(ctxt->s0cn0tn1t, action), m_nScoreIndex, amount, round); // ctxt->n0

      nReturn += cast_weights->m_mapS0tN0tN1t.getOrUpdateScore(std::make_pair(ctxt->s0tn0tn1t, action), m_nScoreIndex, amount, round); // ctxt->n0
//      nReturn += cast_weights->m_mapS0jN0tN1t.getOrUpdateScore(std::make_pair(ctxt->s0jn0tn1t, action), m_nScoreIndex, amount, round); // ctxt->n0
   }

   // S0 S1 S2
   if (ctxt->s1!=-1) {
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s0w, &(ctxt->s0cs1cs2c), &action); 
      nReturn += cast_weights->m_mapS0wS1cS2c.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s1w, &(ctxt->s0cs1cs2c), &action); 
      nReturn += cast_weights->m_mapS0cS1wS2c.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      if (ctxt->s2!=-1) {
         refer_or_allocate_tuple3(word_cfgset_action, ctxt->s2w, &(ctxt->s0cs1cs2c), &action); 
         nReturn += cast_weights->m_mapS0cS1cS2w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      }
      nReturn += cast_weights->m_mapS0cS1cS2c.getOrUpdateScore(std::make_pair(ctxt->s0cs1cs2c, action), m_nScoreIndex, amount, round);

      nReturn += cast_weights->m_mapS0tS1tS2t.getOrUpdateScore(std::make_pair(ctxt->s0ts1ts2t, action), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0jS1jS2j.getOrUpdateScore(std::make_pair(ctxt->s0js1js2j, action), m_nScoreIndex, amount, round);
   }

   // Tag bigrams
//   nReturn += cast_weights->m_mapS1cS2c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s2c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS2cS3c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s2c, ctxt->s3c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN1tN2t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n1t, ctxt->n2t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN2tN3t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n2t, ctxt->n3t)), m_nScoreIndex, amount, round);
   
//   nReturn += cast_weights->m_mapS0cS2c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s2c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS1cS3c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s3c)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapS0cN1t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->n1t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN0tN2t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n0t, ctxt->n2t)), m_nScoreIndex, amount, round);
//   nReturn += cast_weights->m_mapN1tN3t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n1t, ctxt->n3t)), m_nScoreIndex, amount, round);

   // S0 S0LRUN0
//   if (ctxt->s0l!=-1) {
//      assert(ctxt->s0r!=-1);
//      if (!item->nodes[ctxt->s0].temp||!item->nodes[ctxt->s0].head_left()) {
//      nReturn += cast_weights->m_mapS0cS0LcN0t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cn0t, ctxt->s0lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0LcN0t.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->n0t, ctxt->s0lc))), m_nScoreIndex, amount, round);
//      if (ctxt->n0!=-1) nReturn += cast_weights->m_mapS0cS0LcN0w.getOrUpdateScore(std::make_pair(*(ctxt->n0w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0lc))), m_nScoreIndex, amount, round);
//      }
//      if (item->nodes[ctxt->s0].head_left()) {
      if (ctxt->n0!=-1 && ctxt->s0r!=-1) {
         nReturn += cast_weights->m_mapS0cS0RcN0t.getOrUpdateScore(std::make_pair(ctxt->s0cs0rcn0t, action), m_nScoreIndex, amount, round);
         nReturn += cast_weights->m_mapS0cS0RjN0t.getOrUpdateScore(std::make_pair(ctxt->s0cs0rjn0t, action), m_nScoreIndex, amount, round);
         refer_or_allocate_tuple3(word_cfgset_action, ctxt->n0w, &(ctxt->s0cs0rc), &action); 
         nReturn += cast_weights->m_mapS0cS0RcN0w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
      }
//      nReturn += cast_weights->m_mapS0wS0RcN0t.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->n0t, ctxt->s0rc))), m_nScoreIndex, amount, round);
//      }
//   }
//   if (ctxt->s0u!=-1) {
//      assert(ctxt->s0l==-1);
//      assert(item->nodes[ctxt->s0].temp==false);
//      nReturn += cast_weights->m_mapS0cS0UcN0t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cn0t, ctxt->s0uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0UcN0t.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0uc, ctxt->n0t))), m_nScoreIndex, amount, round);
//      if (ctxt->n0!=-1) nReturn += cast_weights->m_mapS0cS0UcN0w.getOrUpdateScore(std::make_pair(*(ctxt->n0w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0uc))), m_nScoreIndex, amount, round);
//   }

   // S0 S0LRUS1
   if (ctxt->s1!=-1 && ctxt->s0l!=-1) {
      nReturn += cast_weights->m_mapS0cS0LcS1c.getOrUpdateScore(std::make_pair(ctxt->s0cs0lcs1c, action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0cS0LjS1j.getOrUpdateScore(std::make_pair(ctxt->s0cs0ljs1j, action), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s1w, &(ctxt->s0cs0lc), &action); 
      nReturn += cast_weights->m_mapS0cS0LcS1w.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
   if (ctxt->s1 != -1 && ctxt->s1r != -1) {
      nReturn += cast_weights->m_mapS0cS1cS1Rc.getOrUpdateScore(std::make_pair(ctxt->s0cs1cs1rc, action), m_nScoreIndex, amount, round);
      nReturn += cast_weights->m_mapS0jS1cS1Rj.getOrUpdateScore(std::make_pair(ctxt->s0js1cs1rj, action), m_nScoreIndex, amount, round);
      refer_or_allocate_tuple3(word_cfgset_action, ctxt->s0w, &(ctxt->s1cs1rc), &action); 
      nReturn += cast_weights->m_mapS0wS1cS1Rc.getOrUpdateScore(word_cfgset_action, m_nScoreIndex, amount, round);
   }
//   if (ctxt->s0l!=-1) {
//      if (!item->nodes[ctxt->s0].temp||!item->nodes[ctxt->s0].head_left()) {
//      nReturn += cast_weights->m_mapS0cS0LcS1c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s0lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0LcS1c.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0lc, ctxt->s1c))), m_nScoreIndex, amount, round);
//      }
//      if (!item->nodes[ctxt->s0].temp||item->nodes[ctxt->s0].head_left()) {
//         nReturn += cast_weights->m_mapS0cS0RcS1c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s0rc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0RcS1c.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0rc, ctxt->s1c))), m_nScoreIndex, amount, round);
//      if (ctxt->s1!=-1) nReturn += cast_weights->m_mapS0cS0RcS1w.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0rc))), m_nScoreIndex, amount, round);
//      }
//   }
//   if (ctxt->s0u!=-1) {
//      nReturn += cast_weights->m_mapS0cS0UcS1c.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s0uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS0UcS1c.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s0uc, ctxt->s1c))), m_nScoreIndex, amount, round);
//      if (ctxt->s1!=-1) nReturn += cast_weights->m_mapS0cS0UcS1w.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s0uc))), m_nScoreIndex, amount, round);
//   }

   // Tag trigram
   //if (ctxt->n0!=-1) {
   //   if (ctxt->n1!=-1) nReturn += cast_weights->m_mapN0tN1tN2t.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->n0tn1t, ctxt->n2t)), m_nScoreIndex, amount, round);
   //}
//   if (ctxt->s1l!=-1) {
//      assert(ctxt->s1r!=-1);
//      assert(!item->nodes[ctxt->s1].temp||item->nodes[ctxt->s1].head_left());
//      if (!item->nodes[ctxt->s1].temp) {
//         nReturn += cast_weights->m_mapS0cS1cS1Lc.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s1lc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1cS1Lc.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s1lc))), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wS1Lc.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s1lc))), m_nScoreIndex, amount, round);
//      }
//      nReturn += cast_weights->m_mapS0cS1cS1Rc.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s1rc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wS1Rc.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s1rc))), m_nScoreIndex, amount, round);
//   }
//   if (ctxt->s1u!=-1) {
//      nReturn += cast_weights->m_mapS0cS1cS1Uc.getOrUpdateScore(encodeAction(action, encodeTorCs(ctxt->s0cs1c, ctxt->s1uc)), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0wS1cS1Uc.getOrUpdateScore(std::make_pair(*(ctxt->s0w), encodeAction(action, encodeTorCs(ctxt->s1c, ctxt->s1uc))), m_nScoreIndex, amount, round);
//      nReturn += cast_weights->m_mapS0cS1wS1Uc.getOrUpdateScore(std::make_pair(*(ctxt->s1w), encodeAction(action, encodeTorCs(ctxt->s0c, ctxt->s1uc))), m_nScoreIndex, amount, round);
//   }
   return nReturn;
}

/*---------------------------------------------------------------
 *
 * getGlobalScore - get the score of a parse tree
 *
 * Inputs: parse graph
 *
 *---------------------------------------------------------------*/

SCORE_TYPE CConParser::getGlobalScore(const CSentenceParsed &parsed) {
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

void CConParser::updateScores(const CSentenceParsed & parsed , const CSentenceParsed & correct , int round ) {
   
   assert( m_bTrain );

   if ( round > m_nTrainingRound )
      m_nTrainingRound = round ;

   if ( parsed == correct ) 
      return;


    m_nTotalErrors++;

}

/*---------------------------------------------------------------
 *
 * updateScoresForState - update score
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForState( const CStateItem *item , const SCORE_UPDATE update ) {

   static CStateItem st;
   static CAction action;
   st.clear();
   st.sent = &m_lCache;
   static CContext context;
   st.context = &context;
   static unsigned long constituent;
   static bool single_child, head_left, temporary;
   static unsigned long stacksize;

   const SCORE_TYPE amount = (update==eAdd ? 1 : -1);

   while (st != *item)  {
      // make sure 
      context.load(&st, m_lCache, m_lWordLen, true);
      stacksize = st.stack.size();
      if (stacksize>0) assert(st.stack.back()==context.s0);
      if (stacksize>1) assert(st.stack[stacksize-2]==context.s1);
      // update action
      st.FollowMove(*item, action);
      getOrUpdateStackScore(&st, action, amount, m_nTrainingRound );
      // make action
      st.Move(action);
      // update graph
      if ( !action.isShift() && !action.isReduceRoot() ) {
         // reduce
         getOrUpdateGraphScore(&st, amount, m_nTrainingRound);
         single_child = action.singleChild();
         head_left = action.headLeft();
         temporary = action.isTemporary();
         constituent = action.getConstituent();
         if ( !single_child ) {
            const CStateNode &left = st.nodes[context.s1];
            const CStateNode &right = st.nodes[context.s0];
            assert(!left.temp||!right.temp);
#ifdef NO_TEMP_CONSTITUENT
            assert(!left.temp||(head_left&&constituent==left.constituent.code()));
            assert(!right.temp||(!head_left&&constituent==right.constituent.code()));
#else
            assert(!left.temp||(head_left&&CConstituent::clearTmp(constituent)==left.constituent.extractConstituentCode()));
            assert(!right.temp||(!head_left&&CConstituent::clearTmp(constituent)==right.constituent.extractConstituentCode()));
#endif
            if (head_left) {
//               getOrUpdateLinkScore( left.lexical_head, 
//                                     right.lexical_head, 
//                                     amount, m_nTrainingRound );
//               if (context.s1rd!=-1) getOrUpdateSiblingLinkScore( left.lexical_head, 
//                                                                  right.lexical_head, 
//                                                                  st.nodes[st.context->s1rd].lexical_head, 
//                                                                  amount, m_nTrainingRound );
//               getOrUpdateArityScore( right.lexical_head, 
//                                      context.s0ln, 
//                                      context.s0rn, 
//                                      amount, m_nTrainingRound);
//               if (!left.temp) getOrUpdateHeadScore( constituent, 
//                                                     context.s1c, 
//                                                     left.lexical_head, 
//                                                     amount, m_nTrainingRound );
//               getOrUpdateHeadModifierScore( constituent, 
//                                             context.s1_unbinarized_cs[context.s1_head_index], 
//                                             context.s0c, 
//                                             left.lexical_head, right.lexical_head,
//                                             true, 
//                                             neighbour, 
//                                             amount, m_nTrainingRound );
//               if (!neighbour) getOrUpdateBigramScore( constituent, 
//                                                       context.s1_unbinarized_cs.back(), 
//                                                       context.s0c, 
//                                                       true, 
//                                                       amount, m_nTrainingRound );
            }
            else {
//               getOrUpdateLinkScore( right.lexical_head, 
//                                     left.lexical_head, 
//                                     amount, 
//                                     m_nTrainingRound );
//               if (context.s0ld!=-1) getOrUpdateSiblingLinkScore( right.lexical_head, 
//                                                                  left.lexical_head, 
//                                                                  st.nodes[st.context->s0ld].lexical_head, 
//                                                                  amount, m_nTrainingRound );
//               getOrUpdateArityScore( left.lexical_head, 
//                                      context.s1ln, 
//                                      context.s1rn, 
//                                      amount, m_nTrainingRound);
//               if (!right.temp)getOrUpdateHeadScore( constituent, 
//                                                     context.s0c, 
//                                                     right.lexical_head, 
//                                                     amount, m_nTrainingRound );
//               getOrUpdateHeadModifierScore( constituent, 
//                                             context.s0_unbinarized_cs[context.s0_head_index], 
//                                             context.s1c, 
//                                             right.lexical_head, left.lexical_head,
//                                             false, 
//                                             neighbour, 
//                                             amount, m_nTrainingRound );
//               if (!neighbour) getOrUpdateBigramScore( constituent, 
//                                                       context.s0_unbinarized_cs[0], 
//                                                       context.s1c, 
//                                                       false, 
//                                                       amount, m_nTrainingRound);
            }
         }
         else {
            const CStateNode &child = st.nodes[context.s0]; 
//            getOrUpdateHeadScore( constituent, 
//                                  context.s0c, 
//                                  child.lexical_head, 
//                                  amount, m_nTrainingRound);
         }
      }
      if ( action.isReduceRoot() ) {
         // finish
         const CStateNode &root = st.nodes[context.s0];
//         getOrUpdateArityScore( root.lexical_head, 
//                                context.s0ln, 
//                                context.s0rn, 
//                                amount, m_nTrainingRound);
      }
   }
}

/*---------------------------------------------------------------
 *
 * updateScoresForStates - update scores for states
 *
 *--------------------------------------------------------------*/

void CConParser::updateScoresForStates( const CStateItem *outout , const CStateItem *correct ) {

   std::cout << "updating parameters ... " ; 

   updateScoresForState( outout, eSubtract );
   updateScoresForState( correct, eAdd );

   m_nTotalErrors++;
}

/*---------------------------------------------------------------
 *
 * getOrUpdateScore - get or update the score of a state and act
 *
 *--------------------------------------------------------------*/

SCORE_TYPE CConParser::getOrUpdateScore( const conparser::CStateItem &item, const conparser::CAction &action, conparser::SCORE_TYPE amount, int round ) {
   return getOrUpdateStackScore(&item, action, amount, round);
}

/*---------------------------------------------------------------
 *
 * work - the working process shared by training and parsing
 *
 * Returns: makes a new instance of CSentenceParsed 
 *
 *--------------------------------------------------------------*/

void CConParser::work( const bool bTrain , const CTwoStringVector &sentence , CSentenceParsed *retval , const CSentenceParsed &correct , int nBest , SCORE_TYPE *scores ) {

#ifdef DEBUG
   clock_t total_start_time = clock();
#endif
   const int length = sentence.size() ; 

   const static CStateItem *pGenerator ;
   static CStateItem oCandidate ;
   static bool bCorrect ;  // used in learning for early update
   static CStateItem correctState ;
   static unsigned long stack_size;
   static int tmp_i, tmp_j;
   const static CStateItem *pBestGen;
   static CContext context;
   static bool bParsingDone;
   static CAction action;
   static CAgendaSimple<CScoredAction> beam(AGENDA_SIZE);
   static std::vector<CAction> actions; // actions to apply for a candidate
   static CScoredAction scored_action; // used rank actions
   ASSERT(nBest=1, "currently only do 1 best parse");
   // TODO: it is easy to extend this into N-best; just use a std::vector for candidate_outout. during train maybe use the best to adjust
   static CStateItem candidate_outout;

   assert(length<MAX_SENTENCE_SIZE);

   TRACE("Initialising the decoding process ... ") ;
   // initialise word cache
   m_lCache.clear();
   m_lWordLen.clear();
   candidate_outout.clear();
   for ( tmp_i=0; tmp_i<length; tmp_i++ ) {
      m_lCache.push_back( CTaggedWord<CTag, TAG_SEPARATOR>(sentence[tmp_i].first , sentence[tmp_i].second) );
      m_lWordLen.push_back( getUTF8StringLength(sentence[tmp_i].first) );
   }
   // initialise agenda
   m_Agenda->clear();
   oCandidate.clear();                             // restore state using clean
   oCandidate.sent = &m_lCache;
   m_Agenda->pushCandidate(&oCandidate);           // and push it back
   m_Agenda->nextRound();                       // as the generator item
   if (bTrain) { 
      correctState.clear();
      correctState.sent = &m_lCache;
   }

   TRACE("Decoding start ... ") ;
   while (true) { // for each step

      if (bTrain) bCorrect = false ; 

      pGenerator = m_Agenda->generatorStart();
      if (pGenerator==0) { // no more generators
         if (!candidate_outout.IsTerminated()) {
             WARNING("Parser failed: cannot find a parse for the sentence.");
             ASSERT(retval, "Internal error: parser failed during training");
             retval->clear();
             return;
         }
         break; // finish
      }
//      pBestGen = m_Agenda->bestGenerator();

//      if (pBestGen->IsTerminated()) break; // if the first item is complete
//      bParsingDone = false;
//      for (tmp_i=0; tmp_i<m_Agenda->generatorSize(); ++tmp_i) {
//         if (m_Agenda->generator(tmp_i)->score == pBestGen->score && 
//               m_Agenda->generator(tmp_i)->IsTerminated()) {
//             pBestGen = m_Agenda->generator(tmp_i);
//             bParsingDone = true; break;
//         }
//      }
//      if (bParsingDone) break;
         
      for (tmp_i=0; tmp_i<m_Agenda->generatorSize(); ++tmp_i) { // for each generator

         // when an item is terminated, move it to potential outouts.
         if (pGenerator->IsTerminated()) { 
            //m_Agenda->pushCandidate(pGenerator); // push it back intacit
            if (candidate_outout.empty() || pGenerator->score > candidate_outout.score )
              candidate_outout = *pGenerator;
         }
         else {
            // load context
            context.load(pGenerator, m_lCache, m_lWordLen, false);
            oCandidate = *pGenerator;
            oCandidate.context = &context;
   
            // get actions
            beam.clear();
            m_rule.getActions(oCandidate, actions);
            for (tmp_j=0; tmp_j<actions.size(); ++tmp_j) {
               scored_action.load(actions[tmp_j], getOrUpdateScore(oCandidate, actions[tmp_j]));
               beam.insertItem(&scored_action);
            }
   
            // insertItems
            for (tmp_j=0; tmp_j<beam.size(); ++tmp_j) {
               static SCORE_TYPE original_score;
               static int original_unary;
               original_score = oCandidate.score;
               original_unary = oCandidate.unary_reduce;
               oCandidate.Move(beam.item(tmp_j)->action);
               oCandidate.score += beam.item(tmp_j)->score;
               m_Agenda->pushCandidate(&oCandidate);
               oCandidate.UnMove(beam.item(tmp_j)->action, original_score, original_unary);
            }
         }

         // compare generator to corr
         if (bTrain && *pGenerator == correctState) { 
            bCorrect = true ;
         }

         // next loop
         pGenerator = m_Agenda->generatorNext() ; 
      } // done iterating generator item

      // update items if correct item jump std::cout of the agenda
      if (bTrain) { 
#ifdef EARLY_UPDATE
         if (!bCorrect && candidate_outout!=correctState) {
            // no generator, nor the candidate outout is error-free.
            pBestGen = (m_Agenda->generatorSize()==0 || m_Agenda->bestGenerator()->score < candidate_outout.score) ? &candidate_outout : m_Agenda->bestGenerator();
            correctState.trace(&sentence);
            pBestGen->trace(&sentence);
            TRACE("Error at the "<<correctState.current_word<<"th word; total is "<<correct.words.size())
            updateScoresForStates(pBestGen, &correctState) ; 
            return ;
         }
#endif
         if (!correctState.IsTerminated()) {
            correctState.StandardMove(correct, action);
            correctState.Move(action);
         }
         assert (correctState.unary_reduce<=UNARY_MOVES) ;
      }  // end of update

      m_Agenda->nextRound(); // move round
   }

   if (bTrain) {
      // make sure that the correct item is stack top finally
      if ( candidate_outout != correctState ) {
         correctState.trace(&sentence);
         candidate_outout.trace(&sentence);
         TRACE("The best item is not the correct one")
         updateScoresForStates(&candidate_outout, &correctState) ; 
         return ;
      }
   } 

   if (!retval) 
      return;

   TRACE("Outputing sentence");
   candidate_outout.GenerateTree( sentence, retval[0] );
   if (scores) scores[0] = candidate_outout.score;

   TRACE("Done, the highest score is: " << candidate_outout.score ) ;
   TRACE("The total time spent: " << double(clock() - total_start_time)/CLOCKS_PER_SEC) ;
}

/*---------------------------------------------------------------
 *
 * parse - do constituent parsing to a sentence
 *
 * Returns: makes a new instance of CSentenceParsed 
 *
 *--------------------------------------------------------------*/

void CConParser::parse( const CTwoStringVector &sentence , CSentenceParsed *retval , int nBest , SCORE_TYPE *scores ) {

   static CSentenceParsed empty ;

   work(false, sentence, retval, empty, nBest, scores ) ;

}

/*---------------------------------------------------------------
 *
 * parse - do constituent parsing to a sentence
 *
 * Returns: makes a new instance of CSentenceParsed 
 *
 *--------------------------------------------------------------*/

void CConParser::parse( const CSentenceMultiCon<CConstituent> &sentence , CSentenceParsed *retval , int nBest , SCORE_TYPE *scores ) {

   static CSentenceParsed empty ;

   m_rule.SetLexConstituents( sentence.constituents );
   work(false, sentence.words, retval, empty, nBest, scores ) ;
   m_rule.UnsetLexConstituents();

}

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CConParser::train( const CSentenceParsed &correct , int round ) {

   static CTwoStringVector sentence ;
//   static CSentenceParsed outout ; 

   UnparseSentence( &correct, &sentence ) ;

   // The following code does update for each processing stage
   m_nTrainingRound = round ;
//   work( true , sentence , &outout , correct , 1 , 0 ) ; 
   work( true , sentence , 0 , correct , 1 , 0 ) ; 

};

/*---------------------------------------------------------------
 *
 * train - train the models with an example
 *
 *---------------------------------------------------------------*/

void CConParser::train( const CSentenceMultiCon<CConstituent> &con_input, const CSentenceParsed &correct , int round ) {

   // The following code does update for each processing stage
   m_nTrainingRound = round ;

   m_rule.SetLexConstituents( con_input.constituents );
   work( true , con_input.words , 0 , correct , 1 , 0 ) ; 
   m_rule.UnsetLexConstituents();

};

