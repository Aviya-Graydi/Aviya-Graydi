#ifndef _Parser_c_

#include <stdio.h>


//#include "Stack.c"
#include "AST.c"
//#include"CFG.c"
//#include"../LEXICAL/Token.h"

//#include"Stack.c"
#include"Parser.h"

#include <malloc.h>
#include <excpt.h>


void createParse(Parse* parse, int index, bool isCode, bool isTerminal)
{
	//parse = (Parse*)malloc(sizeof(Parse));
	if (parse)
	{
		parse->index = index;
		parse->isCode = isCode;
		parse->isTerminal = isTerminal;
	}
	//return parse;
}


void printStack()
{
	printf("\n");
	for (int i = 0; i <= parsingStack.top; i++)
	{
		if (((Parse*)(parsingStack.data[i]))->isCode == true)
		{
			if (((Parse*)(parsingStack.data[i]))->isTerminal == true)
			{
				if (codeTokens[((Parse*)(parsingStack.data[i]))->index].idTokenType == 2)
				{
					printf(" %s |", symbolTable[codeTokens[((Parse*)(parsingStack.data[i]))->index].id].name);
				}
				else
				{
					printf(" %s |", tokens[codeTokens[((Parse*)(parsingStack.data[i]))->index].id].value);
				}
			}
			else
			{
				printf(" %s |", nonTerminals[((Parse*)(parsingStack.data[i]))->index].name);
			}			
		}
		else
		{
			printf(" %d |", ((Parse*)(parsingStack.data[i]))->index);
		}		
	}
}


void printStack_AST()
{
	printf("\n");
	for (int i = 0; i <= AST_Stack.top; i++)
	{
		
		if (((AST*)(AST_Stack.data[i]))->isTerminal == true)
		{
			if (codeTokens[((AST*)(AST_Stack.data[i]))->id].idTokenType == 2)
			{
				printf(" %s |", symbolTable[codeTokens[((AST*)(AST_Stack.data[i]))->id].id].name);
			}
			else
			{
				printf(" %s |", tokens[codeTokens[((AST*)(AST_Stack.data[i]))->id].id].value);

			}
		}
		else
		{
			printf(" %s |", nonTerminals[((AST*)(AST_Stack.data[i]))->id].name);
		}

	}
}

int firstState()
{
	int t = parsingStack.top;
	while (((Parse*)(parsingStack.data[t]))->isCode == true)
	{
		t--;
	}
	return ((Parse*)(parsingStack.data[t]))->index;
}


void parser()
{
	char delimiter[] = " ";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה


	char* actionName;
	char tableValue[255] ="--";
	char value[255];

	int index = 0;
	int tokenId;
	int actionNumber;
	int actionNumber2;
	int x;
	int countValues;

	parsingStack = createStack();
	AST_Stack = createStack();

	AST* ast;
	AST* father;

	codeTokens = (Token*)realloc(codeTokens, sizeof(Token) *( sizeCodeTokens + 1));
	if (codeTokens)
	{
		createToken(&codeTokens[sizeCodeTokens++], sizeTokens - 1, 1, "<$>");
	}

	bool success = false;
	Parse* p;// = (Parse*)malloc(sizeof(Parse));
	//Stack* s = createStackValue( 0 , false , false);

	p = (Parse*)malloc(sizeof(Parse));
	createParse(p,0, false, false);
	push(&parsingStack ,p);
	printStack();
	
	int i = 0;
	int size =0;
	while(true)
	{
		if (codeTokens[index].idTokenType == 2)
		{
			tokenId = 0;
		}
		else 
		{
			tokenId = codeTokens[index].id;
		} 
		p = (Parse*)peek(&parsingStack);
		strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[p->index][tokenId].actionOrGOTO);
		
		actionName = strtok_s(tableValue, delimiter, &context);

		if (!strcmp(actionName, "<>"))
		{
			if (strcmp(actionAndGOTO_Table[p->index][1].actionOrGOTO, "<>"))
			{
				strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[p->index][1].actionOrGOTO);
				actionName = strtok_s(tableValue, delimiter, &context);
				//index--;
			}
		}

		if (!strcmp(actionName, "S"))//Shift
		{
			//s = createStackValue(index, true  , true);//הוספת האסימון למחסנית

			p = (Parse*)malloc(sizeof(Parse));
			createParse(p , index, true, true);
			push(&parsingStack, p);
			

			actionNumber =  atoi(strtok_s(NULL, delimiter, &context));

			//s = createStackValue(actionNumber, false ,false);//הוספת הפעולה בטבלה למחסנית

			p = (Parse*)malloc(sizeof(Parse));
			createParse(p ,actionNumber, false, false);
			push(&parsingStack , p);

			ast = (AST*)malloc(sizeof(AST));
			createAST(ast ,index ,true , 0 );
			push(&AST_Stack, ast);

			//if (codeAST == NULL)
			//{
			//	codeAST = (AST*)malloc(sizeof(AST));
			//	codeAST->id = index;
			//	codeAST->isTerminal = true;
			//	codeAST->numBrothers = 1;
			//	codeAST->numChildrens = 0;
			//	codeAST->childrens = NULL;
			//	//*codeAST = createAst(index, true, 1, 0, NULL , sizeof(AST));
			//}
			//else
			//{
			//	codeAST = createBrother(codeAST,index, true, 0, NULL);
			//}

			index++;//מעבר לאסימון הבא בקלט

		}
		else if(!strcmp(actionName, "R"))//Reduce
		{
			actionNumber = atoi(strtok_s(NULL, delimiter, &context));
			actionNumber2 = atoi(strtok_s(NULL, delimiter, &context));

			countValues = (nonTerminals[actionNumber].options[actionNumber2]->countValues);

			ast = (AST*)malloc(sizeof(AST));
			createAST(ast ,nonTerminals[actionNumber].id, false, countValues);

			for (int i = countValues -1; i >=0 ; i--)
			{
				 pop(&parsingStack);
				 pop(&parsingStack);
				 ast->childrens[i] = peek(&AST_Stack);
				 pop(&AST_Stack);
			}

			push(&AST_Stack, ast);

			x = firstState();

			//s = createStackValue(nonTerminals[actionNumber].id, true ,false);//הוספת כלל הגזירה למחסנית

			p = (Parse*)malloc(sizeof(Parse));
			createParse(p ,nonTerminals[actionNumber].id, true, false);
			push(&parsingStack , p);

			//printStack();

			strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[x][sizeTokens + actionNumber].actionOrGOTO);
			actionName = strtok_s(tableValue, delimiter, &context);//GOTO
			p = peek(&parsingStack);
			if (!strcmp(actionName, "<>"))
			{
				if (strcmp(actionAndGOTO_Table[p->index][1].actionOrGOTO, "<>"))
				{
					strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[p->index][1].actionOrGOTO);
					actionName = strtok_s(tableValue, delimiter, &context);
				}
			}
			if (strcmp(actionName, "<>"))
			{

				//הוספת GOTO למחסנית
				actionNumber = atoi(strtok_s(NULL, delimiter, &context));
				p = (Parse*)malloc(sizeof(Parse));
				createParse(p, actionNumber, false, false);
				push(&parsingStack, p);
				
			}
		}
		
		else if(!strcmp(actionName, "Accept"))//Accept
		{
			//free(stack);
			//printStack();
			success = true;
			break;
		}
		else
		{
			success = false;
			break;
			
		}
		printStack();
		printStack_AST();
		
		//printAST(ast);
		
	}

	if (success == true)
	{
		printf("\n\nThe code is correct!!!\n");
		codeAST = peek(&AST_Stack);
	}
	else
	{
		printf("\n\nThe code is incorrect :(\n");
	}
	printAST(codeAST);

}



#endif // !_Parser_c_