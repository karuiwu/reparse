#include "tags.h"
#ifdef LABELED
#include "dependency/label/penn.h"
#endif

namespace english {
#ifdef LABELED
inline bool canAssignLabel(const std::vector< CTaggedWord<CTag,TAG_SEPARATOR> > &sent, const int &head, const int &dep, const CDependencyLabel&label) {
   assert(head==DEPENDENCY_LINK_NO_HEAD||head>=0); // correct head
   assert(dep>=0);
   // if the head word is none, only ROOT
   if (head==DEPENDENCY_LINK_NO_HEAD) {
      if (label.code()==PENN_DEP_ROOT) 
         return true;
      return false;
   }
      // for each case
   const unsigned &head_pos = sent[head].tag.code();
   const unsigned &dep_pos = sent[dep].tag.code();
   assert(head!=DEPENDENCY_LINK_NO_HEAD);
   if (label == PENN_DEP_ROOT) // now head is not DEPENDENCY_LINK_NO_HEAD
      return false;
   if (label==PENN_DEP_PRD) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_RP
           || head_pos==PENN_TAG_DOLLAR
           || head_pos==PENN_TAG_FW
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_TO
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || head_pos==PENN_TAG_ADVERB_COMPARATIVE
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_WP_DOLLAR
           || head_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_ADJECTIVE_COMPARATIVE
           || head_pos==PENN_TAG_SYM
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_POS
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_PDT
           || dep_pos==PENN_TAG_EX
           || dep_pos==PENN_TAG_WP_DOLLAR
           || dep_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_SYM
         ) return false;
   }
   else if (label==PENN_DEP_VC) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_WDT
           || head_pos==PENN_TAG_ADJECTIVE
           || head_pos==PENN_TAG_WP
           || head_pos==PENN_TAG_DT
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_RP
           || head_pos==PENN_TAG_DOLLAR
           || head_pos==PENN_TAG_NOUN
           || head_pos==PENN_TAG_FW
           || head_pos==PENN_TAG_POS
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_TO
           || head_pos==PENN_TAG_PRP
           || head_pos==PENN_TAG_ADVERB
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_NOUN_PLURAL
           || head_pos==PENN_TAG_NOUN_PROPER
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_WRB
           || head_pos==PENN_TAG_CC
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || head_pos==PENN_TAG_ADVERB_COMPARATIVE
           || head_pos==PENN_TAG_CD
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_IN
           || head_pos==PENN_TAG_WP_DOLLAR
           || head_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_ADJECTIVE_SUPERLATIVE
           || head_pos==PENN_TAG_ADJECTIVE_COMPARATIVE
           || head_pos==PENN_TAG_SYM
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_PRP_DOLLAR
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_WDT
           || dep_pos==PENN_TAG_WP
           || dep_pos==PENN_TAG_DT
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_RP
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_FW
           || dep_pos==PENN_TAG_POS
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_CC
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_PDT
           || dep_pos==PENN_TAG_CD
           || dep_pos==PENN_TAG_EX
           || dep_pos==PENN_TAG_WP_DOLLAR
           || dep_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || dep_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_ADJECTIVE_SUPERLATIVE
           || dep_pos==PENN_TAG_ADJECTIVE_COMPARATIVE
           || dep_pos==PENN_TAG_SYM
           || dep_pos==PENN_TAG_UH
         ) return false;
   }
   else if (label==PENN_DEP_OBJ) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_WDT
           || head_pos==PENN_TAG_ADJECTIVE
           || head_pos==PENN_TAG_WP
           || head_pos==PENN_TAG_DT
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_RP
           || head_pos==PENN_TAG_DOLLAR
           || head_pos==PENN_TAG_FW
           || head_pos==PENN_TAG_POS
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_TO
           || head_pos==PENN_TAG_PRP
           || head_pos==PENN_TAG_ADVERB
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_NOUN_PROPER
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_WRB
           || head_pos==PENN_TAG_CC
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || head_pos==PENN_TAG_ADVERB_COMPARATIVE
           || head_pos==PENN_TAG_CD
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_IN
           || head_pos==PENN_TAG_WP_DOLLAR
           || head_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_ADJECTIVE_SUPERLATIVE
           || head_pos==PENN_TAG_ADJECTIVE_COMPARATIVE
           || head_pos==PENN_TAG_SYM
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_WDT
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_RP
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_TO
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_CC
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_EX
           || dep_pos==PENN_TAG_WP_DOLLAR
           || dep_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_SYM
         ) return false;
   }
   else if (label==PENN_DEP_SUB) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_DT
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_RP
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_PRP
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_CC
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_WP_DOLLAR
           || head_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_ADJECTIVE_SUPERLATIVE
           || head_pos==PENN_TAG_SYM
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_RP
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_ADVERB_COMPARATIVE
           || dep_pos==PENN_TAG_WP_DOLLAR
           || dep_pos==PENN_TAG_MD
           || dep_pos==PENN_TAG_R_BRACKET
         ) return false;
   }
   else if (label==PENN_DEP_VMOD) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_WDT
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_RP
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_WRB
           || head_pos==PENN_TAG_CC
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_WP_DOLLAR
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_SYM
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_R_BRACKET
         ) return false;
   }
   else if (label==PENN_DEP_PMOD) {
      if ( head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_VERB_PRES
           || head_pos==PENN_TAG_WDT
           || head_pos==PENN_TAG_WP
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_POS
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_PRP
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_WRB
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || head_pos==PENN_TAG_CD
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_WP_DOLLAR
           || head_pos==PENN_TAG_MD
           || head_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_ADJECTIVE_SUPERLATIVE
           || head_pos==PENN_TAG_ADJECTIVE_COMPARATIVE
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_PDT
           || dep_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_SYM
         ) return false;
   }
   else if (label==PENN_DEP_DEP) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_POS
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_POS
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_PDT
           || dep_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || dep_pos==PENN_TAG_EX
           || dep_pos==PENN_TAG_R_BRACKET
         ) return false;
   }
   else if (label==PENN_DEP_NMOD) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_CC
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_R_BRACKET
         ) return false;
   }
   else if (label==PENN_DEP_AMOD) {
      if ( head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_POS
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_PRP
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_WP_DOLLAR
           || head_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_SYM
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_WP_DOLLAR
           || dep_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || dep_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_UH
         ) return false;
   }
   else if (label==PENN_DEP_P) {
      if ( head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_PRP_DOLLAR
           || dep_pos==PENN_TAG_VERB_PROG
           || dep_pos==PENN_TAG_VERB_PAST
           || dep_pos==PENN_TAG_VERB_PAST_PARTICIPATE
           || dep_pos==PENN_TAG_VERB_PRES
           || dep_pos==PENN_TAG_WDT
           || dep_pos==PENN_TAG_ADJECTIVE
           || dep_pos==PENN_TAG_WP
           || dep_pos==PENN_TAG_VERB_THIRD_SINGLE
           || dep_pos==PENN_TAG_DT
           || dep_pos==PENN_TAG_RP
           || dep_pos==PENN_TAG_NOUN
           || dep_pos==PENN_TAG_FW
           || dep_pos==PENN_TAG_POS
           || dep_pos==PENN_TAG_TO
           || dep_pos==PENN_TAG_PRP
           || dep_pos==PENN_TAG_ADVERB
           || dep_pos==PENN_TAG_NOUN_PLURAL
           || dep_pos==PENN_TAG_NOUN_PROPER
           || dep_pos==PENN_TAG_WRB
           || dep_pos==PENN_TAG_CC
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_PDT
           || dep_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || dep_pos==PENN_TAG_ADVERB_COMPARATIVE
           || dep_pos==PENN_TAG_CD
           || dep_pos==PENN_TAG_EX
           || dep_pos==PENN_TAG_IN
           || dep_pos==PENN_TAG_WP_DOLLAR
           || dep_pos==PENN_TAG_MD
           || dep_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || dep_pos==PENN_TAG_ADJECTIVE_SUPERLATIVE
           || dep_pos==PENN_TAG_ADJECTIVE_COMPARATIVE
           || dep_pos==PENN_TAG_SYM
           || dep_pos==PENN_TAG_VERB
           || dep_pos==PENN_TAG_UH
         ) return false;
   }
   else if (label==PENN_DEP_SBAR) {
      if ( head_pos==PENN_TAG_PRP_DOLLAR
           || head_pos==PENN_TAG_VERB_PROG
           || head_pos==PENN_TAG_VERB_PAST
           || head_pos==PENN_TAG_VERB_PAST_PARTICIPATE
           || head_pos==PENN_TAG_COMMA
           || head_pos==PENN_TAG_R_QUOTE
           || head_pos==PENN_TAG_VERB_THIRD_SINGLE
           || head_pos==PENN_TAG_SHART
           || head_pos==PENN_TAG_RP
           || head_pos==PENN_TAG_DOLLAR
           || head_pos==PENN_TAG_FW
           || head_pos==PENN_TAG_POS
           || head_pos==PENN_TAG_PERIOD
           || head_pos==PENN_TAG_PRP
           || head_pos==PENN_TAG_L_BRACKET
           || head_pos==PENN_TAG_COLUM
           || head_pos==PENN_TAG_L_QUOTE
           || head_pos==PENN_TAG_LS
           || head_pos==PENN_TAG_PDT
           || head_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || head_pos==PENN_TAG_ADVERB_COMPARATIVE
           || head_pos==PENN_TAG_CD
           || head_pos==PENN_TAG_EX
           || head_pos==PENN_TAG_MD
           || head_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || head_pos==PENN_TAG_R_BRACKET
           || head_pos==PENN_TAG_ADJECTIVE_SUPERLATIVE
           || head_pos==PENN_TAG_ADJECTIVE_COMPARATIVE
           || head_pos==PENN_TAG_SYM
           || head_pos==PENN_TAG_VERB
           || head_pos==PENN_TAG_UH
           || dep_pos==PENN_TAG_PRP_DOLLAR
           || dep_pos==PENN_TAG_COMMA
           || dep_pos==PENN_TAG_R_QUOTE
           || dep_pos==PENN_TAG_WDT
           || dep_pos==PENN_TAG_WP
           || dep_pos==PENN_TAG_SHART
           || dep_pos==PENN_TAG_RP
           || dep_pos==PENN_TAG_DOLLAR
           || dep_pos==PENN_TAG_FW
           || dep_pos==PENN_TAG_POS
           || dep_pos==PENN_TAG_PERIOD
           || dep_pos==PENN_TAG_PRP
           || dep_pos==PENN_TAG_L_BRACKET
           || dep_pos==PENN_TAG_COLUM
           || dep_pos==PENN_TAG_L_QUOTE
           || dep_pos==PENN_TAG_WRB
           || dep_pos==PENN_TAG_CC
           || dep_pos==PENN_TAG_LS
           || dep_pos==PENN_TAG_PDT
           || dep_pos==PENN_TAG_ADVERB_SUPERLATIVE
           || dep_pos==PENN_TAG_ADVERB_COMPARATIVE
           || dep_pos==PENN_TAG_EX
           || dep_pos==PENN_TAG_WP_DOLLAR
           || dep_pos==PENN_TAG_NOUN_PROPER_PLURAL
           || dep_pos==PENN_TAG_R_BRACKET
           || dep_pos==PENN_TAG_SYM
           || dep_pos==PENN_TAG_UH
         ) return false;
   }
   // total number of rules are 438.
   return true;
}
#endif

inline const bool hasLeftHead(const unsigned &tag) {
   return true;
}

inline const bool hasRightHead(const unsigned &tag) {
   return true;
}
inline const bool canBeRoot(const unsigned &tag) {
   return true;
}
}
