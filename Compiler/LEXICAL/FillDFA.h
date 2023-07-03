#ifndef _FillDFA_H_


#include<stdio.h>
#include "DFA.h"
//#include "Token.c"

//void fillFinalTransitions(DFA* dfa);
void fillRegularExpressions(DFA* dfa);
void fillDFA(DFA* dfa , char* fileName);
DFA* dfa(char* fileName);

//----------------------------------
void fillDFA1(DFA* dfa);
void finalStates(DFA* dfa);

#endif // !_FillDFA_H_
