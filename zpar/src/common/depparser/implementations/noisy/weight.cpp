/****************************************************************
 *                                                              *
 * weight.cpp - the weight definition method                    *
 *                                                              *
 * Part of the dependency parser of english                     *
 *                                                              *
 * Yue Zhang                                                    *
 * 2007.8                                                       *
 *                                                              *
 ****************************************************************/

#include "weight.h"

using namespace TARGET_LANGUAGE;
using namespace TARGET_LANGUAGE::depparser;

//const CScore<SCORE_TYPE> g_zeroScore;

/*
   left(m_mapHeadWord)right \
   left(m_mapDepWord)right \
   left(m_mapHeadWordTag)right \
   left(m_mapDepWordTag)right \
   left(m_mapHeadTag)right \
   left(m_mapDepTag)right \
   left(m_mapHeadWordTagDepWordTag)right \
   left(m_mapHeadWordTagDepWord)right \
   left(m_mapHeadWordTagDepTag)right \
   left(m_mapHeadWordDepWordTag)right \
   left(m_mapHeadTagDepWordTag)right \
   left(m_mapHeadWordDepWord)right \
   left(m_mapHeadTagDepTag)right \
   left(m_mapBetweenTags)right \
   left(m_mapSurroundingTagsLL)right \
   left(m_mapSurroundingTagsLR)right \
   left(m_mapSurroundingTagsRL)right \
   left(m_mapSurroundingTagsRR)right \
   left(m_mapSiblingWords)right \
   left(m_mapSiblingWordTag)right \
   left(m_mapSiblingTagWord)right \
   left(m_mapSiblingTags)right \
   left(m_mapSiblingAndParentTags)right \
   left(m_mapTwoSiblingTags)right \
   left(m_mapTwoSiblingAndParentTags)right \
   left(m_mapGrandChildTags)right \
   left(m_mapHeadWordTagArity)right \
   left(m_mapHeadTagArity)right 
*/

#define iterate_templates(left, right) \
   left(m_mapHeadWord)right \
   left(m_mapDepWord)right \
   left(m_mapHeadWordTag)right \
   left(m_mapDepWordTag)right \
   left(m_mapHeadTag)right \
   left(m_mapDepTag)right \
\
   left(m_mapHeadWordTagDepWordTag)right \
   left(m_mapHeadWordTagDepWord)right \
   left(m_mapHeadWordTagDepTag)right \
   left(m_mapHeadWordDepWordTag)right \
   left(m_mapHeadTagDepWordTag)right \
   left(m_mapHeadWordDepWord)right \
   left(m_mapHeadTagDepTag)right \
\
   left(m_mapBetweenTags)right \
   left(m_mapSurroundingTagsLL)right \
   left(m_mapSurroundingTagsLR)right \
   left(m_mapSurroundingTagsRL)right \
   left(m_mapSurroundingTagsRR)right \
\
   left(m_mapSiblingWords)right \
   left(m_mapSiblingWordTag)right \
   left(m_mapSiblingTagWord)right \
   left(m_mapSiblingTags)right \
   left(m_mapSiblingAndParentTags)right \
\
   left(m_mapTwoSiblingTags)right \
   left(m_mapTwoSiblingAndParentTags)right \
   left(m_mapGrandChildTags)right \
   left(m_mapHeadWordTagArity)right \
   left(m_mapHeadTagArity)right \
\
   left(m_mapSTwt)right\
   left(m_mapSTw)right\
   left(m_mapSTt)right\
   left(m_mapN0wt)right\
   left(m_mapN0w)right\
   left(m_mapN0t)right\
   left(m_mapN1wt)right\
   left(m_mapN1w)right\
   left(m_mapN1t)right\
\
   left(m_mapSTwtN0wt)right\
   left(m_mapSTwtN0w)right\
   left(m_mapSTwN0wt)right\
   left(m_mapSTtN0wt)right\
   left(m_mapSTwtN0t)right\
   left(m_mapSTwN0w)right\
   left(m_mapSTtN0t)right\
\
   left(m_mapN0tN1t)right\
   left(m_mapN0tN1tN2t)right\
   left(m_mapSTtN0tN1t)right\
   left(m_mapSTtN0tN0LDt)right\
   left(m_mapSTHtSTtN0t)right\
   left(m_mapSTtSTLDtN0t)right\
   left(m_mapSTtSTRDtN0t)right\
\
   left(m_mapN0wN1t)right\
   left(m_mapN0wN1tN2t)right\
   left(m_mapSTtN0wN1t)right\
   left(m_mapSTtN0wN0LDt)right\
   left(m_mapSTHtSTtN0w)right\
   left(m_mapSTtSTLDtN0w)right\
   left(m_mapSTtSTRDtN0w)right\
\
   left(m_mapSRwtN0t)right\
   left(m_mapSRtN0wt)right\
   left(m_mapSRtN0t)right

/*---------------------------------------------------------------
 *
 * loadScores - load scores from the file specified at constructor
 *              currently this uses database, but it can be modified
 *         
 * Affects: m_bScoreModified, clearing it. 
 *
 *--------------------------------------------------------------*/

void CWeight::loadScores() {
   clock_t time_start = clock();
   std::cout<<"Loading scores..."; std::cout.flush();
   std::ifstream file ; 
   file.open(m_sRecordPath.c_str()) ;

   if (!file.is_open()) {
      std::cout << " empty." << std::endl; return;
   }

   iterate_templates(file >>,;);

   file.close() ;
   std::cout << " done. (" << double(clock()-time_start)/CLOCKS_PER_SEC << "s)" << std::endl;
}

/*---------------------------------------------------------------
 *
 * saveScores - save scores back to database
 *
 * This method is called by the destructor is m_bScoreModified
 * is true.
   std::cout<<"done."<<std::endl;
 *
 *--------------------------------------------------------------*/

void CWeight::saveScores() {
   std::cout<<"Saving scores..."; std::cout.flush();
   std::ofstream file ;
   file.open(m_sRecordPath.c_str()) ;

   iterate_templates(file<<,;)

   file.close();
   std::cout<<" done."<<std::endl;
}

/*--------------------------------------------------------------
 *
 * computeAverageFeatureWeights - compute average feature weights
 *
 *-------------------------------------------------------------*/

void CWeight::computeAverageFeatureWeights(int round) {
   std::cout<<"Computing averaged (total) feature vector..."; std::cout.flush();
   iterate_templates(,.computeAverage(round);) ;

   std::cout<<"done."<<std::endl;
}

