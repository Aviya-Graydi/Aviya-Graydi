#ifndef _CFG_C

#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include< ctype.h >

#include "CFG.h"
#include "../LEXICAL/Token.c"


void createNonTerminal(NonTerminal* nonTerminal,int id, char* name)
{

	char value[80];
	//nonTerminal = (NonTerminal*)malloc(sizeof(NonTerminal));
	if (nonTerminal)
	{
		nonTerminal->id = id;
		strcpy_s(nonTerminal->name, (rsize_t)_countof(value), name);
		nonTerminal->countOptions = 0;
		nonTerminal->options = NULL;
	}
	//return *nonTerminal;
}

void createClosedGroupTable(ClosedGroupTable* c)
{
	//c = (ClosedGroupTable*)malloc(sizeof(ClosedGroupTable));
	if (c)
	{
		c->rules = NULL;
		c->size = 0;
	}
}
void createClosedGroup(ClosedGroup* closedGroup , int index ,int lastIndex ,  int idNonTerminal  , int option,int idValue)
{
	//closedGroup = (ClosedGroup*)malloc(sizeof(ClosedGroup));
	if (closedGroup)
	{
		closedGroup->idNonTerminal = idNonTerminal;
		closedGroup->index = index;
		closedGroup->lastIndex = lastIndex;
		closedGroup->option = option;
		closedGroup->idValue = idValue;
	}
}


void fillCFG(DFA* tokensDFA , DFA* CFG_DFA)
{
	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = "\t";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת


	DFA* dfa;
	int idNonTerminal;
	int idTerminalOrNonTerminal;
	bool isTerminal;
	int i;
	int j;
	char value[255];
	int flag = 0;

	FILE* f1;
	
	char path[100] = "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\CFG.txt";
	errno_t error = fopen_s(&f1, path, "r");
	//errno_t error = fopen_s(&f1, "X:\\סמינר\\התמחויות\\הנדסת תוכנה\\שנה ב\\תלמידות\\אביה גריידי\\פרוייקט\\פרוייקט\\Transitions.csv", "r");
	if (error != 0)
	{
		printf("\nCould not open the file CFG :(");
		return;
	}

	printf("\nThe file CFG was opened successfully !!! :)");


	while (fgets(&line, 255, f1))//הלולאה עוברת שורה שורה
	{

		//fscanf_s(f, "%s", &line, 255);//שורה מהקובץ
		//fgets(&line, 255, f);

		slot = strtok_s(line, delimiter, &context);//משבצת

		if (*slot != '|')//משבצת ראשונה
		{
			CFG_DFA->currentState = 0;

			while (*slot)
			{
				if (CFG_DFA->currentState != -1)
				{
					CFG_DFA->currentState = (CFG_DFA->states[CFG_DFA->currentState]).transitions[hash(*slot)].idState;

				}

				slot++;
			}

			if (CFG_DFA->currentState != -1 && CFG_DFA->states[CFG_DFA->currentState].isFinal == true)
			{
				idNonTerminal = CFG_DFA->states[CFG_DFA->currentState].idInArray;
			}
		}

		//מילוי אופציה במערך הנונ-טרמינלים 
		//צריך לבדוק- אם זו אופציה ראשונהה לעשות מאלוק ואם לא - לעשות ריאלוק


		if (nonTerminals[idNonTerminal].countOptions == 0)
		{
			nonTerminals[idNonTerminal].options = (CFG_Value**)malloc(sizeof(CFG_Value*));

		}
		else
		{
			nonTerminals[idNonTerminal].options = (CFG_Value**)realloc(nonTerminals[idNonTerminal].options, sizeof(CFG_Value*) * (nonTerminals[idNonTerminal].countOptions + 1));
		}

		i = 0;

		while ((slot = strtok_s(NULL, delimiter, &context)) && *slot != '\n')//שאר המשבצות = הצד הימני בו צריך להחליף כל נונ-טרמינל
		{

			strcpy_s(value, (rsize_t)_countof(value), slot);

			if (isupper(*slot))
			{
				isTerminal = false;
				dfa = CFG_DFA;
			}
			else
			{
				isTerminal = true;
				dfa = tokensDFA;
			}
		

			dfa->currentState = 0;
			if (strcmp(value,  "identifier"))
			{


				while (*slot)
				{
					if (dfa->currentState >= 0)
					{
						dfa->currentState = (dfa->states[dfa->currentState]).transitions[hash(*slot)].idState;
					}


					slot++;
				}
				idTerminalOrNonTerminal = dfa->states[dfa->currentState].idInArray;
				
			}
			else {
				idTerminalOrNonTerminal = 0;
			}
			

			if (dfa->states[dfa->currentState].isFinal == true || !strcmp(value, "identifier"))
			{
				//מילוי ערך לאופציה במערך הנונ-טרמינלים 
				//צריך לבדוק- אם זה ערך ראשון לעשות מאלוק ואם לא - לעשות ריאלוק
				

				if (i == 0)
				{
					nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions] = (CFG_Value*)malloc(sizeof(CFG_Value));

					nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions]->countValues = 0;
				}
				else
				{
					nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions] = (CFG_Value*)realloc(nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions] , sizeof(CFG_Value)*(i+1));
					nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].countValues = 0;
				}
				
				
				

				nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].idTerminalOrNonTerminal = idTerminalOrNonTerminal;
				nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].isTerminal = isTerminal;
				nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].id = i;
				nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][0].countValues++;
				strcpy_s(nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].value, (rsize_t)_countof(value) , value);
				//printf("\n %s id value: %d is terminal: %d value: %s \n", nonTerminals[idNonTerminal].name, nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].idTerminalOrNonTerminal, nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].isTerminal, nonTerminals[idNonTerminal].options[nonTerminals[idNonTerminal].countOptions][i].value);

			}
			i++;
		}
		nonTerminals[idNonTerminal].countOptions++;

	}
	CFG_Value* cfg_value;
	int t;
	for (i = 0; i < sizeNonTerminals; i++)
	{
		printf("\n%s \n", nonTerminals[i].name);
		for (j = 0; j < nonTerminals[i].countOptions; j++)
		{
			cfg_value = nonTerminals[i].options[j];
			for (t = 0; t < cfg_value->countValues; t++)
			{
				printf("%d %d %d  %s \n", cfg_value[t].id, cfg_value[t].idTerminalOrNonTerminal, cfg_value[t].isTerminal, cfg_value[t].value);
			}
		}
	}
	fclose(f1);
}

void createFirstAndFollow()
{
	int i, j, t;
	int idNonTerminal;
	int idTerminal;

	first = (int**)malloc(sizeof(int*) * sizeNonTerminals);
	follow = (int**)malloc(sizeof(int*) * sizeNonTerminals);
	closedGroupTable = (ClosedGroupTable*)malloc(sizeof(ClosedGroupTable) * sizeNonTerminals);

	for (i = 0; i < sizeNonTerminals; i++)
	{
		first[i] = (int*)malloc(sizeof(int) * (sizeTokens + sizeNonTerminals));
		follow[i] = (int*)malloc(sizeof(int) * (sizeTokens +sizeNonTerminals));
		createClosedGroupTable(&closedGroupTable[i]);
		for (j = 0; j < (sizeTokens + sizeNonTerminals);j++)
		{
			first[i][j] = -1;
			follow[i][j] = -1;
		}
				
	}

	for (i = 0; i < sizeNonTerminals; i++)
	{
		//idNonTerminal = i;
		for (j = 0; j < nonTerminals[i].countOptions; j++)
		{
			
			findFirst(nonTerminals[i].id, nonTerminals[i].id, 0, 0);
				
				//first[i][nonTerminals[idNonTerminal].options[j][0].idTerminalOrNonTerminal] = 1;
				//printf("\n %s-%s", nonTerminals[i].name, tokens[nonTerminals[idNonTerminal].options[j][0].idTerminalOrNonTerminal].value);
		}

	}
	
	findFollow();


	for (i = 0; i < sizeNonTerminals; i++)
	{
		printf("\n \n %s \n\n first: " , nonTerminals[i].name);
		for (j = 0; j < sizeTokens; j++)
		{
			
			if (first[i][j] == 1)
			{
				printf(" %s ,  ", tokens[j].value);
			}
			
		}
		/*for (; j < (sizeTokens + sizeNonTerminals); j++)
		{
			if (first[i][j] == 1)
			{
				printf("%s ,", nonTerminals[j - sizeTokens].name);
			}

		}*/
		printf("  \n follow:  ");
		for (j = 0; j < sizeTokens; j++)
		{

			if (follow[i][j] == 1)
			{
				printf(" %s ,  ", tokens[j].value);
			}

		}
		for (; j < (sizeTokens + sizeNonTerminals); j++)
		{
			if (follow[i][j] == 1)
			{
				printf("%s ,", nonTerminals[j - sizeTokens].name);
			}
			
		}

		printf("  \n closedGroupTable:  ");
		
		for (int r = 0; r < closedGroupTable[i].size;r++)
		{
			printf(" %s ,  ", closedGroupTable[i].rules[r]);

		}
	}

}

//רעיון חדש - פונקציה רקורסיבית...
void findFirst(int id , int idNonTerminal ,int idLast, int index)
{
	int i;
	int x;
	char value[10];
	int isDuplicate = 0;
	for (i = 0; i < nonTerminals[idNonTerminal].countOptions && index  < nonTerminals[idNonTerminal].options[i]->countValues;i++)
	{
		if (nonTerminals[idNonTerminal].options[i][index].isTerminal == false && strcmp(nonTerminals[idNonTerminal].options[i][index].value, "identifier"))
		{
			x = sprintf_s(value, (rsize_t)_countof(value), "%d %d", idNonTerminal, i);

			if (idNonTerminal != nonTerminals[idNonTerminal].options[i][index].idTerminalOrNonTerminal)
			{
				first[id][sizeTokens + nonTerminals[idNonTerminal].options[i][index].idTerminalOrNonTerminal] = 1;
				findFirst(id , nonTerminals[idNonTerminal].options[i][index].idTerminalOrNonTerminal ,idNonTerminal, 0);
			}
			if (closedGroupTable[id].size == 0)
			{
				closedGroupTable[id].rules = (char**)malloc(sizeof(char*));
				closedGroupTable[id].rules[0] = (char*)malloc(10* sizeof(char));
			}
			else
			{

				for (int d = 0; d < closedGroupTable[id].size; d++)
				{
					if (strcmp(closedGroupTable[id].rules[d], value) == 0)
					{
						isDuplicate = 1;
						break;
					}
				}
				if (isDuplicate == 0)
				{
					closedGroupTable[id].rules = (char**)realloc(closedGroupTable[id].rules, (closedGroupTable[id].size + 1) * sizeof(char*));
					closedGroupTable[id].rules[closedGroupTable[id].size] = (char*)malloc(10 * sizeof(char));

				}
				//else
				//{
				//	//printf("????");
				//}
			}
			if (isDuplicate == 0)
			{
				strcpy_s(closedGroupTable[id].rules[closedGroupTable[id].size], (rsize_t)_countof(value), value);
				closedGroupTable[id].size++;
			}

		}
		else if (!strcmp(nonTerminals[idNonTerminal].options[i][index].value, "identifier"))
		{
			first[id][0] = 1;
			//printf("\n %s-%s", nonTerminals[id].name, tokens[nonTerminals[idNonTerminal].options[i][index].idTerminalOrNonTerminal].value);
			x = sprintf_s(value, (rsize_t)_countof(value), "%d %d", idNonTerminal, i);

			if (closedGroupTable[id].size == 0)
			{
				closedGroupTable[id].rules = (char**)malloc(sizeof(char*));
				closedGroupTable[id].rules[0] = (char*)malloc(10 * sizeof(char));
			}
			else
			{

				for (int d = 0; d < closedGroupTable[id].size; d++)
				{
					if (strcmp(closedGroupTable[id].rules[d], value) == 0)
					{
						isDuplicate = 1;
						break;
					}
				}
				if (isDuplicate == 0)
				{
					closedGroupTable[id].rules = (char**)realloc(closedGroupTable[id].rules, (closedGroupTable[id].size + 1) * sizeof(char*));
					closedGroupTable[id].rules[closedGroupTable[id].size] = (char*)malloc(10 * sizeof(char));

				}
				/*else
				{
					printf("????");
				}*/
			}
			if (isDuplicate == 0)
			{
				strcpy_s(closedGroupTable[id].rules[closedGroupTable[id].size], (rsize_t)_countof(value), value);
				closedGroupTable[id].size++;
			}
		}
		else
		{
			first[id][nonTerminals[idNonTerminal].options[i][index].idTerminalOrNonTerminal] = 1;
			x = sprintf_s(value, (rsize_t)_countof(value), "%d %d", idNonTerminal, i);

			if (closedGroupTable[id].size == 0)
			{
				closedGroupTable[id].rules = (char**)malloc(sizeof(char*));
				closedGroupTable[id].rules[0] = (char*)malloc(10 * sizeof(char));
			}
			else
			{

				for (int d = 0; d < closedGroupTable[id].size; d++)
				{
					if (strcmp(closedGroupTable[id].rules[d], value) == 0)
					{
						isDuplicate = 1;
						break;
					}
				}
				if (isDuplicate == 0)
				{
					closedGroupTable[id].rules = (char**)realloc(closedGroupTable[id].rules, (closedGroupTable[id].size + 1) * sizeof(char*));
					closedGroupTable[id].rules[closedGroupTable[id].size] = (char*)malloc(10 * sizeof(char));
				}
				/*else
				{
					printf("????");
				}*/
			}
			if (isDuplicate == 0)
			{
				strcpy_s(closedGroupTable[id].rules[closedGroupTable[id].size], (rsize_t)_countof(value), value);
				closedGroupTable[id].size++;
			}

			if (!strcmp(nonTerminals[idNonTerminal].options[i][index].value, "<null>"))
			{
				findFirst(id,idLast, idNonTerminal, ++index);
			}
		}
	}
}


void copyFirstToFollow(int NonTerminalId,int optionId ,int valueId , int followId)
{
	int i;

	if (nonTerminals[NonTerminalId].options[optionId]->countValues == valueId + 1)//אם אני האחרון בערכים
	{
		follow[followId][sizeTokens + NonTerminalId] = 1;
	}
	else if (nonTerminals[NonTerminalId].options[optionId][valueId+1].isTerminal == true)
	{
		follow[followId][nonTerminals[NonTerminalId].options[optionId][valueId+1].idTerminalOrNonTerminal] = 1;
	}
	else//האיבר הבא הוא נונטרמינל
	{
		for (i = 0; i < sizeTokens; i++)
		{
			follow[followId][i] = first[nonTerminals[NonTerminalId].options[optionId][valueId + 1].idTerminalOrNonTerminal][i];
		}
		if (first[nonTerminals[NonTerminalId].options[optionId][valueId + 1].idTerminalOrNonTerminal][1] = 1)
		{
			copyFirstToFollow(NonTerminalId, optionId, valueId + 1, followId);
		}
	}

}



void copyFollow()
{
	int i, j, t;
	for (i = 0; i < sizeNonTerminals ; i++)//כל נונטרמינל
	{
		for (j = 0; j < sizeNonTerminals; j++)//
		{
			if (follow[i][sizeTokens + j] == 1)
			{
				for (t = 0; t < sizeTokens; t++)
				{
					if (follow[j][t] == 1)
					{
						follow[i][t] = 1;
					}
					

				}


			}

		}
	}
}

void findFollow()
{
	int i;
	int j;
	for (i = 0; i < sizeNonTerminals; i++)
	{
		for(j = 0;j<nonTerminals[i].countOptions;j++)
		{
			for (int t = 0; t < nonTerminals[i].options[j]->countValues; t++)
			{
				
				if (nonTerminals[i].options[j][t].isTerminal == false)//בעבור כל הנונטרמינלים
				{

					copyFirstToFollow(i, j, t, nonTerminals[i].options[j][t].idTerminalOrNonTerminal);


				}

			}
		}
	}
	follow[0][sizeTokens - 1] = 1;
	copyFollow();
}



initStateInTable()
{
	if (sizeActionAndGOTO_Table == 0)
	{
		actionAndGOTO_Table = (Table**)malloc(sizeof(Table*));

	}
	else
	{
		actionAndGOTO_Table = (Table**)realloc(actionAndGOTO_Table , sizeof(Table*)*(sizeActionAndGOTO_Table+1));
	}

	actionAndGOTO_Table[sizeActionAndGOTO_Table] = (Table*)malloc(sizeof(Table) * (sizeTokens + sizeNonTerminals));

	for (int i = 0; i < (sizeTokens + sizeNonTerminals); i++)
	{
		strcpy_s(actionAndGOTO_Table[sizeActionAndGOTO_Table][i].actionOrGOTO , 255 , "<>");
	}

	sizeActionAndGOTO_Table++;
}

//void fillReduce(int idNonTerminal , int r , int index , int i ,int j)
//{
//	int t;
//	char value[255] = " ";
//	int x;
//
//	char delimiter[] = " ";//סימן הפרדה
//	char* context = NULL;//תוכן מופרד מהשורה
//
//	strcpy_s(value, 255, actionAndGOTO_Table[index][r].actionOrGOTO);
//
//	char* str  = strtok_s(value, delimiter, &context);
//
//	if (follow[idNonTerminal][r] == 1 &&  (!strcmp(str, "<>")))
//	{
//		if (i == 0 && j == 0)
//		{
//			strcpy_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "Accept", i, j);
//		}
//		else
//		{
//			x = sprintf_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "R %d %d", i, j);
//		}
//		printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);
//	}
//	else
//	{
//
//		for ( t = 0; t< sizeNonTerminals;t++ )
//		{
//			if (follow[idNonTerminal][sizeTokens + t]== 1 && idNonTerminal != t)
//			{
//				fillReduce( t, r, index, i, j);
//			}
//			
//		}
//		
//	}
//	
//}


void fillReduce7(int idNonTerminal, int index, int i, int j)
{
	int t;
	char value[255] = " ";
	int x ,r;

	char delimiter[] = " ";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה

	

	for (r = 0; r < sizeTokens; r++)//למלא רדיוס בכל מקום שיש FOLLOW
	{
		strcpy_s(value, 255, actionAndGOTO_Table[index][r].actionOrGOTO);

		char* str = strtok_s(value, delimiter, &context);
		if (follow[idNonTerminal][r] == 1 && (!strcmp(str, "<>")))
		{
			if (i == 0 && j == 0)
			{
				strcpy_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "Accept", i, j);

			}
			else
			{
				x = sprintf_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "R %d %d", i, j);

			}
			printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);

		}
	}
	for (t = 0; t < sizeNonTerminals; t++)
	{
		if (follow[idNonTerminal][sizeTokens + t] == 1 && idNonTerminal != t)
		{
			fillReduce7(t,  index, i, j);
		}

	}
	
	
		


}


void buildTable()
{
	int flag = 0;

	int i , j , t;
	int index;
	int x;
	int size;
	int id;
	int r;
	int lastIndex;
	int lastLastIndex;

	char value[255] = "";
	char buffer[255] = "";
	char* str = "";
	char s[255];

	char delimiter[] = " ";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה

	char action[255] = "";

	initStateInTable();

	printf("\n");

	CFG_Value cfg_value;

	
	for (i = 0; i < sizeNonTerminals; i++)//נונ-טרמינל
	{	
		for (j = 0; j < nonTerminals[i].countOptions; j++)//אופציה
		{
			lastLastIndex = -1;
			lastIndex = -1;
			index = 0; 
			for (t = 0; t < nonTerminals[i].options[j]->countValues;t++)//איבר בדקדוק
			{
				flag = 0;
				cfg_value = nonTerminals[i].options[j][t];

						
				if (cfg_value.isTerminal == false)//non-terminal
				{
					strcpy_s(value, 255, actionAndGOTO_Table[index][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO);

					str = strtok_s(value, delimiter, &context);
					id = cfg_value.idTerminalOrNonTerminal;

					if (!strcmp(str, "<>"))
					{
						//strcpy_s(actionAndGOTO_Table[index][cfg_value.idTerminalOrNonTerminal].actionOrGOTO, 255, "<>");

						x = sprintf_s(actionAndGOTO_Table[index][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "GOTO %d", sizeActionAndGOTO_Table);
						printf("%d %s %s\n", index, nonTerminals[cfg_value.idTerminalOrNonTerminal].name, actionAndGOTO_Table[index][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO);
						lastLastIndex = lastIndex;
						lastIndex = index;
						index = sizeActionAndGOTO_Table;
						initStateInTable();

						//for (r = 0; r < sizeTokens; r++)//למלא S בכל מקום שיש first
						//{
						//	if ((first[cfg_value.idTerminalOrNonTerminal][r] == 1) && (!strchr(actionAndGOTO_Table[lastIndex][r].actionOrGOTO, 'S')))
						//	{
						//		/*if (t != 0)
						//		{
						//			x = sprintf_s(actionAndGOTO_Table[lastIndex][r].actionOrGOTO, (rsize_t)_countof(value), "S %d", index);
						//			printf("%d %s %s\n", lastIndex, tokens[r].value, actionAndGOTO_Table[lastIndex][r].actionOrGOTO);

						//		}
						//		else
						//		{*/
						//			strcpy_s(actionAndGOTO_Table[lastIndex][r].actionOrGOTO, 255, "S");
						//			printf("%d %s %s\n", lastIndex, tokens[r].value, actionAndGOTO_Table[lastIndex][r].actionOrGOTO);
						//		//}
						//		


						//	}


						//}

						//for (r = 0; r <  sizeNonTerminals; r++)//למלא GOTO בכל מקום שיש first
						//{
						//	if (first[ cfg_value.idTerminalOrNonTerminal][sizeTokens + r] == 1 && (!strstr(actionAndGOTO_Table[lastIndex][r].actionOrGOTO, "GOTO")))
						//	{
						//		if (t == 0)
						//		{
						//			x = sprintf_s(actionAndGOTO_Table[lastIndex][r].actionOrGOTO, (rsize_t)_countof(value), "GOTO %d", index);
						//			printf("%d %s %s\n", lastIndex, nonTerminals[r].name, actionAndGOTO_Table[lastIndex][r].actionOrGOTO);

						//		}
						//		else
						//		{
						//			strcpy_s(actionAndGOTO_Table[lastIndex][sizeTokens + r].actionOrGOTO, 255, "GOTO");
						//			printf("%d %s %s\n", lastIndex, nonTerminals[r].name, actionAndGOTO_Table[lastIndex][sizeTokens + r].actionOrGOTO);
						//		}
						//			//flag = 1;
						//	}
						//}
						/*if (flag == 1)
						{
							lastIndex = index;
							index = sizeActionAndGOTO_Table;
							initStateInTable();
						}*/


						//--
						// 
						//for (r = 0; r < sizeActionAndGOTO_Table; r++)//למלא GOTO בכל מקום שיש 
						//{
						//	if (t == 0 && !strcmp(actionAndGOTO_Table[r][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO, "GOTO"))
						//	{
						//		x = sprintf_s(actionAndGOTO_Table[r][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "GOTO %d", sizeActionAndGOTO_Table - 1);
						//		printf("%d %s %s\n", r, nonTerminals[cfg_value.idTerminalOrNonTerminal].name, actionAndGOTO_Table[r][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO);

						//	}


						//}

						if (t != 0)
						{
							// ליצור איבר בטבלה של קבוצת הסגור---

							if (sizeClosedGroups == 0)
							{
								closedGroups = (ClosedGroup*)malloc(sizeof(ClosedGroup));
							}
							else
							{
								closedGroups = (ClosedGroup*)realloc(closedGroups,(sizeClosedGroups+1)*sizeof(ClosedGroup));
							}
							createClosedGroup(&closedGroups[sizeClosedGroups++], lastIndex, lastLastIndex ,i, j , id);

						}
						
						
					}
					else
					{
						lastLastIndex = lastIndex;
						lastIndex = index;
						index = atoi(strtok_s(NULL, delimiter, &context));

					}
					
					


					if ((t + 1) == nonTerminals[i].options[j]->countValues)//האיבר האחרון
					{

						

						for ( r = 0; r < sizeTokens; r++)//למלא רדיוס בכל מקום שיש FOLLOW
						{
							strcpy_s(value, 255, actionAndGOTO_Table[index][r].actionOrGOTO);
							str = strtok_s(value, delimiter, &context);
							if (follow[id][r] == 1 && follow[i][r]==1 && (!strcmp(str, "<>")))
							{
								if (i == 0 && j == 0)
								{
									strcpy_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "Accept");

								}
								else
								{

									x = sprintf_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "R %d %d", i, j);

								}
								printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);

							}
							//else if (follow[id][r] == 1 && follow[i][r] == 1 && strchr(str, 'R'))*************************************************
							//{

							//	x = sprintf_s(action, (rsize_t)_countof(value), "R %d %d", i, j);

							//	if (strcmp(actionAndGOTO_Table[index][r].actionOrGOTO, action))//אם זה שונה
							//	{
							//		x = sprintf_s(actionAndGOTO_Table[lastIndex][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "GOTO %d", sizeActionAndGOTO_Table);
							//		printf("%d %s %s\n", lastIndex, nonTerminals[cfg_value.idTerminalOrNonTerminal].name, actionAndGOTO_Table[lastIndex][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO);


							//		lastIndex = index;
							//		index = sizeActionAndGOTO_Table;
							//		initStateInTable();
							//		strcpy_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), action);

							//	}

							//	printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);

							//}*************************************************
						}
						//fillReduce7(id, index, i, j);
					}	
					
					
					
				}
				else if (cfg_value.isTerminal == true)//Terminal
				{
					strcpy_s(value, 255, actionAndGOTO_Table[index][ cfg_value.idTerminalOrNonTerminal].actionOrGOTO);

					str = strtok_s(value, delimiter, &context);

					//strcpy_s(actionAndGOTO_Table[index][cfg_value.idTerminalOrNonTerminal].actionOrGOTO ,255 , "<>");

					if ((!strcmp(str, "S") )&& strcmp(context, ""))
					{
						lastLastIndex = lastIndex;
						lastIndex = index;
						index = atoi(strtok_s(NULL, delimiter, &context));
					}
					else
					{
						x = sprintf_s(actionAndGOTO_Table[index][cfg_value.idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "S %d", sizeActionAndGOTO_Table);
						printf("%d %s %s\n", index, tokens[cfg_value.idTerminalOrNonTerminal].value, actionAndGOTO_Table[index][cfg_value.idTerminalOrNonTerminal].actionOrGOTO);
						id = nonTerminals[i].id;

						lastLastIndex = lastIndex;
						lastIndex = index;
						index = sizeActionAndGOTO_Table;
						initStateInTable();

					}

					

					//--

					//for (r = 0; r < sizeActionAndGOTO_Table; r++)//למלא S בכל מקום שיש first
					//{
					//	if (!strcmp(actionAndGOTO_Table[r][cfg_value.idTerminalOrNonTerminal].actionOrGOTO, "S"))
					//	{
					//		x = sprintf_s(actionAndGOTO_Table[r][cfg_value.idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "S %d", sizeActionAndGOTO_Table-1);
					//		printf("%d %s %s\n", r, tokens[cfg_value.idTerminalOrNonTerminal].value, actionAndGOTO_Table[r][cfg_value.idTerminalOrNonTerminal].actionOrGOTO);

					//	}


					//}

					if ((t + 1) == nonTerminals[i].options[j]->countValues)//האיבר האחרון
					{
						
						//index = atoi(strtok_s(NULL, delimiter, &context));

						//for ( r = 0; r < sizeTokens; r++)//למלא רדיוס בכל מקום שיש FOLLOW
						//{
						//	//if (cfg_value.isTerminal == false || (cfg_value.isTerminal == true && r != cfg_value.idTerminalOrNonTerminal))
						//	//{

						//		fillReduce(id, r, index, i, j);
						//	//}


						//}

						for (r = 0; r < sizeTokens; r++)//למלא רדיוס בכל מקום שיש FOLLOW
						{
							strcpy_s(value, 255, actionAndGOTO_Table[index][r].actionOrGOTO);

							str = strtok_s(value, delimiter, &context);
							if (follow[id][r] == 1 && (!strcmp(str, "<>")))
							{
								if (i == 0 && j == 0)
								{
									strcpy_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "Accept", i, j);

								}
								else
								{
									x = sprintf_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), "R %d %d", i, j);

								}
								printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);

							}
							//else if (follow[id][r] == 1 && strchr(str, 'R'))
							//{
							//	
							//	x = sprintf_s(action, (rsize_t)_countof(value), "R %d %d", i, j);

							//	if (strcmp(actionAndGOTO_Table[index][r].actionOrGOTO, action))//אם זה שונה
							//	{
							//		x = sprintf_s(action, (rsize_t)_countof(value), "S %d", sizeActionAndGOTO_Table);
							//		if (strcmp(actionAndGOTO_Table[lastIndex][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO, action))//אם גם שם יש התנגשות
							//		{
							//			x = sprintf_s(actionAndGOTO_Table[lastIndex][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "S %d", sizeActionAndGOTO_Table);
							//			printf("%d %s %s\n", lastIndex, nonTerminals[cfg_value.idTerminalOrNonTerminal].name, actionAndGOTO_Table[lastIndex][sizeTokens + cfg_value.idTerminalOrNonTerminal].actionOrGOTO);

							//			lastLastIndex = lastIndex;
							//			lastIndex = index;
							//			index = sizeActionAndGOTO_Table;
							//			initStateInTable();
							//			strcpy_s(actionAndGOTO_Table[index][r].actionOrGOTO, (rsize_t)_countof(value), action);
							//			printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);


							//		}
							//		else
							//		{
							//			printf("? ? ? ? ? ? ? ? ? ? ?");
							//		}
							//	}
							//}
							
						}
					}

				}

				/*if (strcmp(context, ""))
				{
					index = atoi(strtok_s(NULL, delimiter, &context));

				}*/
					
			
				//initStateInTable();		
			}
		}
	}

	//for (j = 0; j < sizeTokens; j++)
	//{
	//	for (r = 1; r < sizeActionAndGOTO_Table; r++)//למלא S בכל מקום שיש 
	//	{

	//		if (!strcmp(actionAndGOTO_Table[r][j].actionOrGOTO, "S"))
	//		{
	//			strcpy_s(actionAndGOTO_Table[r][j].actionOrGOTO, (rsize_t)_countof(value), actionAndGOTO_Table[0][j].actionOrGOTO);
	//			printf("%d %s %s\n", r, tokens[j].value, actionAndGOTO_Table[r][j].actionOrGOTO);

	//		}


	//	}
	//}
	//for (; j < (sizeTokens + sizeNonTerminals); j++)
	//{
	//	for (r = 1; r < sizeActionAndGOTO_Table; r++)//למלא GOTO בכל מקום שיש 
	//	{
	//		if (!strcmp(actionAndGOTO_Table[r][j].actionOrGOTO, "GOTO"))
	//		{
	//			strcpy_s(actionAndGOTO_Table[r][j].actionOrGOTO, (rsize_t)_countof(value), actionAndGOTO_Table[0][j].actionOrGOTO);
	//			printf("%d %s %s\n", r, nonTerminals[j - sizeTokens].name, actionAndGOTO_Table[r][j].actionOrGOTO);

	//		}


	//	}
	//}

	printf("\n\n\nfill closed groups\n\n\n");

	int option;
	int indexTable;
	char* actionName;
	char* actionTableName;
	char tableValue[255] = "--";//הערך בטבלה
	char* contextTable = NULL;//תוכן מופרד מהשורה
	flag = 0;
	char compare[255] = "--";
	char v[255] = "--";
	/*************/
	/*char last[255] = " ";*/
	
	for (i = 0; i < sizeClosedGroups; i++)
	{
		for (j = 0; j < closedGroupTable[closedGroups[i].idValue].size; j++)
		{
			indexTable = 0;
			r = 0;
			strcpy_s(value, (rsize_t)_countof(value),closedGroupTable[closedGroups[i].idValue].rules[j]);
			id = atoi(strtok_s(value, delimiter, &context));
			option =atoi( strtok_s(NULL, delimiter, &context));
			index = closedGroups[i].index;
			lastIndex = closedGroups[i].lastIndex;

			if (nonTerminals[id].options[option][r].isTerminal == true)
			{
				strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[indexTable][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);
			}
			else
			{
				strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[indexTable][nonTerminals[id].options[option][r].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO);
			}

			if (nonTerminals[id].options[option][r].isTerminal == true)
			{
				strcpy_s(value, (rsize_t)_countof(value), actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);
			}
			else
			{
				strcpy_s(value, (rsize_t)_countof(value), actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO);
			}
			//מה עושים אם זה ACCEPT????
			while (r + 1 < nonTerminals[id].options[option]->countValues && strcmp(value, "<>"))// && strchr(value, 'R') && !strcmp(value, "Accept"))
			{
				/*************/
				/*strcpy_s(last, (rsize_t)_countof(value), value);*/
				lastIndex = index;

				actionName = strtok_s(value, delimiter, &context);
				/*if (strcmp(value, "Accept"))
				{*/
					index = atoi(strtok_s(NULL, delimiter, &context));

				/*}*/
			
				actionTableName = strtok_s(tableValue, delimiter, &contextTable);
				indexTable = atoi(strtok_s(NULL, delimiter, &contextTable));
				
				r++;

				if (nonTerminals[id].options[option][r].isTerminal == true)
				{
					strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[indexTable][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);
				}
				else
				{
					strcpy_s(tableValue, (rsize_t)_countof(value), actionAndGOTO_Table[indexTable][nonTerminals[id].options[option][r].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO);
				}

				

				if ( nonTerminals[id].options[option][r].isTerminal == true)
				{
					strcpy_s(value, (rsize_t)_countof(value), actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);
				}
				else
				{
					strcpy_s(value, (rsize_t)_countof(value), actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO);
				}
			}

			

			strcpy_s(v, (rsize_t)_countof(value), tableValue);
			actionTableName = strtok_s(tableValue, delimiter, &contextTable);

			//הגעתי לאינדקס-מצב בטבלה ממנו צריך לחבר. כאן צריך לבדוק אם
			//follow-אין התנגשות ב

			//if (!strcmp(actionTableName, "GOTO"))
			//{

			//	if (closedGroups[i].idValue == 5)
			//	{
			//		printf("F");
			//	}
			//	//tableValue תמיד יהיה S או GOTO
			//	//אם ב- follow שלו יש R והוא שונה מהכלל שאני צריכה

			//	if (id != closedGroups[i].idValue && closedGroups[i].idValue != closedGroups[i].idNonTerminal)
			//	{
			//		flag = 0;
			//	}

			//	if (flag == 1)
			//	{

			//	

			//		indexTable = atoi(strtok_s(NULL, delimiter, &contextTable));
			//		if(indexTable == 50)
			//		{
			//			printf("value???????");
			//		}

			//		
			//		x = sprintf_s(action, (rsize_t)_countof(value), "R %d %d", id, option);					
			//		for (int f = 0; f < sizeTokens && flag == 1; f++)
			//		{
			//			if (follow[closedGroups[i].idNonTerminal][f] == 1 && strchr(actionAndGOTO_Table[indexTable][f].actionOrGOTO, 'R'))
			//			{
			//				strcpy_s(compare, (rsize_t)_countof(value), actionAndGOTO_Table[indexTable][f].actionOrGOTO);
			//				if (strcmp(compare, action))//אם זה שונה
			//				{
			//					flag =0;
			//				}
			//			}
			//		}
			//		if (flag == 0)//אם צריך לעשות R ויש שם R אחר ממה שצריך ליצור מצב חדש
			//		{
			//			//לבדוק לכל ה- follow
			//			if (strcmp(compare, action))//אם זה שונה
			//			{
			//				x = sprintf_s(actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal +sizeTokens].actionOrGOTO, (rsize_t)_countof(value), "GOTO %d", sizeActionAndGOTO_Table);
			//				printf("%d %s %s\n", index, nonTerminals[nonTerminals[id].options[option][r].idTerminalOrNonTerminal].name, actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);
			//				lastIndex = index;
			//				index = sizeActionAndGOTO_Table;
			//				initStateInTable();
			//				for (int f = 0; f < sizeTokens; f++)
			//				{
			//					if (follow[closedGroups[i].idNonTerminal][f] == 1)
			//					{
			//						strcpy_s(actionAndGOTO_Table[index][f].actionOrGOTO, (rsize_t)_countof(value), action);
			//						printf("%d %s %s\n", index, tokens[nonTerminals[id].options[option][r].idTerminalOrNonTerminal].value, actionAndGOTO_Table[index][f].actionOrGOTO);

			//					}
			//				}


			//			}

			//			flag = 1;
			//		}
			//	
			//	/*if (strcmp(actionTableName, 'R'))
			//	{

			//	}*/
			//	}


			//	
			//}

			//
			//
			////if (r + 1 < nonTerminals[id].options[option]->countValues)
			////{
			////	if (nonTerminals[id].options[option][r + 1].isTerminal == true)
			////	{
			////		x = sprintf_s(action, (rsize_t)_countof(value), "R %d %d", atoi(strtok_s(value, delimiter, &context)), atoi(strtok_s(value, delimiter, &context)));
			////		if (strcmp(actionAndGOTO_Table[indexTable][nonTerminals[id].options[option][r + 1].idTerminalOrNonTerminal].actionOrGOTO, action))//אם זה שונה
			////		{
			////			x = sprintf_s(actionAndGOTO_Table[lastIndex][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "GOTO %d", sizeActionAndGOTO_Table);
			////			printf("%d %s %s\n", lastIndex, nonTerminals[nonTerminals[id].options[option][r].idTerminalOrNonTerminal].name, actionAndGOTO_Table[lastIndex][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);


			////			lastIndex = index;
			////			index = sizeActionAndGOTO_Table;
			////			initStateInTable();
			////			strcpy_s(actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), action);

			////		}

			////		printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);
			////	}
			////	else
			////	{
			////		x = sprintf_s(action, (rsize_t)_countof(value), "GOTO %d", atoi(strtok_s(value, delimiter, &context)));
			////		if (strcmp(actionAndGOTO_Table[indexTable][nonTerminals[id].options[option][r + 1].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO, action))//אם זה שונה
			////		{
			////			x = sprintf_s(actionAndGOTO_Table[lastIndex][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), "GOTO %d", sizeActionAndGOTO_Table);
			////			printf("%d %s %s\n", lastIndex, nonTerminals[nonTerminals[id].options[option][r].idTerminalOrNonTerminal].name, actionAndGOTO_Table[lastIndex][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);


			////			lastIndex = index;
			////			index = sizeActionAndGOTO_Table;
			////			initStateInTable();
			////			strcpy_s(actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), action);

			////		}

			////		printf("%d %s %s\n", index, tokens[r].value, actionAndGOTO_Table[index][r].actionOrGOTO);
			////	}
			////}
			//
			//if (!strcmp(actionTableName, "S") || !strcmp(actionTableName, "R")||  flag == 0)
			//{
				if (nonTerminals[id].options[option][r].isTerminal == true)
				{
					strcpy_s(actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO, (rsize_t)_countof(value), v);
					printf("%d %s %s\n", index, tokens[nonTerminals[id].options[option][r].idTerminalOrNonTerminal].value, actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal].actionOrGOTO);

				}
				else //if(flag == 0 )&&!strcmp(actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO , "<>"))
				{
					strcpy_s(actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO, (rsize_t)_countof(value), v);
					printf("%d %s %s\n", index, nonTerminals[nonTerminals[id].options[option][r].idTerminalOrNonTerminal].name, actionAndGOTO_Table[index][nonTerminals[id].options[option][r].idTerminalOrNonTerminal + sizeTokens].actionOrGOTO);

				}
			/*}
			
				
			flag = 1;*/
			
			
		}
	}
	

	//strcpy_s(actionAndGOTO_Table[0][sizeTokens].actionOrGOTO, 255, "GO-TO 1");
	//strcpy_s(actionAndGOTO_Table[1][sizeTokens-1].actionOrGOTO, 255, "Accept");
	printf("\nThe Table------------------\n");

	for (i = 0; i < sizeActionAndGOTO_Table; i++)
	{
		for (j = 0; j < (sizeTokens + sizeNonTerminals);j++)
		{
			if (strcmp(actionAndGOTO_Table[i][j].actionOrGOTO, "<>"))
			{
				printf("%d %d %s\n", i, j, actionAndGOTO_Table[i][j].actionOrGOTO);

			}
		}
	}

	printf("\n\nsize table: %d", sizeActionAndGOTO_Table);



}


#endif // !_CFG_C_
