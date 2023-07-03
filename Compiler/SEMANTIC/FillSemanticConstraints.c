
#include<stdio.h>
#include"FillSemanticConstraints.h"

//sizeTypes = 10;

//char type[255] = "O.K.";
bool isAssignment = false;
int* idVariable = NULL;//אינדקס בטבלת הסמלים
int sizeIdVariables = 0;
int idTypeVariable;
bool firstIdentifier = true;



void fillTables(DFA* tokensDFA)
{

	//קריאה מקובץ

	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = "\t";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת

	int valueId = 0;
	int nextState;
	int idOperator = 0;

	char* name = '\0';

	FILE* f;

	//errno_t error = fopen_s(&f3, "X:\\סמינר\\התמחויות\\הנדסת תוכנה\\שנה ב\\תלמידות\\אביה גריידי\\פרוייקט\\פרוייקט\\kinds.csv", "r");
	errno_t error = fopen_s(&f, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\tables.txt", "r");
	if (error != 0)
	{
		printf("\nCould not open the file tables :(");
		return;
	}

	printf("\nThe file tables was opened successfully !!! :)");
	
	fgets(&line, 255, f);
	slot = strtok_s(line, delimiter, &context);//משבצת
	canChange = ! atoi(slot);
	
	while (fgets(&line, 255, f))//הלולאה עוברת שורה שורה
	{

		//fscanf_s(f, "%s", &line, 255);//שורה מהקובץ
		//fgets(&line, 255, f);
		slot = strtok_s(line, delimiter, &context);//משבצת

		printf("\n%d" , sizeOperatorsTable);

		//שורה ראשונה = האופרטורים המתאימים לטבלה
		while (slot != NULL && strcmp(slot, "\n"))
		{
			printf("%s ", slot);

			tokensDFA->currentState = 0;
			while (*slot)
			{
				//זיהוי האופרטור
				//לסמן לכל אופרטור מה המיקום שלו בטבלה

				if (tokensDFA->states[tokensDFA->currentState].transitions[hash(*slot)].idState == -1)
				{
					printf("The operator is not recognized");
					break;
				}
				nextState = tokensDFA->states[tokensDFA->currentState].transitions[hash(*slot)].idState;
				slot++;
				tokensDFA->currentState = nextState;
			}

			if (tokensDFA->states[tokensDFA->currentState].isFinal == true)
			{
				tokens[tokensDFA->states[tokensDFA->currentState].idInArray].idType = idOperator;
			}

			slot = strtok_s(NULL, delimiter, &context);

		}
		if (sizeOperatorsTable == 0)
		{
			operatorsTable = (int***)malloc(sizeof(int**));			
		}
		else
		{
			operatorsTable = (int***)realloc(operatorsTable , sizeof(int**)*(sizeOperatorsTable + 1));
		}
		operatorsTable[sizeOperatorsTable] = (int**)malloc(sizeof(int*) * sizeTypes);
		for (int i = 0; i < sizeTypes; i++)
		{
			operatorsTable[sizeOperatorsTable][i] = (int*)malloc(sizeof(int) * sizeTypes);
		}
		//השורות הבאות == האילוצים הסמנטיים - הביטויים התקינים לכל אופרטור
		for (int i = 0; i < sizeTypes; i++)
		{
			printf("\n");
			fgets(&line, 255, f);
			slot = strtok_s(line, delimiter, &context);//משבצת

			for (int j = 0; j < sizeTypes; j++)
			{
				operatorsTable[sizeOperatorsTable][i][j] = atoi(slot);
				slot = strtok_s(NULL, delimiter, &context);//משבצת
				printf("%d ", operatorsTable[sizeOperatorsTable][i][j]);
			}			
		}	
		sizeOperatorsTable++;
	}
	fclose(f);	
}


//מעדכנים את הטיפוס של האיבר הראשון - האיבר שאליו עשינו את ההשמה להיות תוצאה של ביטוי ערך ההשמה
int updateAssignmentValue(AST* ast)
{
	int i = 0;
	int operatorId = 0;
	int L;
	int R;
	int type;
	int s;//scope
	int e;//element in the stack
	int flag = 0;
	if (ast->isTerminal == true)
	{
		if (codeTokens[ast->id].idTokenType == 2)
		{
			for (e = scopesStack.size - 1; e >= 0 && e >= stopScopes - 1 && flag == 0; e--)
			{
				s = *(int*)scopesStack.data[e];
				if (symbolTable[codeTokens[ast->id].id].correctScopes[s] == true)
				{
					flag = 1;
				}
			}
			//if (scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isDefined == false || scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isInitialis == false)
			if(flag == 0)
			{
				return 1;//err
			}
			if (symbolTable[codeTokens[ast->id].id].isFunc == true)
			{
				return 0;
			}
			return scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].idTypes[scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].count-1];
		}
		if (tokens[codeTokens[ast->id].id].idType == 1)
		{
			printf("\n\n1\n\n");
		}
		return tokens[codeTokens[ast->id].id].idType;// ast->idType;
	}
	AST** a = ast->childrens;
	type = updateAssignmentValue(a[i]);
	a[i]->idType = type;
	i++;
	for (; i < ast->numChildrens; i++)
	{
		if (a[i]->isTerminal == true)
		{
			//....אם זה אופרטור לשנות את מזהה האופרטור ולבדוק איתו את האחים
			if (a[i]->idType == 3)
			{
				operatorId = tokens[a[i]->id].idType;
			}
			
			a[i]->idType = updateAssignmentValue(a[i]);
		
			tokens[ast->id].idType = a[i]->idType;
			
		}
		else
		{
			a[i]->idType = updateAssignmentValue(a[i]);
		}
		type = operatorsTable[operatorId][a[i - 1]->idType][a[i]->idType];
	}	
	if (type == 1)
	{
		printf("\n\n1\n\n");
	}
	return type;
}

void updateVariableAssignment(AST* ast)
{
	int type;
	if (ast->isTerminal == true)
	{
		if (codeTokens[ast->id].idTokenType == 2)//שם המשתנה
		{
			if (firstIdentifier)
			{
				firstIdentifier = false;

				if (canChange == true)
				{
					//symbol->idTypes = NULL;
					//symbol->count = 0;

					idVariable = (int*)malloc(sizeof(int));
					idVariable[sizeIdVariables] = codeTokens[ast->id].id;
					int* p = (int*)peek(&scopesStack);
					scopes[*p].symbols[idVariable[sizeIdVariables]].isDefined = true;

					if (scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables]].count == 0)
					{
						scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables]].idTypes = (int*)malloc(sizeof(int));
					}
					else
					{
						scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables]].idTypes = (int*)realloc(scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].idTypes, sizeof(int) * (scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables]].count + 1));
					}
					scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables]].isInitialis = true;
					
					scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables]].count++;
					symbolTable[idVariable[sizeIdVariables++]].correctScopes[*(int*)peek(&scopesStack)] = true;
					//symbolTable[tokens[ast->id].id].idTypes[symbolTable[tokens[ast->id].id].count++] = ??צריך להתעדכן פה או בפו' של הסריקה
					//צריך לעדכן את הטיפוס של האיבר הראשון. מה הטיפוס שלו? תוצאה של כל ביטוי ההשמההה
					//איך מחשבים את תוצאת טיפוס ביטוי ההשמה?????????..........
					//את זה נעשה לאחר הבדיקה שכל המשתנים בהמשך תקינים - מוגדרים ומאותחלים
					//symbolTable[tokens[ast->id].id].idTypes[symbolTable[tokens[ast->id].id].count++] = updateAssignmentValue();
				}

			}
			//else //if (canChange == false)
			//{
			//	//הגעתי לכאן בגלל שהיתה השמה בתוך הגדרה או שאלו שאר המשתנים בתוך השמה שהיא גם הגדרה!!
			//	//ולכן - לא צריך לסמן שהמשתנה מוגדר אלא רק לבדוק את ההמשך - שהמשתנים ששמים מוגדרים בכלל
			//	//אם כן - ממשיכים הלאה. אם לא צריך לסמן שיש שגיאה של שימוש במשתנה לא מאותחל
			//	if (symbolTable[codeTokens[ast->id].id].isDefined == false)
			//	{
			//		//strcpy(type, "err!! identifier "+ symbolTable[tokens[ast->id].id].name+"is not def");לסמן שגיאהההה

			//		return;
			//	}
			//}

		}
	}
	else if (ast->id == 4 && canChange == true)//AssignmentValue ערך ההשמה למשתנה
	{
		if (isAssignment == true)
		{
			isAssignment == false;
			type = updateAssignmentValue(ast);
			if (type != 1)
			{
				//symbolTable[idVariable[sizeIdVariables-1]].isInitialis = true;
				scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables - 1]].idTypes[scopes[*(int*)peek(&scopesStack)].symbols[idVariable[sizeIdVariables - 1]].count-1] = type;

			}

			//רק אם אני בתוך השמה צרים לעדכן את הטיפוס של L להיות הטיפוס של המשתנה
			//L = symbolTable[idVariable[sizeIdVariables]].idTypes[symbolTable[idVariable[sizeIdVariables]].count++];
		}
	}
	AST** a = ast->childrens;
	for (int i = 0; i < ast->numChildrens; i++)
	{

		updateVariableAssignment(a[i]);
		//if (a[i]->isTerminal == false && a[i]->id == 4)//AssignmentValue
		//{
		//	updateAssignmentValue(a[i]);
		//}
	}
}



void updateVariableDefinition(AST* ast)
{
	if (ast->isTerminal == true)
	{
		if (codeTokens[ast->id].idTokenType == 2)//המשתנה
		{
			if (scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isDefined == false)//רק אם המשתנה לא הוגדר בעבר
			{
				if (sizeIdVariables == 0)
				{
					scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isDefined = true;
					idVariable = (int*)malloc(sizeof(int));
					idVariable[sizeIdVariables++] = codeTokens[ast->id].id;
				}
				else//אם מאפשרים להגדיר כמה בחד באותו שורה של טיפוס
				{
					scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isDefined = true;
					idVariable = (int*)realloc(idVariable, sizeof(int) * (sizeIdVariables + 1));
					idVariable[sizeIdVariables++] = codeTokens[ast->id].id;
				}
			}
		}
		else if (tokens[ast->id].idTokenType == 6)//טיפוס
		{
			idTypeVariable == types[codeTokens[ast->id].idType].id;
		}
	}
	AST** a = ast->childrens;
	for (int i = 0; i < ast->numChildrens; i++)
	{
		
		if (a[i]->isTerminal && a[i]->id == 3 && canChange == true)//Assignment השמה למשתנה
		{
			updateVariableAssignment(a[i]);
		}
		updateVariableDefinition(a[i]);
	}
}
int searchIdentifier(AST* ast , int id)
{
	if (ast->isTerminal == true)
	{
		if (codeTokens[ast->id].idTokenType == 2)
		{
			return codeTokens[ast->id].id;
		}
		return -1;
	}
	else
	{
		AST** a = ast->childrens;
		int i = 0;
		id = searchIdentifier(a[i] , id);
		i++;
		for (; i < ast->numChildrens; i++)
		{
			if (id != -1)
			{
				return id;
			}
			id = searchIdentifier(a[i]  , id);
			
		}
		return id;
	}
}

int typesCheck(AST* ast)
{
	int i = 0;
	int operatorId = 0;
	int L;
	int R;
	int type;
	AST** a = NULL;
	int s;
	int element;
	int e;
	int flag = 0;
	int identifier;
	if (ast->isTerminal == true)
	{

		if (codeTokens[ast->id].idTokenType == 2)
		{
			printf("\n %s . ", symbolTable[codeTokens[ast->id].id].name);
			for (e = scopesStack.size - 1; e >= 0 /*&& e >= stopScopes-1*/ && flag == 0; e--)
			{
				s = *(int*)scopesStack.data[e];
				if (symbolTable[codeTokens[ast->id].id].correctScopes[s] == true)
				{
					flag = 1;
				}
			}

			//if (symbolTable[codeTokens[ast->id].id].isDefined == false || symbolTable[codeTokens[ast->id].id].isInitialis == false)
			if (flag == 0)
			{
				return 1;//err
			}
			//כאן לבדוק האם הוא מוגדר ומאותחל
			//אני אגיע לכאן וודאי אחרי הגדרה והשמה!!
			if (symbolTable[codeTokens[ast->id].id].isFunc == true)
			{
				return 0;
			}
			return scopes[s].symbols[codeTokens[ast->id].id].idTypes[scopes[s].symbols[codeTokens[ast->id].id].count - 1];
		}
		else
		{
			printf("\n %s . ", tokens[codeTokens[ast->id].id].value);
		}
		ast->idType = codeTokens[ast->id].idType;
		return ast->idType;
	}

	///לבדוקקק הגדרההה
	if (ast->id == 2)//IdentifierDefinition הגדרת משתנה
	{
		free(idVariable);
		sizeIdVariables = 0;
		updateVariableDefinition(ast);
		for (int j = 0; j < sizeIdVariables; j++)
		{
			//צריך את שני השורות האלו??
			scopes[*(int*)peek(&scopesStack)].symbols[idVariable[j]].idTypes = (int*)malloc(sizeof(int));
			scopes[*(int*)peek(&scopesStack)].symbols[tokens[ast->id].id].idTypes[scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].count++] = idTypeVariable;

			symbolTable[idVariable[j]].idType = idTypeVariable;
		}
	}
	if (ast->id == 3 /*&& canChange == true*/)//Assignment השמה למשתנה
	{
		isAssignment = true;
		firstIdentifier = true;
		free(idVariable);
		sizeIdVariables = 0;
		updateVariableAssignment(ast);
	}
	if (ast->id == 8 || ast->id == 9)//Loop | FunctionDefinition לולאה או הגדרת פונקציה
	{
		countScope = (int*)malloc(sizeof(int));
		*countScope = ++c;
		scopes = (Scope*)realloc(scopes, sizeof(Scope) * (*countScope+1));
		createScope(&scopes[*countScope], *countScope);
		push(&scopesStack, countScope);

		/*if (ast->id == 8)
		{
			stopScopes = *countScope;
		}*/
		if (ast->id == 9)
		{
			identifier = searchIdentifier(ast,-1);
			scopes[0].symbols[identifier].isDefined = true;
			scopes[0].symbols[identifier].isInitialis = true;
			symbolTable[identifier].correctScopes[0] = true;
			symbolTable[identifier].isFunc= true;
		}
	}
	if (ast->id == 10 && canChange == false)//FunctionCall קריאה לפונקציה
	{
		identifier = searchIdentifier(ast,-1);//...להמשיך

	}
	a = ast->childrens;
	ast->idType = typesCheck(a[i]);
	L = ast->idType;
	i++;
	
	
	//להעתיק גם לכאן את הבדיקות של השמה והגדרה!!
	//L-הטיפוס של המשתנה שווה ל
	
	for (; i < ast->numChildrens; i++)
	{		
		if (a[i]->isTerminal == true)
		{		
			//....אם זה אופרטור לשנות את מזהה האופרטור ולבדוק איתו את האחים
			if (a[i]->idType == 3)
			{
				operatorId = tokens[a[i]->id].idType;//איך מוצאים את המזהה של האופרטור?????//איך יודעים מהו האופרטור???????
			}
		}	
		a[i]->idType = typesCheck(a[i]); 
		R = a[i]->idType;
		L = operatorsTable[operatorId][L][R];

	}
	//printf("\n%s ", nonTerminals[ast->id].name);
	if (ast->id == 8 || ast->id == 9)//Loop || FunctionDefinition לולאה או הגדרת פונקציה
	{
		pop(&scopesStack);
		if (ast->id == 9)
		{
			stopScopes = 0;
		}
	}
	return L;//גם אם יש בן אחד

	//אם זה הגדרה או השמה צריל לסמן את הטיפוס של המזהה 
	//עדכון טבלת הסמלים
}




//האיבר הראשון מסומן כמוגדר ומבצעים בדיקה שגם שאר המשתנים בערך ההשמה מוגדרים ומאותחלים





void functionsCheck()
{
	
}

void semanticCheck()
{
	countScope = (int*)malloc(sizeof(int));
	*countScope = c;
	scopes = (Scope*)malloc(sizeof(Scope));
	createScope(&scopes[*countScope], *countScope);
	scopesStack = createStack();
	
	
	push(&scopesStack, countScope);
	int result = typesCheck(codeAST);
	pop(&scopesStack);
	printf("\n\n%s", types[result].name);
	if (result != 1)
	{
		printf("\n\nCorrect semantics!!");
	}
	else
	{
		printf("\n\nIncorrect semantics...:(");
	}
	
}

