
#include<stdio.h>
#include"FillSemanticConstraints.h"

//sizeTypes = 10;

//char type[255] = "O.K.";
bool isAssignment = false;
int* idVariable = NULL;//������ ����� ������
int sizeIdVariables = 0;
int idTypeVariable;
bool firstIdentifier = true;



void fillTables(DFA* tokensDFA)
{

	//����� �����

	char line[255] = "line";//���� ������
	char delimiter[] = "\t";//���� �����
	char* context = NULL;//���� ����� ������
	char* slot = NULL;//�����

	int valueId = 0;
	int nextState;
	int idOperator = 0;

	char* name = '\0';

	FILE* f;

	//errno_t error = fopen_s(&f3, "X:\\�����\\��������\\����� �����\\��� �\\�������\\���� ������\\�������\\�������\\kinds.csv", "r");
	errno_t error = fopen_s(&f, "D:\\����\\����� �����\\�������\\tables.txt", "r");
	if (error != 0)
	{
		printf("\nCould not open the file tables :(");
		return;
	}

	printf("\nThe file tables was opened successfully !!! :)");
	
	fgets(&line, 255, f);
	slot = strtok_s(line, delimiter, &context);//�����
	canChange = ! atoi(slot);
	
	while (fgets(&line, 255, f))//������ ����� ���� ����
	{

		//fscanf_s(f, "%s", &line, 255);//���� ������
		//fgets(&line, 255, f);
		slot = strtok_s(line, delimiter, &context);//�����

		printf("\n%d" , sizeOperatorsTable);

		//���� ������ = ���������� �������� �����
		while (slot != NULL && strcmp(slot, "\n"))
		{
			printf("%s ", slot);

			tokensDFA->currentState = 0;
			while (*slot)
			{
				//����� ��������
				//���� ��� ������� �� ������ ��� �����

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
		//������ ����� == �������� �������� - �������� ������� ��� �������
		for (int i = 0; i < sizeTypes; i++)
		{
			printf("\n");
			fgets(&line, 255, f);
			slot = strtok_s(line, delimiter, &context);//�����

			for (int j = 0; j < sizeTypes; j++)
			{
				operatorsTable[sizeOperatorsTable][i][j] = atoi(slot);
				slot = strtok_s(NULL, delimiter, &context);//�����
				printf("%d ", operatorsTable[sizeOperatorsTable][i][j]);
			}			
		}	
		sizeOperatorsTable++;
	}
	fclose(f);	
}


//������� �� ������ �� ����� ������ - ����� ����� ����� �� ����� ����� ����� �� ����� ��� �����
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
			//....�� �� ������� ����� �� ���� �������� ������ ���� �� �����
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
		if (codeTokens[ast->id].idTokenType == 2)//�� ������
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
					//symbolTable[tokens[ast->id].id].idTypes[symbolTable[tokens[ast->id].id].count++] = ??���� ������� �� �� ���' �� ������
					//���� ����� �� ������ �� ����� ������. �� ������ ���? ����� �� �� ����� �������
					//��� ������ �� ����� ����� ����� �����?????????..........
					//�� �� ���� ���� ������ ��� ������� ����� ������ - ������� ���������
					//symbolTable[tokens[ast->id].id].idTypes[symbolTable[tokens[ast->id].id].count++] = updateAssignmentValue();
				}

			}
			//else //if (canChange == false)
			//{
			//	//����� ���� ���� ����� ���� ���� ����� �� ���� ��� ������� ���� ���� ���� �� �����!!
			//	//���� - �� ���� ���� ������� ����� ��� �� ����� �� ����� - �������� ����� ������� ����
			//	//�� �� - ������� ����. �� �� ���� ���� ��� ����� �� ����� ������ �� ������
			//	if (symbolTable[codeTokens[ast->id].id].isDefined == false)
			//	{
			//		//strcpy(type, "err!! identifier "+ symbolTable[tokens[ast->id].id].name+"is not def");���� ��������

			//		return;
			//	}
			//}

		}
	}
	else if (ast->id == 4 && canChange == true)//AssignmentValue ��� ����� ������
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

			//�� �� ��� ���� ���� ���� ����� �� ������ �� L ����� ������ �� ������
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
		if (codeTokens[ast->id].idTokenType == 2)//������
		{
			if (scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isDefined == false)//�� �� ������ �� ����� ����
			{
				if (sizeIdVariables == 0)
				{
					scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isDefined = true;
					idVariable = (int*)malloc(sizeof(int));
					idVariable[sizeIdVariables++] = codeTokens[ast->id].id;
				}
				else//�� ������� ������ ��� ��� ����� ���� �� �����
				{
					scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].isDefined = true;
					idVariable = (int*)realloc(idVariable, sizeof(int) * (sizeIdVariables + 1));
					idVariable[sizeIdVariables++] = codeTokens[ast->id].id;
				}
			}
		}
		else if (tokens[ast->id].idTokenType == 6)//�����
		{
			idTypeVariable == types[codeTokens[ast->id].idType].id;
		}
	}
	AST** a = ast->childrens;
	for (int i = 0; i < ast->numChildrens; i++)
	{
		
		if (a[i]->isTerminal && a[i]->id == 3 && canChange == true)//Assignment ���� ������
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
			//��� ����� ��� ��� ����� �������
			//��� ���� ���� ����� ���� ����� �����!!
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

	///������� �������
	if (ast->id == 2)//IdentifierDefinition ����� �����
	{
		free(idVariable);
		sizeIdVariables = 0;
		updateVariableDefinition(ast);
		for (int j = 0; j < sizeIdVariables; j++)
		{
			//���� �� ��� ������ ����??
			scopes[*(int*)peek(&scopesStack)].symbols[idVariable[j]].idTypes = (int*)malloc(sizeof(int));
			scopes[*(int*)peek(&scopesStack)].symbols[tokens[ast->id].id].idTypes[scopes[*(int*)peek(&scopesStack)].symbols[codeTokens[ast->id].id].count++] = idTypeVariable;

			symbolTable[idVariable[j]].idType = idTypeVariable;
		}
	}
	if (ast->id == 3 /*&& canChange == true*/)//Assignment ���� ������
	{
		isAssignment = true;
		firstIdentifier = true;
		free(idVariable);
		sizeIdVariables = 0;
		updateVariableAssignment(ast);
	}
	if (ast->id == 8 || ast->id == 9)//Loop | FunctionDefinition ����� �� ����� �������
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
	if (ast->id == 10 && canChange == false)//FunctionCall ����� ��������
	{
		identifier = searchIdentifier(ast,-1);//...������

	}
	a = ast->childrens;
	ast->idType = typesCheck(a[i]);
	L = ast->idType;
	i++;
	
	
	//������ �� ���� �� ������� �� ���� ������!!
	//L-������ �� ������ ���� �
	
	for (; i < ast->numChildrens; i++)
	{		
		if (a[i]->isTerminal == true)
		{		
			//....�� �� ������� ����� �� ���� �������� ������ ���� �� �����
			if (a[i]->idType == 3)
			{
				operatorId = tokens[a[i]->id].idType;//��� ������ �� ����� �� ��������?????//��� ������ ��� ��������???????
			}
		}	
		a[i]->idType = typesCheck(a[i]); 
		R = a[i]->idType;
		L = operatorsTable[operatorId][L][R];

	}
	//printf("\n%s ", nonTerminals[ast->id].name);
	if (ast->id == 8 || ast->id == 9)//Loop || FunctionDefinition ����� �� ����� �������
	{
		pop(&scopesStack);
		if (ast->id == 9)
		{
			stopScopes = 0;
		}
	}
	return L;//�� �� �� �� ���

	//�� �� ����� �� ���� ���� ���� �� ������ �� ����� 
	//����� ���� ������
}




//����� ������ ����� ������ ������� ����� ��� ��� ������� ���� ����� ������� ���������





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

