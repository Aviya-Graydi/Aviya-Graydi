#ifndef _DFA_H_

#include<stdio.h>




#include<stdbool.h>

#define _DFA_H_


typedef struct {
	char value;
	int idState;
	//int defaultState;
}Transition;

typedef struct {
	int id;
	bool isFinal;
	char name[255];
	int idInArray;
	int countTransitions;
	Transition transitions[100];
}State;

typedef struct  {
	int startState;
	int currentState;
	int countStates;
	State* states;
}DFA ;

//
//DFA* tokensDFA;
//DFA* CFG_DFA;



//Transition* createTransition(char value, int toStateId);
void addTransition(DFA *dfa, int fromStateId, char value, int toStateId);
State createState(int id);
void addState(DFA *dfa, int id);
DFA* createDFA();
int hash(char value);
void moveDFA(DFA* dfa, char c);


#endif // !_DFA_H_


