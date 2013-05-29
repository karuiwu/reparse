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

class parserstate {
 public:

  item dummyitem;
  vector<item> inputq;
  vector<int> s;

  vector<int> outlinks;
  vector<string> outlabels;

  double score;
  int numacts;
  string prevact;
  int i;

 parserstate():
  score( 0.0 ),
    numacts( 0 ),
    prevact( "_" ),
    i( 0 ) {
  }

 parserstate( vector<item> inq ):
  inputq( inq ),
    score( 0.0 ),
    numacts( 0 ),
    prevact( "_" ),
    i( 0 ) {
  }

 parserstate( const parserstate &ps ):
  inputq( ps.inputq ),
    s( ps.s ),
    outlinks( ps.outlinks ),
    outlabels( ps.outlabels ),
    score( ps.score ),
    numacts( ps.numacts ),
    prevact( ps.prevact ),
    i( ps.i ) {
  }
  std::ostream& print(std::ostream& strm=std::cout) const {
    strm << "stack size:" << s.size() << std::endl;
    const item *s1 = getst( 1 );
    const item *s2 = getst( 2 );
    const item *s3 = getst( 3 );

    const item *q1 = getq( 1 );
    const item *q2 = getq( 2 );
    const item *q3 = getq( 3 );

    strm << "s1:" << std::endl;
    strm << "idx:" << s1->idx << "\t";
    strm << "word:" << s1->word << "\t";
    strm << "pos:" << s1->pos << "\t";
    strm << "label:" << s1->label << "\n";

    strm << "s2:" << std::endl;
    strm << "idx:" << s2->idx << "\t";
    strm << "word:" << s2->word << "\t";
    strm << "pos:" << s2->pos << "\t";
    strm << "label:" << s2->label << "\n";

    strm << "s3:" << std::endl;
    strm << "idx:" << s3->idx << "\t";
    strm << "word:" << s3->word << "\t";
    strm << "pos:" << s3->pos << "\t";
    strm << "label:" << s3->label << "\n";

    strm << "q1:" << std::endl;
    strm << "idx:" << q1->idx << "\t";
    strm << "word:" << q1->word << "\t";
    strm << "pos:" << q1->pos << "\t";

    strm << "q2:" << std::endl;
    strm << "idx:" << q2->idx << "\t";
    strm << "word:" << q2->word << "\t";
    strm << "pos:" << q2->pos << "\t";
    strm << "q3:" << std::endl;
    strm << "idx:" << q3->idx << "\t";
    strm << "word:" << q2->word << "\t";
    strm << "pos:" << q3->pos << "\t";

    return strm;
  }

  static bool equivalent( const parserstate& pst1, const parserstate& pst2 ) {
    const item *s1_1 = pst1.getst(1);
    const item *s1_2 = pst2.getst(1);
    if (s1_1->word_map != s1_2->word_map)
      return false;
    if (s1_1->pos_map != s1_2->pos_map)
      return false;
    if (s1_1->label.compare(s1_2->label) != 0)
      return false;

    //checking the left & right nodes                                                                        
    if (pst1.inputq[ s1_1->lch ].pos_map != pst2.inputq[ s1_2->lch ].pos_map)
      return false;
    if (pst1.inputq[ s1_1->lch ].label.compare(pst2.inputq[ s1_2->lch ].label) != 0)
      return false;
    if (pst1.inputq[ s1_1->rch ].pos_map != pst2.inputq[ s1_2->rch ].pos_map)
      return false;
    if (pst1.inputq[ s1_1->rch ].label.compare(pst2.inputq[ s1_2->rch ].label) != 0)
      return false;

    const item *s2_1 = pst1.getst(2);
    const item *s2_2 = pst2.getst(2);
    if (s2_1->word_map != s2_2->word_map)
      return false;
    if (s2_1->pos_map != s2_2->pos_map)
      return false;
    if (s2_1->label.compare(s2_2->label) != 0)
      return false;

    //checking the left & right nodes
    if (pst1.inputq[ s2_1->lch ].pos_map != pst2.inputq[ s2_2->lch ].pos_map)
      return false;
    if (pst1.inputq[ s2_1->lch ].label.compare(pst2.inputq[ s2_2->lch ].label) != 0)
      return false;
    if (pst1.inputq[ s2_1->rch ].pos_map != pst2.inputq[ s2_2->rch ].pos_map)
      return false;
    if (pst1.inputq[ s2_1->rch ].label.compare(pst2.inputq[ s2_2->rch ].label) != 0)
      return false;

    int dist1_1 = s1_1->idx-s2_1->idx;
    int dist1_2 = s1_2->idx-s2_2->idx;

    if (dist1_1 != dist1_2)
      return false;

    if (dist1_1 > 1)
      if (pst1.inputq[s1_1->idx - 1 ].pos_map != pst2.inputq[s1_2->idx - 1 ].pos_map)
        return false;

    if (dist1_1 > 2)
      if (pst1.inputq[s2_1->idx + 1 ].pos_map != pst2.inputq[s2_2->idx + 1 ].pos_map)
        return false;

    const item *s3_1 = pst1.getst(3);
    const item *s3_2 = pst2.getst(3);
    if (s3_1->word_map != s3_2->word_map)
      return false;
    if (s3_1->pos_map != s3_2->pos_map)
      return false;
    if (s3_1->label.compare(s3_2->label) != 0)
      return false;

    const item *q1_1 = pst1.getq( 1 );
    const item *q1_2 = pst2.getq( 1 );
    if (q1_1->pos_map != q1_2->pos_map)
      return false;
    if (q1_1->word_map != q1_2->word_map)
      return false;

    int dist2_1 = q1_1->idx - s1_1->idx;
    int dist2_2 = q1_2->idx - s1_2->idx;
    if (dist2_1 != dist2_2)
      return false;

    const item *q2_1 = pst1.getq( 2 );
    const item *q2_2 = pst2.getq( 2 );
    if (q2_1->pos_map != q2_2->pos_map)
      return false;
    if (q2_1->word_map != q2_2->word_map)
      return false;

    const item *q3_1 = pst1.getq( 3 );
    const item *q3_2 = pst2.getq( 3 );
    if (q3_1->pos_map != q3_2->pos_map)
      return false;

    if (pst1.prevact.compare(pst2.prevact) != 0)
      return false;

    // if none of the above happens
    return true;

  }
  // returns true if the two states are equivalent (in no-lookahead mode)
  // equivalence is defined based on the features that we extract from each state
  // so if we change the features we need to change this function accordingly
  static bool equivalentNoLookahead( const parserstate& pst1,
                                     const parserstate& pst2 ) {
    const item *s1_1 = pst1.getst(1);
    const item *s1_2 = pst2.getst(1);
    if (s1_1->word_map != s1_2->word_map)
      return false;
    if (s1_1->pos_map != s1_2->pos_map)
      return false;
    if (s1_1->label.compare(s1_2->label) != 0)
      return false;

    //checking the left & right nodes
    if (pst1.inputq[ s1_1->lch ].pos_map != pst2.inputq[ s1_2->lch ].pos_map)
      return false;
    if (pst1.inputq[ s1_1->lch ].label.compare(pst2.inputq[ s1_2->lch ].label)
	!= 0)
      return false;
    if (pst1.inputq[ s1_1->rch ].pos_map != pst2.inputq[ s1_2->rch ].pos_map)
      return false;
    if (pst1.inputq[ s1_1->rch ].label.compare(pst2.inputq[ s1_2->rch ].label)
        != 0)
      return false;

    const item *s2_1 = pst1.getst(2);
    const item *s2_2 = pst2.getst(2);
    if (s2_1->word_map != s2_2->word_map)
      return false;
    if (s2_1->pos_map != s2_2->pos_map)
      return false;
    if (s2_1->label.compare(s2_2->label) != 0)
      return false;

    //checking the left & right nodes
    if (pst1.inputq[ s2_1->lch ].pos_map != pst2.inputq[ s2_2->lch ].pos_map)
      return false;
    if (pst1.inputq[ s2_1->lch ].label.compare(pst2.inputq[ s2_2->lch ].label)
        != 0)
      return false;
    if (pst1.inputq[ s2_1->rch ].pos_map != pst2.inputq[ s2_2->rch ].pos_map)
      return false;
    if (pst1.inputq[ s2_1->rch ].label.compare(pst2.inputq[ s2_2->rch ].label)
        != 0)
      return false;

    int dist1_1 = s1_1->idx-s2_1->idx;
    int dist1_2 = s1_2->idx-s2_2->idx;

    if (dist1_1 != dist1_2)
      return false;

    if (dist1_1 > 1)
      if (pst1.inputq[s1_1->idx - 1 ].pos_map != pst2.inputq[s1_2->idx - 1 ].pos_map)
        return false;

    if (dist1_1 > 2)
      if (pst1.inputq[s2_1->idx + 1 ].pos_map != pst2.inputq[s2_2->idx + 1 ].pos_map)
        return false;

    const item *s3_1 = pst1.getst(3);
    const item *s3_2 = pst2.getst(3);
    if (s3_1->word_map != s3_2->word_map)
      return false;
    if (s3_1->pos_map != s3_2->pos_map)
      return false;
    if (s3_1->label.compare(s3_2->label) != 0)
      return false;

    if (pst1.prevact.compare(pst2.prevact) != 0)
      return false;

    // if none of the above happens
    return true;

  }

  bool operator<( const parserstate &right ) const {
    return score < right.score;
  }

  struct compareStatePointers : public std::binary_function<parserstate*, parserstate*, bool> {
    bool operator()(const parserstate* lhs, const parserstate* rhs) const
    {
      return (*lhs) < (*rhs);
    }
  };

  const item* getst( int si ) const {
    si = s.size() - si;

    if( si < 0 ) {
      return &dummyitem;
    }

    if( si > ( s.size() - 1 ) ) {
      return &dummyitem;
    }

    return &( inputq[s[si]] );
  }

  ////////////////////////////////////////////////////////////////////////                           
  //                                                                                                 
  // pact: a simple class for sortable parser actions                                                
  //                                                                                                 
  ////////////////////////////////////////////////////////////////////////                           

  class pact {
  public:
    string label;
    double score;

    pact( double p, string lab ) { label = lab; score = p; }

    bool operator<( const pact & right ) const {
      return score < right.score;
    }
  };
