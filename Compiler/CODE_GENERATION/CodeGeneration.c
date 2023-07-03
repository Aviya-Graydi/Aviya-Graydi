
#include "CodeGeneration.h"
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include<stdbool.h>



int searchType(AST* ast)
{

	return 3;
}


int tab =2;
void addTab()
{
	for (int i = 0; i < tab; i++)
	{
		strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\t");
	}
}

void GeneratingTheCode()
{
	int identifier, type;
	countScope = (int*)malloc(sizeof(int));
	*countScope = 0;
	c = 0;
	push(&scopesStack,  countScope);
	//createScope(&scopes[*countScope], *countScope);

	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "public class Program { \n\tpublic static void main(string[] args){\n");
	Generate(codeAST);
	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\t}\n");
	//כאן לכתוב את הגדרות הפונקציות

	//for(int i = 0;)

	AST** a;

	strcat_s(codeGeneration, (rsize_t)_countof(codeFunctions), codeFunctions);



	//for (int i = 0; i < sizeFunctionsDefinitions; i++)
	//{
	//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\tpublic static ");
	//	identifier = searchIdentifier(functionsDefinitions[i]);
	//	type = symbolTable[identifier].idType;//??

	//	//strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), types[type].name);
	//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "dynamic");
	//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
	//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
	//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "(  )\n\t{\n");

	//	a = functionsDefinitions[i]->childrens;

	//	for (int j = 0; j < functionsDefinitions[i]->numChildrens; j++)
	//	{
	//		if (a[j]->isTerminal == false)
	//		{
	//			Generate(a[j]);
	//		}
	//	}

	//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\t}");

	//}

	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\n}");

	FILE* f;
	errno_t error = fopen_s(&f, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\code.cs", "w");
	if (error != 0)
	{
		printf("\nCould not open the file code :(");
		return;
	}
	fputs(&codeGeneration, f);
	fclose(f);
}

int printValueFlag = -1;
int assignmentValueFlag = -1;

void Generate(AST* ast)
{
	AST** a;
	int type;
	int identifier;
	AST* value;
	if (ast->isTerminal == false)
	{
		switch (ast->id)
		{
			case 0://Start
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "Public class Program { \n\tpublic static void main(string[] args){\n");
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					Generate(a[i]);
				}
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\t}"); break;
			case 1://Program
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					Generate(a[i]);
					if (a[i]->isTerminal==false &&a[i]->id == 10)
					{
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ";\n");
					}
				}break;
			case 2://IdentifierDefinition
				identifier = searchIdentifier(ast , -1);
				type = symbolTable[identifier].idType;

				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), types[type].name);
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ";");			
				break;

			case 3://Assignment
				
				addTab();

				identifier = searchIdentifier(ast , -1);

				type = symbolTable[identifier].idType;//לשנות על ידי מונה---לא צריך עשיתי dynamic
				if (canChange == true )
				{
					int flag = 0 , s , e;
					for ( e = scopesStack.size - 1 ; e >= 0 && e >= stopScopes-1 && flag == 0; e--)
					{
						s = *(int*)scopesStack.data[e];
						if (symbolTable[identifier].isGenerate[s] == true)
						{
							flag = 1;
						}
					}

					if (flag == 0) /* && symbolTable[identifier].isDefined*/
					{
						symbolTable[identifier].isGenerate[s] = true;
						if (type == 0)//??
						{
							type = 5;
						}
						//strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), types[type].name);
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "dynamic");
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
					}
				
				}
				
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " = ");
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					Generate(a[i]);
				}
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ";\n"); break;
			case 4://AssignmentValue

				
				if (printValueFlag != -1)
				{
					if (printValueFlag != 0 && assignmentValueFlag == 0)
					{
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "+ ' ' +");
					}
					printValueFlag++;
					assignmentValueFlag++;
				}
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (a[i]->isTerminal == true && codeTokens[a[i]->id].id != 2)//operator
					{
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), codeTokens[a[i]->id].value);
					}
					else
					{
						Generate(a[i]);
					}
				}
				
				break;
			case 5://Input
				
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "Console.ReadLine(");
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (a[i]->isTerminal == false)
					{
						
						Generate(a[i]);
					}

				}
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ")");

				break;
			case 6://Print
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "Console.WriteLine(");
				a = ast->childrens;
				printValueFlag = 0;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (a[i]->isTerminal == false)
					{	
											
						Generate(a[i]);	
						
					}
				}
				printValueFlag = -1;
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ");\n");
				break;
			case 7://Condition
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "if(");
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					//ביטוי תנאי
					if (a[i]->isTerminal == false && a[i]->id == 14)
					{
						
						Generate(a[i]);
					}
				}				
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ")\n");
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "{\n");
				tab++;
				//גוף התנאי
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (a[i]->isTerminal == false && a[i]->id != 14 && a[i]->id != 23 && a[i]->id != 24)
					{
						Generate(a[i]);
					}
				}
				tab--;
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "}\n");
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (a[i]->isTerminal == false &&(  a[i]->id == 23 || a[i]->id == 24))
					{
						Generate(a[i]);
					}
					
				}

				break;
			case 8://Loop
				a = ast->childrens;
				countScope = (int*)malloc(sizeof(int));
				*countScope = ++c;
				push(&scopesStack, countScope);
				//createScope(&scopes[*countScope], *countScope);
				for (int i = 0; i < ast->numChildrens; i++)
				{
					Generate(a[i]);
				}
				pop(&scopesStack);
				break;
			case 9://FunctionDefinition
				countScope = (int*)malloc(sizeof(int));
				*countScope = ++c;
				push(&scopesStack,  countScope);
				//createScope(&scopes[*countScope],*countScope);
				stopScopes = *countScope;

				strcpy_s(codeCopy, (rsize_t)_countof(codeCopy), codeGeneration);
				strcpy_s(codeGeneration, (rsize_t)_countof(codeCopy), "");
				
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\tpublic static ");
				identifier = searchIdentifier(ast, -1);
				type = symbolTable[identifier].idType;

				//strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), types[type].name);
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "dynamic");
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "(  )\n\t{\n");

				a = ast->childrens;

				for (int j = 0; j < ast->numChildrens; j++)
				{
					if (a[j]->isTerminal == false)
					{
						Generate(a[j]);
					}
				}

				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "\t}");

				strcat_s(codeFunctions, (rsize_t)_countof(codeCopy), codeGeneration);
				strcpy_s(codeGeneration, (rsize_t)_countof(codeCopy), codeCopy);


				/*if (sizeFunctionsDefinitions == 0)
				{
					functionsDefinitions = (AST**)malloc(sizeof(AST*));
				}
				else
				{
					functionsDefinitions = (AST**)realloc(functionsDefinitions , sizeof(AST*)*( sizeFunctionsDefinitions +1));
				}
				functionsDefinitions[sizeFunctionsDefinitions++] = ast;	*/	
				pop(&scopesStack);
				stopScopes = 0;

				break;
			case 10://FunctionCalling
				addTab();
				identifier = searchIdentifier(ast,-1);
				
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "()");
			
				break;
			case 11://T
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (a[i]->isTerminal == true)
					{
						//strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), tokens[a[i]->id].value);
					}
					else
					{
						Generate(a[i]);
					}
				}
				break;
			case 12://Value
				
				a = ast->childrens;
				if (a[0]->isTerminal == true)
				{
					if (codeTokens[a[0]->id].idTokenType == 2)
					{
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[codeTokens[a[0]->id].id].name);
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
					}
					else
					{
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), constant[currentConstant++].value);						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
					};
				}
				else
				{
					if (a[0]->id == 10)
					{
						Generate(a[0]);
						

					}
				} break;
				
			//case 13://PrintValue
			//	a = ast->childrens;
			//	for (int i = 0; i < ast->numChildrens; i++)
			//	{
			//		if (a[i]->isTerminal == false)
			//		{
			//			
			//			Generate(a[i]);
			//			
			//		}
			//	}
			//	break;
			case 14://ConditionalExpression
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (a[i]->isTerminal == true && codeTokens[a[i]->id].idTokenType == 3)
					{
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), codeTokens[a[i]->id].value);
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
					}
					else if (a[i]->isTerminal == true && codeTokens[a[i]->id].idTokenType == 4)
					{
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), tokens[a[i]->id].value);
						strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
					}
					else
					{
						Generate(a[i]);
					}
				}
				break;
			case 16:
				a = ast->childrens;
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), codeTokens[a[0]->id].value);
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " ");
				break;
			case 18://For
				addTab();
				identifier = searchIdentifier(ast,-1);
				if (constant[currentConstant].idType == 3)//int
				{
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "for(int ");
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " = 0; ");
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "<");
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), constant[currentConstant++].value);
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "; ");
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "++)\n");
				}
				else//==5 - string
				{
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "foreach(char ");
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), symbolTable[identifier].name);
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), " in ");
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), constant[currentConstant++].value);
					strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ")\n");
				}


				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "{\n");
				tab++;
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					//גוף הלולאה
					if (a[i]->isTerminal == false)
					{
						Generate(a[i]);
					}
				}
				tab--;
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "}\n");

				break;
			case 19://While
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "while(");
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					//ביטוי תנאי
					if (a[i]->isTerminal == false && a[i]->id == 14)
					{
						Generate(a[i]);
					}
				}
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ")\n");
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "{\n");
				tab++;
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					//גוף הלולאה
					if (a[i]->isTerminal == false && a[i]->id != 14)
					{
						Generate(a[i]);
					}
				}
				tab--;
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "}\n");

				break;
			case 23://Elif
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "else if(");

				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					//ביטוי תנאי
					if (a[i]->isTerminal == false && a[i]->id == 14)
					{

						Generate(a[i]);
					}
				}
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), ")\n");
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "{\n");

				tab++;

				//גוף התנאי
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (!(a[i]->isTerminal == false && a[i]->id != 14))
					{
						//לסדר elif , else
						//Generate(a[i]);
					}
					else
					{
						Generate(a[i]);
					}
				}
				tab--;
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "}\n");


				break;
			case 24://Else
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "else\n");
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "{\n");

				tab++;

				//גוף התנאי בינתיים פרינט
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					if (!(a[i]->isTerminal == false && a[i]->id != 14))
					{
						//לסדר elif , else
						//Generate(a[i]);
					}
					else
					{
						Generate(a[i]);
					}
				}
				tab--;
				addTab();
				strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), "}\n");


				break;
			default:
				a = ast->childrens;
				for (int i = 0; i < ast->numChildrens; i++)
				{
					
					if (a[i]->isTerminal == false)
					{
						if (a[i]->id == 4)
						{
							assignmentValueFlag = 0;
						}
						Generate(a[i]);
						if (a[i]->id == 4)
						{
							assignmentValueFlag = -1;
						}
					}
					
					
				}
		}
		
	}

	else {
		//if (tokens[ast->id].idTokenType == 1)//operator
		//{
		//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), tokens[ast->id].value);
		//}
		//else if (tokens[ast->id].idTokenType == 2)//identifier
		//{

		//}
		//else if (tokens[ast->id].idTokenType == 3)//constant
		//{
		//	strcat_s(codeGeneration, (rsize_t)_countof(codeGeneration), constant[currentConstant++].value);
		//}
	}
}