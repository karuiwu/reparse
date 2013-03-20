#include<iostream>
#include<map>
#include<string>
#include<sstream>
#include<vector>
#include<queue>
#include<fstream>
#include<iostream>

#include <deque>
#include <ostream>

#include "ksutil.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//
// item: a class for elements in the stack or the input list in 
// stack-based parsing algorithms.
//
// goldin and goldout are the number of incoming and outgoing arcs,
// according to the training data (not used at test time).
//
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

class item {
 public:
  
  string word;
  int word_map; // map to number for the word
  string pos;
  int pos_map; // map to number for the pos
  string lemma;
  string morph;
  string cpos;
  int idx; //item number in the word sequence
  int link;
  string label;
  int goldlink;
  string goldlabel;
  double score;
  int goldin;
  int goldout;
  int lch;
  int rch;
  int nch;
  int nlch;
  int nrch;

  item(): 
    word( "_" ),
    word_map(0),
    pos( "_" ), 
    pos_map(0),
    lemma( "_" ), 
    morph( "_" ), 
    cpos( "_" ), 
    idx( 0 ),
    score( 0.0 ),
    link( 0 ),
    label( "_" ),
    goldlink( 0 ),
    goldlabel( "_" ),
    goldin( 0 ),
    goldout( 0 ),
    lch( 0 ),
    rch( 0 ), 
      nch( 0 ), 
      nlch( 0 ),
      nrch( 0 )  {
  }

  item( const item &it ): 
    word( it.word ), 
    word_map( it.word_map),
    pos( it.pos ), 
    pos_map( it.pos_map),
    lemma( it.lemma ), 
    morph( it.morph ), 
    cpos( it.cpos ), 
    idx( it.idx ),
    score( it.score ),
    link( it.link ),
    label( it.label ),
    goldlink( it.goldlink ),
    goldlabel( it.goldlabel ),
    goldin( it.goldin ),
    goldout( it.goldout ),
    lch( it.lch ),
    rch( it.rch ),
      nch( it.nch ),
      nlch( it.nrch ),
      nrch( it.nlch ) {
  } 
    
  void print(std::ostream& strm=std::cout) const {
    strm << idx << "\t";
    strm << word << "\t";
    strm << lemma << "\t";
    strm << cpos << "\t";
    strm << pos << "\t";
    strm << morph << "\t";
    strm << link << "\t";
    strm << label << "\t";
    strm << "_" << "\t";
    strm << "_" << "\n";
  }
};

