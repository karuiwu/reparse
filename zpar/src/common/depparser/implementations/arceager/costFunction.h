/*
 * costFunction.h
 *
 *  Created on: Nov 7, 2013
 *      Author: juneki
 */

#ifndef COSTFUNCTION_H_
#define COSTFUNCTION_H_


class State;
class AutomataAction;

class costFunction{
	std::map<State,SCORE_TYPE> transitionFunction;
	std::map<State,SCORE_TYPE> takeParentFunction;
	std::map<State,SCORE_TYPE> flipFunction;
	std::map<State,SCORE_TYPE> haltFunction;
public:
	costFunction(){
		//	TRANSITION = 1, TAKEPARENT = 2, FLIP = 3, HALT = 4, NULLACTION
	}
	costFunction(std::string fileName){
		//TODO read in a file and populate the functions
	}
	~costFunction(){
	}


	SCORE_TYPE getCost(AutomataAction action, State s){


		SCORE_TYPE cost = 0;
		cost = s.leftParent;
		cost = 0;

		switch(action){
		case TRANSITION:
//			cost = transitionFunction[tempS];
//			cost = transitionFunction[*s];
			break;
		case TAKEPARENT:
//			cost = takeParentFunction[*s];
			break;
		case FLIP:
//			cost = flipFunction[*s];
			break;
		case HALT:
//			cost = haltFunction[*s];
			break;
		}
		return cost;
	}

};



#endif /* COSTFUNCTION_H_ */
