#ifndef _DFA_C_



#include "DFA.h"

#include <string.h>
#include <stdlib.h>
#include <malloc.h>


//*Transition* createTransition(char value , int toStateId)
//{
//	Transition* transition = (Transition*)malloc(sizeof(Transition));
//	transition->value = value;;
//	transition->idState = toStateId;
//	return transition;
//}*//

void addTransition(DFA *dfa , int fromStateId , char value , int toStateId)
{
	if (dfa)
	{
		dfa->states[fromStateId].countTransitions++;
		dfa->states[fromStateId].transitions[hash(value)].idState = toStateId;//createTransition(value, toStateId);
		dfa->states[fromStateId].transitions[hash(value)].value = value;
		//dfa->states[fromStateId].transitions[hash(value)].defaultState = -1;
	}
	

}

State createState(int id)
{

	char stringBuffer[80] = "";
	State* state = (State*)malloc(sizeof(State));
	if (state)
	{
		state->id = id;
		state->isFinal = false;
		strcpy_s(state->name, (rsize_t)_countof(stringBuffer), "");
		state->idInArray = -1;
		state->countTransitions = 0;

	}
	return *state;
}
void addState(DFA * dfa, int id )
{
	char stringBuffer[80] = "";
	if(dfa->countStates==0)
	{ 
		dfa->states = (State*)malloc(sizeof(State));
	}
	else
	{
		dfa->states = (State*)realloc(dfa->states, sizeof(State) * (dfa->countStates + 1));
	}
	if (dfa->states)
	{
		dfa->states[id].countTransitions = 0;
		dfa->countStates++;
		dfa->states[id].id = id;
		dfa->states[id].isFinal = false;
		//dfa->states[id].token = "";
		strcpy_s(dfa->states[id].name, (rsize_t)_countof(stringBuffer), "");
		dfa->states[id].idInArray = -1;
		for (int i = 0; i < 100; i++) {
			dfa->states[id].transitions[i].idState = -1;
		}

		return;
	}

	
}

DFA* createDFA()
{
	DFA* dfa = (DFA*)malloc(sizeof(DFA));
	if (dfa)
	{
		dfa->startState = 0;
		dfa->currentState = 0;
		dfa->countStates = 0;
		dfa->states = (State*)malloc(sizeof(State));
		return dfa;
	}
	return NULL;
	
}

int hash(char value)
{
	return value - 32;
}

void moveDFA(DFA* dfa, char c)
{
	dfa->currentState = dfa->states[dfa->currentState].transitions[hash(c)].idState;

	//dfa->currentStateId = currentState->defaultToStateId; 
}



#endif // !_DFA_C_