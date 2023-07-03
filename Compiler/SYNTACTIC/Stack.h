#pragma once

#include<stdbool.h>

typedef struct {
	void** data;
	int size;
	int top;
}Stack;



Stack parsingStack;
Stack AST_Stack;


Stack createStack();
void push(Stack* stack, void* element);
void pop(Stack* stack);
void* peek(Stack* stack);


