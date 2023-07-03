#include<malloc.h>

#include"AST.h"
#include "../LEXICAL/FillDFA.C"

void createAST(AST* ast ,int id, bool isTerminal, int numChildrens)
{
	//AST* ast = (AST*)malloc(sizeof(AST));
	if (ast)
	{
		ast->id = id;
		ast->isTerminal = isTerminal;
		ast->numChildrens = numChildrens;
		ast->idType = 0;//O.K.
		ast->childrens = (AST**)malloc(sizeof(AST*) * numChildrens);
		
	}
	//return ast;
}


AST* createFather( int id, bool isTerminal ,int numNewChildrens)
{
	//int numNewChildrens = nonTerminals[id].options[]
	AST* father = (AST*)malloc(sizeof(AST));
	createAST(father ,id, isTerminal, numNewChildrens);
	return father;
}

void printAST(AST* ast)
{
	if (ast == NULL)
	{
		return;
	}
	if (ast->isTerminal == true)
	{
		if (codeTokens[ast->id].idTokenType == 2)
		{
			printf("\n %s . ", symbolTable[codeTokens[ast->id].id].name);
		}
		else
		{
			printf("\n %s . ", tokens[codeTokens[ast->id].id].value);

		}
	}
	else
	{
		printf("%s -> ", nonTerminals[ast->id].name);
	}
	for (int i = 0; i < ast->numChildrens; i++)
	{		
		
		printAST(ast->childrens[i]);

	}
}