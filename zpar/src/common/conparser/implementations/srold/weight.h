// Copyright (C) University of Oxford 2010
/***************************************************************
 *
 * weight.h - the constituent parser weights 
 *
 * Yue Zhang, 2007.8 - 2008.1
 *
 ***************************************************************/

#ifndef _CONPARSER_WEIGHTS_H
#define _CONPARSER_WEIGHTS_H

#include "weight_base.h"

//#define TABLE_SIZE 122651
#define TABLE_SIZE 131072

namespace TARGET_LANGUAGE {

namespace conparser {

//#include "conparser_macros.h"

//
// TYPE DEFINITIONS
//
typedef CScoreMap<CWord, SCORE_TYPE> CWordMap;
typedef CScoreMap<CTaggedWord<CTag, TAG_SEPARATOR>, SCORE_TYPE> CTaggedWordMap;
typedef CScoreMap<std::pair<CWord, unsigned long>,  SCORE_TYPE> CWordIntMap;
typedef CScoreMap<std::pair<CWord, CAction>,  SCORE_TYPE> CWordActionMap;
typedef CScoreMap<std::pair<CTaggedWord<CTag, TAG_SEPARATOR>, CAction>, SCORE_TYPE> CTaggedWordActionMap;
typedef CScoreMap<unsigned long, SCORE_TYPE> CIntMap;
typedef CScoreMap<std::pair<unsigned long, unsigned long>, SCORE_TYPE> CTwoIntMap;
typedef CScoreMap<CTwoWords, SCORE_TYPE> CTwoWordsMap;
typedef CScoreMap<std::pair<CTwoWords, CAction>, SCORE_TYPE> CTwoWordsActionMap;
typedef CScoreMap<std::pair<CTwoWords, CActionType>, SCORE_TYPE> CTwoWordsActionTypeMap;
typedef CScoreMap<CTuple3<CTwoWords, CTag, CAction>, SCORE_TYPE> CTwoWordsTagActionMap;
typedef CScoreMap<CTuple3<CTwoWords, CTag, CActionType>, SCORE_TYPE> CTwoWordsTagActionTypeMap;
typedef CScoreMap<CTuple3<CTwoWords, CCFGSet, CAction>, SCORE_TYPE>
CTwoWordsCFGSetActionMap;
typedef CScoreMap<CTwoTaggedWords, SCORE_TYPE> CTwoTaggedWordsMap;
typedef CScoreMap<std::pair<CTwoTaggedWords, CAction>, SCORE_TYPE> CTwoTaggedWordsActionMap;
typedef CScoreMap<std::pair<CTwoTaggedWords, CActionType>, SCORE_TYPE> CTwoTaggedWordsActionTypeMap;
typedef CScoreMap<CCFGSet, SCORE_TYPE> CCFGSetMap;
typedef CScoreMap<std::pair<CCFGSet, unsigned long>, SCORE_TYPE> CCFGSetIntMap;
typedef CScoreMap<std::pair<CCFGSet, CAction>, SCORE_TYPE> CCFGSetActionMap;
typedef CScoreMap<CTuple3<CWord, CCFGSet, CAction>, SCORE_TYPE> CWordCFGSetActionMap;
typedef CScoreMap<CTuple3<CWord, CCFGSet, CActionType>, SCORE_TYPE> CWordCFGSetActionTypeMap;
typedef CScoreMap<std::pair<CWord, CCFGSet>, SCORE_TYPE> CWordCFGRuleMap;
typedef CScoreMap<std::pair<CTwoWords, CCFGSet>, SCORE_TYPE> CTwoWordsCFGRuleMap;
typedef CScoreMap<std::pair<CCFGSet, unsigned long>, SCORE_TYPE> CCFGSetIntMap;
typedef CScoreMap<std::pair<CCFGSet, CAction>, SCORE_TYPE> CCFGSetActionMap;
typedef CScoreMap<std::pair<CCFGSet, CActionType>, SCORE_TYPE> CCFGSetActionTypeMap;
typedef CScoreMap<CTuple3<CWord, CConstituent, CAction>, SCORE_TYPE>
CWordConstituentActionMap;
typedef CScoreMap<CTuple3<CTag, CConstituent, CAction>, SCORE_TYPE> CTagConstituentActionMap;
typedef CScoreMap<CTuple3<CTag, CTag, unsigned long>, SCORE_TYPE> CWordTagIntMap;
typedef CScoreMap<std::pair<CConstituent, CAction>, SCORE_TYPE> CConstituentActionMap;

typedef CHashMap<CWord, unsigned long> CWordToIntMap;

/*===============================================================
 *
 * CWeight - the definition of weights, in feature
 *
 *==============================================================*/

class CWeight : public CWeightBase {

public:

   // Gr
   CWordIntMap m_mapHw;
   CTwoIntMap m_mapHt;
   CCFGSetMap m_mapHc;
   CWordCFGRuleMap m_mapHwc;

   CCFGSetMap m_mapHcMc;
   CWordCFGRuleMap m_mapHwMc;
   CWordCFGRuleMap m_mapHcMw;
   CTwoWordsCFGRuleMap m_mapHwMw;

   CCFGSetMap m_mapBigramsL;
   CCFGSetMap m_mapBigramsR;
   CWordCFGRuleMap m_mapBigramsLexA;
   CWordCFGRuleMap m_mapBigramsLexB;

   CCFGSetMap m_mapRules;
   CCFGSetIntMap m_mapRulesWithSizes;

   CIntMap m_mapBracket;

   CCFGSetMap m_mapHtMt;
   CWordCFGRuleMap m_mapHtMw;
   CWordCFGRuleMap m_mapHwMt;

   CIntMap m_mapSiblingTags;
   CWordIntMap m_mapSiblingWordTag;
   CWordIntMap m_mapSiblingTagWord;

   CTwoIntMap m_mapHtArityL;
   CTwoIntMap m_mapHtArityR;
   CWordIntMap m_mapHwArityL;
   CWordIntMap m_mapHwArityR;
   
   // S0
   CConstituentActionMap m_mapS0c;
   CTaggedWordActionMap m_mapS0w;
   CTagConstituentActionMap m_mapS0tc;
   CWordConstituentActionMap m_mapS0wc;

   // S1
   CConstituentActionMap m_mapS1c;
   CTaggedWordActionMap m_mapS1w;
   CTagConstituentActionMap m_mapS1tc;
   CWordConstituentActionMap m_mapS1wc;

   // S2
   CTagConstituentActionMap m_mapS2tc;
   CWordConstituentActionMap m_mapS2wc;

   // S3
   CTagConstituentActionMap m_mapS3tc;
   CWordConstituentActionMap m_mapS3wc;

   // N0
   CWordActionMap m_mapN0w;
   CIntMap m_mapN0t;
   CTaggedWordActionMap m_mapN0wt;

   // N1
   CWordActionMap m_mapN1w;
   CIntMap m_mapN1t;
   CTaggedWordActionMap m_mapN1wt;

   // N2
   CWordActionMap m_mapN2w;
   CIntMap m_mapN2t;
   CTaggedWordActionMap m_mapN2wt;

   // N3
   CWordActionMap m_mapN3w;
   CIntMap m_mapN3t;
   CTaggedWordActionMap m_mapN3wt;

   // S0L
   CWordConstituentActionMap m_mapS0Lwc;
   CTagConstituentActionMap m_mapS0Ltc;
   CCFGSetActionMap m_mapS0cS0HcS0L2c;

   // S0R
   CWordConstituentActionMap m_mapS0Rwc;
   CTagConstituentActionMap m_mapS0Rtc;
   CCFGSetActionMap m_mapS0cS0HcS0R2c;

   // S0U
   CWordConstituentActionMap m_mapS0Uwc;
   CTagConstituentActionMap m_mapS0Utc;

   // S1L
   CWordConstituentActionMap m_mapS1Lwc;
   CTagConstituentActionMap m_mapS1Ltc;

   // S1R
   CWordConstituentActionMap m_mapS1Rwc;
   CTagConstituentActionMap m_mapS1Rtc;
   CCFGSetActionMap m_mapS1cS1HcS1R2c;

   // S1U
   CWordConstituentActionMap m_mapS1Uwc;
   CTagConstituentActionMap m_mapS1Utc;

   // S0LD
   CWordActionMap m_mapS0LDw;
   CIntMap m_mapS0LDt;

   // S0RD
   CWordActionMap m_mapS0RDw;
   CIntMap m_mapS0RDt;

   // S1LD
   CWordActionMap m_mapS1LDw;
   CIntMap m_mapS1LDt;

   // S1RD
   CWordActionMap m_mapS1RDw;
   CIntMap m_mapS1RDt;

   // S0 S1
   CTwoWordsCFGSetActionMap m_mapS0wcS1wc; // the original 
   CWordCFGSetActionMap m_mapS0wS1c;
   CWordCFGSetActionMap m_mapS0cS1w;
   CCFGSetActionMap m_mapS0cS1c;

   CTwoTaggedWordsActionTypeMap m_mapS0wtS1wt;
   CTwoWordsTagActionTypeMap m_mapS0wtS1w;
   CTwoWordsTagActionTypeMap m_mapS0wS1wt;
   CTwoWordsActionTypeMap m_mapS0wS1w;
   CWordCFGSetActionTypeMap m_mapS0wtS1t;
   CWordCFGSetActionTypeMap m_mapS0tS1wt;
   CCFGSetActionTypeMap m_mapS0tS1t;

   CCFGSetActionMap m_mapBetweenTags;

   // S0 N0
   CTwoWordsCFGSetActionMap m_mapS0wN0w;
   CWordCFGSetActionMap m_mapS0cN0w;
   CWordCFGSetActionMap m_mapS0wN0t;
   CCFGSetActionMap m_mapS0cN0t;
   CTwoIntMap m_mapS0cmN0tm;

   // S1 N0
   CTwoWordsCFGSetActionMap m_mapS1wN0w;
   CWordCFGSetActionMap m_mapS1cN0w;
   CWordCFGSetActionMap m_mapS1wN0t;
   CCFGSetActionMap m_mapS1cN0t;

   // N0 N1
   CTwoWordsCFGSetActionMap m_mapN0wN1w;
   CWordCFGSetActionMap m_mapN0tN1w;
   CWordCFGSetActionMap m_mapN0wN1t;
   CCFGSetActionMap m_mapN0tN1t;

   // Bracket
#ifdef _CHINESE_CFG_H
   CIntMap m_mapBracketS0c;
   CWordIntMap m_mapBracketS0w;
   CWordIntMap m_mapBracketS0wS1c;
   CWordIntMap m_mapBracketS0cS1w;
   CIntMap m_mapBracketS0cS1c;
   CWordIntMap m_mapBracketS0cN0w;
   CWordIntMap m_mapBracketS0wN0t;
   CIntMap m_mapBracketS0cN0t;

   // Separator
   CIntMap m_mapS0cSeparator;
   CWordIntMap m_mapS0wcSeparator;
   CIntMap m_mapS0cSepCount;
   CWordIntMap m_mapS0wcSepCount;
   CIntMap m_mapS1cSeparator;
   CWordIntMap m_mapS1wcSeparator;
   CIntMap m_mapS1cSepCount;
   CWordIntMap m_mapS1wcSepCount;
   CIntMap m_mapS0cS1cSeparator;
   CIntMap m_mapS0cS1cSepCount;
#endif

   // Distance
   CWordIntMap m_mapS0wS1Dist;
   CIntMap m_mapS0cS1Dist;
   CWordIntMap m_mapS0S1wDist;
   CIntMap m_mapS0S1cDist;
   CTwoWordsActionMap m_mapS0wS1wDist;
   CWordIntMap m_mapS0wS1cDist;
   CWordIntMap m_mapS0cS1wDist;
   CIntMap m_mapS0cS1cDist;

   // S0 S1N0
   CWordCFGSetActionMap m_mapS0wS1cN0t;
   CWordCFGSetActionMap m_mapS0cS1wN0t;
   CWordCFGSetActionMap m_mapS0cS1cN0w;
   CCFGSetActionMap m_mapS0cS1cN0t;
   CCFGSetActionMap m_mapS0tS1tN0t;
   CCFGSetActionMap m_mapS0jS1jN0t;

   // S0 N0N1
   CWordCFGSetActionMap m_mapS0wN0tN1t;
   CWordCFGSetActionMap m_mapS0cN0wN1t;
   CWordCFGSetActionMap m_mapS0cN0tN1w;
   CCFGSetActionMap m_mapS0cN0tN1t;
   CCFGSetActionMap m_mapS0tN0tN1t;
   CCFGSetActionMap m_mapS0jN0tN1t;

   // S0 S1S2
   CWordCFGSetActionMap m_mapS0wS1cS2c;
   CWordCFGSetActionMap m_mapS0cS1wS2c;
   CWordCFGSetActionMap m_mapS0cS1cS2w;
   CCFGSetActionMap m_mapS0cS1cS2c;
   CCFGSetActionMap m_mapS0tS1tS2t;
   CCFGSetActionMap m_mapS0jS1jS2j;

   // tag bigram
   CIntMap m_mapS1cS2c;
   CIntMap m_mapS2cS3c;
   CIntMap m_mapN1tN2t;
   CIntMap m_mapN2tN3t;
   
   CIntMap m_mapS0cS2c;
   CIntMap m_mapS1cS3c;
   CIntMap m_mapS0cN1t;
   CIntMap m_mapN0tN2t;
   CIntMap m_mapN1tN3t;

   CIntMap m_mapN0tN1tN2t;

   // S0 S0LRUN0
   CCFGSetActionMap m_mapS0cS0RcN0t;
   CCFGSetActionMap m_mapS0cS0RjN0t;
   CWordCFGSetActionMap m_mapS0cS0RcN0w;

   CCFGSetActionMap m_mapS0cS0LcN0t;
   CWordCFGSetActionMap m_mapS0wS0LcN0t;
   CWordCFGSetActionMap m_mapS0cS0LcN0w;
   CWordCFGSetActionMap m_mapS0wS0RcN0t;
   CCFGSetActionMap m_mapS0cS0UcN0t;
   CWordCFGSetActionMap m_mapS0wS0UcN0t;
   CWordCFGSetActionMap m_mapS0cS0UcN0w;

   // S0 S0LRUS1
   CCFGSetActionMap m_mapS0cS0LcS1c;
   CCFGSetActionMap m_mapS0cS0LjS1j;
   CWordCFGSetActionMap m_mapS0cS0LcS1w;

   CWordCFGSetActionMap m_mapS0wS0LcS1c;
   CCFGSetActionMap m_mapS0cS0RcS1c;
   CWordCFGSetActionMap m_mapS0wS0RcS1c;
   CWordCFGSetActionMap m_mapS0cS0RcS1w;
   CCFGSetActionMap m_mapS0cS0UcS1c;
   CWordCFGSetActionMap m_mapS0wS0UcS1c;
   CWordCFGSetActionMap m_mapS0cS0UcS1w;

   // S0 S1LRUS1
   CCFGSetActionMap m_mapS0cS1cS1Rc;
   CCFGSetActionMap m_mapS0jS1cS1Rj;
   CWordCFGSetActionMap m_mapS0wS1cS1Rc;

   CCFGSetActionMap m_mapS0cS1cS1Lc;
   CWordCFGSetActionMap m_mapS0wS1cS1Lc;
   CWordCFGSetActionMap m_mapS0cS1wS1Lc;
   CWordCFGSetActionMap m_mapS0cS1wS1Rc;
   CCFGSetActionMap m_mapS0cS1cS1Uc;
   CWordCFGSetActionMap m_mapS0wS1cS1Uc;
   CWordCFGSetActionMap m_mapS0cS1wS1Uc;

   // Rule dictionary
   unsigned long m_nMaxWordFrequency;
   CWordToIntMap m_mapWordFrequency;

public:

   CWeight(bool bTrain) : CWeightBase(bTrain) ,

                          m_nMaxWordFrequency(0) ,

                          m_mapHw("HeadWord", TABLE_SIZE),
                          m_mapHt("HeadTag", TABLE_SIZE),
                          m_mapHc("HeadConstituents", TABLE_SIZE),
                          m_mapHwc("HeadWord", TABLE_SIZE),

                          m_mapHcMc("HeadModifierConstituents", TABLE_SIZE),
                          m_mapHwMc("HeadModifierWordConstituent", TABLE_SIZE),
                          m_mapHcMw("HeadModifierConstituentW", TABLE_SIZE),
                          m_mapHwMw("HeadModifierWrds", TABLE_SIZE),

                          m_mapBigramsL("BigramsOnTheLeft", TABLE_SIZE),
                          m_mapBigramsR("BigramsOnTheRight", TABLE_SIZE),
                          m_mapBigramsLexA("LexicalizedBigramsWC", TABLE_SIZE),
                          m_mapBigramsLexB("LexicalizedBigramsCW", TABLE_SIZE),

                          m_mapRules("Rules", TABLE_SIZE),
                          m_mapRulesWithSizes("RulesSz", TABLE_SIZE),

                          m_mapBracket("BracketType", TABLE_SIZE),

                          m_mapHtMt("HeadTagModifierTag", TABLE_SIZE),
                          m_mapHtMw("HeadTagModifierWord", TABLE_SIZE),
                          m_mapHwMt("HeadWordModifierTag", TABLE_SIZE),
                          m_mapSiblingTags("SiblingTs", TABLE_SIZE),
                          m_mapSiblingWordTag("SiblingWt", TABLE_SIZE),
                          m_mapSiblingTagWord("SiblingTw", TABLE_SIZE),
                          m_mapHtArityL("HeadTagArityLeft", TABLE_SIZE),
                          m_mapHtArityR("HeadTagArityRight", TABLE_SIZE),
                          m_mapHwArityL("HeadWordArityLeft", TABLE_SIZE),
                          m_mapHwArityR("HeadWordArityRight", TABLE_SIZE),

                          m_mapS0c("Stack0Constituent"),
                          m_mapS0w("Stack0Word"),
                          m_mapS0tc("Stack0Tag"),
                          m_mapS0wc("Stack0WordConstituent"),

                          m_mapS1c("Stack1Constituent"),
                          m_mapS1w("Stack1Word"),
                          m_mapS1tc("Stack1Tag"),
                          m_mapS1wc("Stack1WordConstituent"),

                          m_mapS2tc("Stack2Tag"),
                          m_mapS2wc("Stack2WordConstituent"),

                          m_mapS3tc("Stack3Tag"),
                          m_mapS3wc("Stack3WordConstituent"),

                          m_mapN0w("Next0Word"),
                          m_mapN0t("Next0Tag"),
                          m_mapN0wt("Next0WordTag"),
                                            
                          m_mapN1w("Next1Word"),
                          m_mapN1t("Next1Tag"),
                          m_mapN1wt("Next1WordTag"),

                          m_mapN2w("Next2Word"),
                          m_mapN2t("Next2Tag"),
                          m_mapN2wt("Next2WordTag"),

                          m_mapN3w("Next3Word"),
                          m_mapN3t("Next3Tag"),
                          m_mapN3wt("Next3WordTag"),

                          m_mapS0Lwc("Stack0LeftWord"),
                          m_mapS0Ltc("Stack0LeftConstituent"), 
                          m_mapS0cS0HcS0L2c("Stack0ConstituentStack0HeadSubConstituentS0Left2SubConstituents"),

                          m_mapS0Rwc("Stack0RightWord"),
                          m_mapS0Rtc("Stack0RightConstituent"), 
                          m_mapS0cS0HcS0R2c("Stack0ConstituentStack0HeadSubConstituentS0Right2SubConstituents"),

                          m_mapS0Uwc("Stack0UniWord"),
                          m_mapS0Utc("Stack0UniConstituent"), 

                          m_mapS1Lwc("Stack1LeftWord"),
                          m_mapS1Ltc("Stack1LeftConstituent"), 

                          m_mapS1Rwc("Stack1RightWord"),
                          m_mapS1Rtc("Stack1RightConstituent"), 
                          m_mapS1cS1HcS1R2c("Stack1ConstituentStack1HeadSubConstituentS1Right2SubConstituents"),

                          m_mapS1Uwc("Stack1UniWord"),
                          m_mapS1Utc("Stack1UniConstituent"), 

                          m_mapS0LDw("Stack0LeftDepWord"),
                          m_mapS0LDt("Stack0LeftDepTag"), 

                          m_mapS0RDw("Stack0RightDepWord"),
                          m_mapS0RDt("Stack0RightDepTag"), 

                          m_mapS1LDw("Stack1LeftDepWord"),
                          m_mapS1LDt("Stack1LeftDepTag"), 

                          m_mapS1RDw("Stack1RightDepWord"),
                          m_mapS1RDt("Stack1RightDepTag"), 

                          m_mapS0wcS1wc("Stack0WordStack1Word"),
                          m_mapS0wS1c("Stack0WordStack1Constituent"),
                          m_mapS0cS1w("Stack0ConstituentStack1Word"),
                          m_mapS0cS1c("Stack0ConstituentStack1Constituent"),
                          m_mapS0wtS1wt("Stack0WordTagStack1WordTag"),
                          m_mapS0wtS1w("Stack0WordTagStack1Word"),
                          m_mapS0wS1wt("Stack0WordStack1WordTag"),
                          m_mapS0wS1w("Stack0WordStack1Word"),
                          m_mapS0wtS1t("Stack0WordTagStack1Tag"),
                          m_mapS0tS1wt("Stack0TagStack1WordTag"),
                          m_mapS0tS1t("Stack0TagStack1Tag"),
                          m_mapBetweenTags("BetweenTags"),
                                            
                          m_mapS0wN0w("Stack0WordNext0Word", TABLE_SIZE),
                          m_mapS0wN0t("Stack0WordNext0Tag", TABLE_SIZE),
                          m_mapS0cN0w("Stack0ConstituentNext0Word", TABLE_SIZE),
                          m_mapS0cN0t("Stack0ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0cmN0tm("Stack0ConstituentRhythmNext0TagRhythm", TABLE_SIZE),

                          m_mapS1wN0w("Stack1WordNext0Word", TABLE_SIZE),
                          m_mapS1wN0t("Stack1WordNext0Tag", TABLE_SIZE),
                          m_mapS1cN0w("Stack1ConstituentNext0Word", TABLE_SIZE),
                          m_mapS1cN0t("Stack1ConstituentNext0Tag", TABLE_SIZE),

                          m_mapN0wN1w("Next0WordNext1Word"),
                          m_mapN0tN1w("Next0TagNext1Word"),
                          m_mapN0wN1t("Next0WordNext1Tag"),
                          m_mapN0tN1t("Next0TagNext1WordTag"),

#ifdef _CHINESE_CFG_H
                          m_mapBracketS0c("BracketStack0Constituent"), 
                          m_mapBracketS0w("BracketStack0Word"),
                          m_mapBracketS0wS1c("BracketStack0WordStack1Constituent"),
                          m_mapBracketS0cS1w("BracketStack0ConstituentStack1Word"),
                          m_mapBracketS0cS1c("BracketStack0ConsituentStack1Constituent"),
                          m_mapBracketS0cN0w("BracketStack0ConstituentNext0Word"),
                          m_mapBracketS0wN0t("BracketStack0WordNext0Tag"),
                          m_mapBracketS0cN0t("BracketStack0ConstituentNext0Tag"),

                          m_mapS0cSeparator("Stack0ConstituentSeparator"),
                          m_mapS0wcSeparator("Stack0WordConstituentSeparator"),
                          m_mapS0cSepCount("Stack0ConstituentSeparatorCount"),
                          m_mapS0wcSepCount("Stack0WordConstituentSeparatorCount"),
                          m_mapS1cSeparator("Stack1ConstituentSeparator"),
                          m_mapS1wcSeparator("Stack1WordConstituentSeparator"),
                          m_mapS1cSepCount("Stack1ConstituentSeparatorCount"),
                          m_mapS1wcSepCount("Stack1WordConstituentSeparatorCount"),
                          m_mapS0cS1cSeparator("Stack0ConstituentStack1ConstituentSeparator"),
                          m_mapS0cS1cSepCount("Stack0ConstituentStack1ConstituentSeparatorCount"),
#endif

                          m_mapS0wS1Dist("Stack0WordStack1Distance"),
                          m_mapS0cS1Dist("Stack0ConstituentStack1Distance"),
                          m_mapS0S1wDist("Stack0Stack1WordDistance"),
                          m_mapS0S1cDist("Stack0Stack1ConstituentDistance"),
                          m_mapS0wS1wDist("Stack0WordStack1WordDistance"),
                          m_mapS0wS1cDist("Stack0WordStack1ConstituentDistance"),
                          m_mapS0cS1wDist("Stack0ConstituentStack1WordDistance"),
                          m_mapS0cS1cDist("Stack0ConstituentStack1ConstituentDistance"),

                          m_mapS0wS1cN0t("Stack0WordStack1ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0cS1wN0t("Stack0ConstituentStack1WordNext0Tag", TABLE_SIZE),
                          m_mapS0cS1cN0w("Stack0ConstituentStack1ConstituentNext0Word", TABLE_SIZE),
                          m_mapS0cS1cN0t("Stack0ConstituentStack1ConstituentNext0Tag", TABLE_SIZE),
                          m_mapS0tS1tN0t("Stack0TagStack1TagNext0Tag", TABLE_SIZE),
                          m_mapS0jS1jN0t("Stack0TagOrConstituentStack1TagOrConstituentNext0Tag", TABLE_SIZE),

                          m_mapS0wN0tN1t("Stack0WordNext0TagNext", TABLE_SIZE),
                          m_mapS0cN0wN1t("Stack0ConstituentNext0WordNext1Tag", TABLE_SIZE),
                          m_mapS0cN0tN1w("Stack0ConstituentNext0TagNext1Word", TABLE_SIZE),
                          m_mapS0cN0tN1t("Stack0ConstituentNext0TagNext1Tag", TABLE_SIZE),
                          m_mapS0tN0tN1t("Stack0TagNext0TagNext1Tag", TABLE_SIZE),
                          m_mapS0jN0tN1t("Stack0TagOrConstituentNext0TagNext1Tag", TABLE_SIZE),

                          m_mapS0wS1cS2c("Stack0WordStack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS0cS1wS2c("Stack0ConstituentStack1WordStack2Constituent", TABLE_SIZE),
                          m_mapS0cS1cS2w("Stack0ConstituentStack1ConstituentStack2Word", TABLE_SIZE),
                          m_mapS0cS1cS2c("Stack0ConstituentStack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS0tS1tS2t("Stack0TagStack1TagStack2Tag", TABLE_SIZE),
                          m_mapS0jS1jS2j("Stack0TagOrConstituentStack1TagOrConstituentStack2TagOrConstituent", TABLE_SIZE),

                          m_mapS1cS2c("Stack1ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS2cS3c("Stack2ConstituentStack3Constituent", TABLE_SIZE),
                          m_mapN1tN2t("Next1TagNext2Tag", TABLE_SIZE),
                          m_mapN2tN3t("Next2TagNext3Tag", TABLE_SIZE),
                                               
                          m_mapS0cS2c("Stack0ConstituentStack2Constituent", TABLE_SIZE),
                          m_mapS1cS3c("Stack1ConstituentStack3Constituent", TABLE_SIZE),
                          m_mapS0cN1t("Stack0ConstituentNext1Tag", TABLE_SIZE),
                          m_mapN0tN2t("Next0TagNext2Tag", TABLE_SIZE),
                          m_mapN1tN3t("Next1TagNextTag", TABLE_SIZE),

                          m_mapS0cS0LcN0t("Stack0ConstituentStack0LConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0LcN0t("Stack0WordStack0LConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0LcN0w("Stack0ConstituentStack0LConstituentNex0tWord", TABLE_SIZE),
                          m_mapS0cS0RcN0t("Stack0ConstituentStack0RConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0RjN0t("Stack0ConstituentStack0RTagOrConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0RcN0t("Stack0WordStack0RConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0RcN0w("Stack0ConstituentStack0RConstituentNex0tWord", TABLE_SIZE),
                          m_mapS0cS0UcN0t("Stack0ConstituentStack0UConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0wS0UcN0t("Stack0WordStack0UConstituentNex0tTag", TABLE_SIZE),
                          m_mapS0cS0UcN0w("Stack0ConstituentStack0UConstituentNex0tWord", TABLE_SIZE),

                          m_mapS0cS0LcS1c("Stack0ConstituentStack0LConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0LjS1j("Stack0ConstituentStack0LTagOrConstituentStack1TagOrConstituent", TABLE_SIZE),
                          m_mapS0wS0LcS1c("Stack0WordStack0LConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0LcS1w("Stack0ConstituentStack0LConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS0RcS1c("Stack0ConstituentStack0RConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0wS0RcS1c("Stack0WordStack0RConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0RcS1w("Stack0ConstituentStack0RConstituentStack1Word", TABLE_SIZE),
                          m_mapS0cS0UcS1c("Stack0ConstituentStack0UConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0wS0UcS1c("Stack0WordStack0UConstituentStack1Constituent", TABLE_SIZE),
                          m_mapS0cS0UcS1w("Stack0ConstituentStack0UConstituentStack1Word", TABLE_SIZE),

                          m_mapN0tN1tN2t("NextTagTrigram", TABLE_SIZE),
                          m_mapS0cS1cS1Lc("Stack0ConstituentStack1ConstituentStack1LConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Lc("Stack0WordStack1ConstituentStack1LConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Lc("Stack0ConstituentStack1WordStack1LConstituent", TABLE_SIZE),
                          m_mapS0cS1cS1Rc("Stack0ConstituentStack1ConstituentStack1RConstituent", TABLE_SIZE),
                          m_mapS0jS1cS1Rj("Stack0TagOrConstituentStack1ConstituentStack1RTagOrConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Rc("Stack0WordStack1ConstituentStack1RConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Rc("Stack0ConstituentStack1WordStack1RConstituent", TABLE_SIZE),
                          m_mapS0cS1cS1Uc("Stack0ConstituentStack1ConstituentStack1UConstituent", TABLE_SIZE),
                          m_mapS0wS1cS1Uc("Stack0WordStack1ConstituentStack1UConstituent", TABLE_SIZE),
                          m_mapS0cS1wS1Uc("Stack0ConstituentStack1WordStack1UConstituent", TABLE_SIZE)
   { }


   // MEHTODS
   virtual void loadScores(std::ifstream &is);
   virtual void saveScores(std::ofstream &os);
   void computeAverageFeatureWeights(int round);
   SCORE_TYPE dotProduct(const CWeight &w);
 
};

};
};

#endif
