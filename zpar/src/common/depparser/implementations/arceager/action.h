#ifndef DEPPARSER_ACTION_H
#define DEPPARSER_ACTION_H

namespace action {

#ifdef DEPENDENCIES
// SH RE AL AR PR [AL+LABEL] AR+LABEL
enum STACK_ACTION { NO_ACTION=0, SHIFT, REDUCE, ARC_LEFT, ARC_RIGHT, POP_ROOT, LABEL_AL_FIRST, LABEL_AR_FIRST=LABEL_AL_FIRST+DEP_LABEL_COUNT-1, MAX=LABEL_AR_FIRST+DEP_LABEL_COUNT-1 };
#endif

#ifdef LINKS
enum LINK_ACTION { NO_ACTION=0, SHIFT, REDUCE, CONNECT_LEFT, CONNECT_RIGHT, POP_ROOT, LABEL_CL_FIRST, LABEL_CR_FIRST=LABEL_CL_FIRST+DEP_LABEL_COUNT-1, MAX=LABEL_CR_FIRST+DEP_LABEL_COUNT-1};
#endif

#ifdef LABELED
#ifdef DEPENDENCIES
static unsigned encodeAction(const STACK_ACTION &action, const unsigned &label=0) {
   assert(label<DEP_LABEL_COUNT);
   if (action==ARC_LEFT) {
      if (label==0)
         return ARC_LEFT;
      else
         return LABEL_AL_FIRST+label-1;
   }
   else if (action==ARC_RIGHT) {
      if (label==0)
         return ARC_RIGHT;
      else
         return LABEL_AR_FIRST+label-1;
   }
   else
      assert(label==0);
   return action;
}
static unsigned getUnlabeledAction(const unsigned &action ) {
   assert(action<MAX);
   if (action<LABEL_AL_FIRST)
      return action;
   else if (action<LABEL_AR_FIRST)
      return ARC_LEFT;
   else
      return ARC_RIGHT;
}
static unsigned getLabel(const unsigned &action) {
   assert(action<MAX);
   if (action < LABEL_AL_FIRST)
      return 0;
   else if (action < LABEL_AR_FIRST)
      return action-LABEL_AL_FIRST+1;
   else
      return action-LABEL_AR_FIRST+1;
}

#endif


#ifdef LINKS
static unsigned encodeAction(const LINK_ACTION &action, const unsigned &label=0) {
   assert(label<DEP_LABEL_COUNT);
   if (action==CONNECT_LEFT) {
      if (label==0)
         return CONNECT_LEFT;
      else
         return LABEL_CL_FIRST+label-1;
   }
   else if (action==CONNECT_RIGHT) {
      if (label==0)
         return CONNECT_RIGHT;
      else
         return LABEL_CR_FIRST+label-1;
   }
   else
      assert(label==0);
   return action;
}
static unsigned getUnlabeledAction(const unsigned &action ) {
   assert(action<MAX);
   if (action<LABEL_CL_FIRST)
      return action;
   else if (action<LABEL_CR_FIRST)
      return CONNECT_LEFT;
   else
      return CONNECT_RIGHT;
}
static unsigned getLabel(const unsigned &action) {
   assert(action<MAX);
   if (action < LABEL_CL_FIRST)
      return 0;
   else if (action < LABEL_CR_FIRST)
      return action-LABEL_CL_FIRST+1;
   else
      return action-LABEL_CR_FIRST+1;
}


#endif

#endif


struct CScoredAction {
   unsigned action;
   SCORE_TYPE score;
public:
   bool operator < (const CScoredAction &a) const {
      return score < a.score;
   }
   bool operator > (const CScoredAction &a) const {
      return score > a.score;
   }
};

}//namespace
#endif
