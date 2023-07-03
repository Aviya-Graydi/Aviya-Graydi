#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



typedef struct{
	int id;//אינדקס במערך הקוד / הנונ-טרמינלים
	int numChildrens;
	bool isTerminal;
	int idType;
	struct AST** childrens;
}AST;


AST* codeAST;

void createAST(AST*  ast ,int id , bool isTerminal, int numChildrens , AST** childrens );
AST* createFather(AST* childrens, int id, bool isTerminal);
AST* createBrother(AST* ast, int id, bool isTerminal, int numChildrens, AST* childrens);
void printAST(AST* ast);