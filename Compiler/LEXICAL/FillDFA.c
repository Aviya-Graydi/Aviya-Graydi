#ifndef _FillDFA_C_



#include<stdio.h>
#include<string.h>

//#include "DFA.h"

//#include "DFA.c"
//#include "FillDFA.h"
//#include "Token.h"
//#include "Token.c"
#include "../SYNTACTIC/CFG.c"

void fillRegularExpressions(DFA* dfa)
{
	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = "\t";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת
	char token[255];
	token[0] = '\0';
	int stateId = dfa->countStates - 1;
	int currentState = 0;
	int nextState;

	int tokenTypeId;
	int tokenId = sizeTokens;
	int typeId;

	int isNewState;

	FILE* f1;

	errno_t error = fopen_s(&f1, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\RegularExpressions.txt", "r");
	//errno_t error = fopen_s(&f1, "X:\\סמינר\\התמחויות\\הנדסת תוכנה\\שנה ב\\תלמידות\\אביה גריידי\\פרוייקט\\פרוייקט\\RegularExpressions.csv", "r");
	if (error != 0)
	{
		printf("\nCould not open the file RegularExpressions :(");
		return;
	}

	printf("\nThe file RegularExpressions was opened successfully !!! :)");


	while (fgets(&line, 255, f1))//הלולאה עוברת שורה שורה
	{

		//fscanf_s(f, "%s", &line, 255);//שורה מהקובץ
		//fgets(&line, 255, f);

		slot = strtok_s(line, delimiter, &context);//משבצת
		
		if (*slot != '!')
		{
			
			if (*token != '\0')
			{
				

				dfa->states[nextState].isFinal = true;
				strcpy_s(dfa->states[nextState].name, (rsize_t)_countof(token), token);
				dfa->states[nextState].idInArray = tokenId;

				

				//-------------------------מילוי המערך של סוגי האסימונים
				char value[255];
				strcpy_s(value, (rsize_t)_countof(token), token);

				tokenTypeId = 3;//constant
				

				//printf("\n");

				if (sizeTokens == 0)
				{
					tokens = (Token*)malloc(sizeof(Token));
				}
				else
				{
					tokens = (Token*)realloc(tokens, sizeof(Token) * (sizeTokens + 1));
				}
				if (tokens)
				{
					createToken(&tokens[sizeTokens] , tokenId, tokenTypeId, value);
					tokens[sizeTokens++].idType = typeId;
					tokenId++;
					//printf("%d %s %s", tokens[sizeTokens - 1].id, tokens[sizeTokens - 1].value, kindsOfTokens[tokens[sizeTokens - 1].idKindOfToken].name);
				}
			}
			typeId = atoi(slot);


			slot = strtok_s(NULL, delimiter, &context);//משבצת
			strcpy_s(token, (rsize_t)_countof(token), slot);
			currentState = 0;
		}

		slot = strtok_s(NULL, delimiter, &context);
		isNewState = atoi(slot);
		if (isNewState == 1)
		{
			stateId++;
			addState(dfa, stateId);
		}
		

		while ((slot = strtok_s(NULL, delimiter, &context)) && *slot != '\n')//לולאה שעוברת משבצת משבצת - כל משבצת = תו
		{

			if (dfa->states[currentState].transitions[hash(*slot)].idState == -1 )//צריך ליצור מצב חדש
			{
				if (isNewState == 1) 
				{
					dfa->states[currentState].transitions[hash(*slot)].idState = stateId;
					
				}
				else
				{
					//לא צריך ליצור מצב חדש - המעבר הוא לאותו מצב
					dfa->states[currentState].transitions[hash(*slot)].idState = currentState;
				}
				nextState = dfa->states[currentState].transitions[hash(*slot)].idState;
				addTransition(dfa, currentState, *slot, nextState);
				
			}
			nextState = dfa->states[currentState].transitions[hash(*slot)].idState;

			

			
		}

		currentState = nextState;
		

		if (dfa->countStates <= nextState)
		{
			addState(dfa, nextState);
			stateId++;
		}

	}

	dfa->states[nextState].isFinal = true;
	strcpy_s(dfa->states[nextState].name, (rsize_t)_countof(token), token);
	dfa->states[nextState].idInArray = tokenId;

	stateId++;

	//-------------------------מילוי המערך של סוגי האסימונים
	char value[255];
	strcpy_s(value, (rsize_t)_countof(token), token);

	tokenTypeId = 3;//constant



	//printf("\n");

	if (sizeTokens == 0)
	{
		tokens = (Token*)malloc(sizeof(Token));
	}
	else
	{
		tokens = (Token*)realloc(tokens, sizeof(Token) * (sizeTokens + 1));
	}
	if (tokens)
	{
		createToken(&tokens[sizeTokens++] , tokenId, tokenTypeId, value);
		tokenId++;
		//printf("%d %s %s", tokens[sizeTokens - 1].id, tokens[sizeTokens - 1].value, kindsOfTokens[tokens[sizeTokens - 1].idKindOfToken].name);
	}

	tokens = (Token*)realloc(tokens, sizeof(Token) * (sizeTokens + 1));

	createToken(&tokens[sizeTokens++] , tokenId, 1, "<$>");

	fclose(f1);

}

void fillDFA(DFA* dfa , char* fileName)
{
	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = "\t";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת
	char token[255];

	/*int i = 0;*/

	int stateId = 0;
	int currentState;
	int nextState;

	int tokenTypeId;
	int tokenId = 0;
	int typeId;

	FILE* f1;

	char path[100] = "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\";
	strcat_s(path, _countof(path), fileName);
	strcat_s(path, _countof(path), ".txt");
	errno_t error = fopen_s(&f1, path, "r");
	//errno_t error = fopen_s(&f1, "X:\\סמינר\\התמחויות\\הנדסת תוכנה\\שנה ב\\תלמידות\\אביה גריידי\\פרוייקט\\פרוייקט\\Transitions.csv", "r");
	if (error != 0)
	{
		printf("\nCould not open the file Tokens :(");
		return;
	}

	printf("\nThe file Tokens was opened successfully !!! :)");

	addState(dfa, stateId);
	stateId++;

	while (fgets(&line, 255, f1))//הלולאה עוברת שורה שורה
	{

		//fscanf_s(f, "%s", &line, 255);//שורה מהקובץ
		//fgets(&line, 255, f);



		slot = strtok_s(line, delimiter, &context);//משבצת

		if (!strcmp(fileName, "tokens"))
		{

			tokenTypeId = atoi(slot);//סוג האסימון
			slot = strtok_s(NULL, delimiter, &context);
			typeId = atoi(slot);//סוג האסימון
			slot = strtok_s(NULL, delimiter, &context);
			
		}
		
		strcpy_s(token, (rsize_t)_countof(token), slot);



		while (slot != NULL && strcmp(slot, "\n") )
		{

			currentState = 0;

			while (*slot)//לולאה שעוברת תו תו
			{

				if (dfa->states[currentState].transitions[hash(*slot)].idState == -1)
				{
					addTransition(dfa, currentState, *slot, stateId);
					//dfa->states[currentState].transitions[hash(*slot)].idState = stateId;
					addState(dfa, stateId);
					stateId++;
				}
				nextState = dfa->states[currentState].transitions[hash(*slot)].idState;

				//addTransition(dfa, currentState, *slot, nextState);



				slot++;
				currentState = nextState;
			}

			if (dfa->countStates < nextState)
			{
				addState(dfa, nextState);
			}


			dfa->states[nextState].isFinal = true;
			strcpy_s(dfa->states[nextState].name, (rsize_t)_countof(token), token);



			//-------------------------מילוי המערך של סוגי האסימונים

			char value[255];
			strcpy_s(value, (rsize_t)_countof(token), token);





			//printf("\n");


			if (!strcmp(fileName, "tokens"))
			{
				if (sizeTokens == 0)
				{
					tokens = (Token*)malloc(sizeof(Token));
					createToken(&tokens[sizeTokens++] , 0, 0, "identifier");
					tokens = (Token*)realloc(tokens, sizeof(Token) * (sizeTokens + 1));
					tokenId++;


				}
				else
				{
					tokens = (Token*)realloc(tokens, sizeof(Token) * (sizeTokens + 1));
				}
				if (tokens)
				{
					dfa->states[nextState].idInArray = tokenId;

					createToken(&tokens[sizeTokens] , tokenId, tokenTypeId, value);
					tokenId++;
					tokens[sizeTokens++].idType = typeId;
					//printf("%d %s %s", tokens[sizeTokens - 1].id, tokens[sizeTokens - 1].value, kindsOfTokens[tokens[sizeTokens - 1].idKindOfToken].name);
				}
			}

			//if (!strcmp(fileName, "tokensTypes"))
			//{
			//	if (sizeTokenTypes == 0)
			//	{
			//		tokenTypes = (TokenType*)malloc(sizeof(TokenType));
			//	}
			//	else
			//	{
			//		tokenTypes = (TokenType*)realloc(tokenTypes, sizeof(TokenType) * (sizeTokenTypes + 1));
			//	}
			//	
			//	if (tokenTypes)
			//	{
			//		tokenTypes[sizeTokenTypes++] = createTokensTypes(tokenId, value);
			//		tokenId++;
			//		//printf("\n%d %s ", tokenTypes[sizeTokenTypes - 1].id, tokenTypes[sizeTokenTypes - 1].name);
			//	}
			//}

			//--מילוי המערך של הנונ-טרמינלים

			if (!strcmp(fileName, "NonTerminals"))
			{
				if (sizeNonTerminals == 0)
				{
					nonTerminals = (NonTerminal*)malloc(sizeof(NonTerminal));
				}
				else
				{
					nonTerminals = (NonTerminal*)realloc(nonTerminals, sizeof(NonTerminal) * (sizeNonTerminals + 1));
				}
				if (nonTerminals)
				{
					dfa->states[nextState].idInArray = tokenId;

					createNonTerminal(&nonTerminals[sizeNonTerminals++],tokenId, value);
					tokenId++;
					//printf("%d %s %s", tokens[sizeTokens - 1].id, tokens[sizeTokens - 1].value, kindsOfTokens[tokens[sizeTokens - 1].idKindOfToken].name);
				}
			}

			slot = strtok_s(NULL, delimiter, &context);

		}


	}

	if (!strcmp(fileName, "tokens"))
	{
		
		fillRegularExpressions(dfa);
		

		//dfa->states[nextState].idInArray = tokenId;

		
	}
	
	int j;
	int i;
	for (i = 0; i < dfa->countStates; i++)
	{
		printf("\n %d  ,  %s   ,  %d  , %d", dfa->states[i].id, dfa->states[i].name, dfa->states[i].idInArray, dfa->states[i].countTransitions);
		for(j = 0; j<100;j++)
		{ 
			if (dfa->states[i].transitions[j].idState != -1)
			{
				printf("\n              %c   ->     %d", dfa->states[i].transitions[j].value, dfa->states[i].transitions[j].idState);
			}
		}
	}
	printf("\n-----------------------");
	if (!strcmp(fileName, "tokens"))
	{
		for (int i = 0; i < sizeTokens; i++)
		{
			printf("\n %d ,  %d ,  %s ", tokens[i].id, tokens[i].idTokenType, tokens[i].value);
		}

	}
	if (!strcmp(fileName, "NonTerminals"))
	{
		for ( i = 0; i < sizeNonTerminals; i++)
		{
			printf("\n %d  ,  %s ", nonTerminals[i].id,  nonTerminals[i].name);
		}

	}

	
	fclose(f1);
}

DFA* dfa(char* fileName)
{

	DFA* dfa = (DFA*)malloc(sizeof(DFA));
	if (dfa)
	{
		dfa->startState = 0;
		dfa->currentState = 0;
		dfa->countStates = 0;
		dfa->states = NULL;
	}

	fillDFA(dfa , fileName);
	//finalStates(dfa);
	return dfa;
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------

void fillDFA1(DFA* dfa)
{

	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = ",";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת

	int i = 0;

	int stateId = 0;

	char* t[4] = { '\0' ,'\0' , '\0' };

	FILE* f1;

	errno_t error = fopen_s(&f1, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\Transitions.csv", "r");
	if (error != 0)
	{
		printf("\nCould not open the file Transitions :(");
		return;
	}

	printf("\nThe file Transitions was opened successfully !!! :)");

	addState(dfa, stateId);

	while (fgets(&line, 255, f1))//הלולאה עוברת שורה שורה
	{

		slot = strtok_s(line, delimiter, &context);//משבצת

		while (atoi(slot) != stateId)
		{
			stateId++;
			addState(dfa, stateId);
		}

		i = 0;
		while (slot != NULL)//בכל שורה יש 3 משבצות
		{
			t[i++] = slot;
			// Get next slot:
			slot = strtok_s(NULL, delimiter, &context);
		}

		addTransition(dfa, atoi(t[0]), t[1][0], atoi(t[2]));

	}

	addState(dfa, 110);

}


//פונקציה נפרדת לעדכון המצבים הסופיים
void finalStates(DFA* dfa)
{
	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = ",";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת

	char* stringBuffer[80];

	char* stateId = NULL;
	char* token = NULL;

	FILE* f2;

	errno_t error = fopen_s(&f2, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\finalStates.csv", "r");
	if (error != 0)
	{
		printf("\nCould not open the file finalStates :(");
		return;
	}

	printf("\nThe file finalStates was opened successfully !!! :)");

	while (fgets(&line, 255, f2))//הלולאה עוברת שורה שורה
	{
		//שורה מהקובץ
		stateId = strtok_s(line, delimiter, &context);//משבצת

		dfa->states[atoi(stateId)].isFinal = true;

		// Get next slot:
		token = strtok_s(NULL, delimiter, &context);

		strcpy_s(dfa->states[atoi(stateId)].name, (rsize_t)_countof(dfa->states[atoi(stateId)].name), token);
		*line = '\0';
	}
	fclose(f2);
}

#endif // !_FillDFA_C_