#pragma once

#include "Stack.c"

typedef struct {
	bool isCode;
	bool isTerminal;
	int index;

}Parse;


//Stack* createStackValue(int id, bool isToken);

void createParse(Parse*  parse ,int index, bool isCode, bool isTerminal);


void printStack();
int firstState();

void parser();
