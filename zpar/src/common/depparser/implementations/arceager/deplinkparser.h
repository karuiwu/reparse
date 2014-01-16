//Dependency Link Parser
//Juneki Hong 2014
//juneki@jhu.edu

#ifndef _DEPLINKPARSER_H
#define _DEPLINKPARSER_H

#include "depparser.h"
#include "state.h"

namespace TARGET_LANGUAGE {

class CDepLinkParser: public CDepParser {

public:
	CDepLinkParser(const std::string &sFeatureDBPath, bool bTrain, bool bCoNLL =
			false, bool bLinks = false) :
			CDepParser(sFeatureDBPath, bTrain, bCoNLL) {
	}

public:
	void parse_links(const LinkInput &sentence, LinkOutput *retval, int nBest =
			1, depparser::SCORE_TYPE * scores = 0);
	void train_links(const LinkOutput & correct, int round);

protected:
	template<typename LinkInputOrOutput>
	void initLinkCache(const LinkInputOrOutput &sentence);

//#ifdef LINKS
	void work(const bool bTrain, const CTwoStringVector &sentence,
			CDependencyLinkParse *retval, const CDependencyLinkParse &correct,
			int nBest, depparser::SCORE_TYPE *scores);

	void connectleft(const depparser::CStateItem *item,
			const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores);

	void connectright(const depparser::CStateItem *item,
			const CPackedScoreType<depparser::SCORE_TYPE, depparser::action::MAX> &scores);
//#endif

};

}
;

#endif
