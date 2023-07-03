
#pragma once

#include "../SYNTACTIC/Parser.c"


//int* operatorsTable[sizeTypes][sizeTypes] = NULL;
int*** operatorsTable = NULL;

int sizeOperatorsTable = 0;

//int* scopesArr;
int c = 0;
int* countScope; 
Stack scopesStack;

int stopScopes = 0;

int* functionsCalls;

void semanticCheck();

void fillTables(DFA* tokensDFA);
int typesCheck(AST* ast);
void functionsCheck();