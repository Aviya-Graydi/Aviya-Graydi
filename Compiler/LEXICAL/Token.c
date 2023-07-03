
#ifndef _Token_C_


#include "../MAIN/Global.h"

#include "Token.h"
//#include "DFA.h"
//#include "FillDFA.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include <crtdbg.h>  // For _CrtSetReportMode
#include <errno.h>


void createTokenType(TokenType* tokenType ,int id, char* name)
{
	char stringBuffer[80];
	//TokenType* tokenType = (TokenType*)malloc(sizeof(TokenType));
	if (tokenType)
	{
		tokenType->id = id;
		strcpy_s(tokenType->name, (rsize_t)_countof(stringBuffer), name);
	}
	//return *tokenType;
}

void createToken(Token* token ,int id, int idTokenType, char* value)
{

	char stringBuffer[80];
	//Token* token = (Token*)malloc(sizeof(Token));
	if (token)
	{
		token->id = id;
		token->idTokenType = idTokenType;
		token->idType = 0;
		strcpy_s(token->value, (rsize_t)_countof(stringBuffer), value);
	}
	//return *token;
}
void createSymbolAttributes(SymbolAttributes* symbol)
{
	symbol->isDefined = false;
	symbol->isInitialis = false;
	symbol->idTypes = NULL;
	symbol->count = 0;
}
void createScope(Scope* scope, int id)
{
	//Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
	if (scope)
	{
		scope->id = id;
		scope->symbols = (SymbolAttributes*)malloc(sizeof(SymbolAttributes) * sizeSymbolTable);
		for (int i = 0; i < sizeSymbolTable; i++)
		{
			createSymbolAttributes(&scope->symbols[i]);
		}	
	}
}

void createSymbol(Symbol* symbol , int id , char* name ) 
{
	//Symbol* symbol = (Symbol*)malloc(sizeof(Symbol));
	if (symbol)
	{
		symbol->id = id;
		strcpy_s(symbol->name, (rsize_t)_countof(symbol->name), name);
		symbol->idType = 0;
		for (int i = 0; i < 100; i++)
		{
			symbol->correctScopes[i] = false;
			symbol->isGenerate[i] = false;
			symbol->isFunc = false;
		}
		/*symbol->isDefined = false;
		symbol->isInitialis = false;	
		symbol->count = 0;*/
	}
	//return *symbol;
}


void createType(Type* type,int id ,char* name)
{
	//Type* type = (Type*)malloc(sizeof(Type));
	if (type)
	{
		type->id = id;
		strcpy_s(type->name, (rsize_t)_countof(type->name), name);
	}	
	//return *type;
}

void addLexema(DFA* dfa, int idStateLexema, int identifier , char* value)
{
	int id = -1;
	int idTokenType;
	char lexema[255];

	int flag = 0;

	if (sizeCodeTokens == 0)
	{
		codeTokens = malloc(sizeof(Token));
	}
	else
	{
		codeTokens = (Token*)realloc(codeTokens, sizeof(Token) * (sizeCodeTokens + 1));
	}

	if (identifier == 1)
	{
		strcpy_s(lexema, (rsize_t)_countof(lexema), value);
		for (int i = 0; i < sizeSymbolTable; i++)
		{
			if (!strcmp(symbolTable[i].name, lexema))
			{
				flag = 1;
				id = symbolTable[i].id;
				break;
			}
		}
		
		if (id == -1)
		{
			id = sizeSymbolTable;
		}

		if (flag == 0)
		{
			if (sizeSymbolTable == 0)
			{
				symbolTable = (Symbol*)malloc(sizeof(Symbol));
			}
			else
			{
				symbolTable = (Symbol*)realloc(symbolTable, sizeof(Symbol) * (sizeSymbolTable + 1));
			}
			createSymbol(&symbolTable[sizeSymbolTable++] ,id,lexema); //אם קיים כבר????   !!!!!!!!
		}
		
		 createToken(&codeTokens[sizeCodeTokens++] , id, 2, lexema);
	}
	else if(idStateLexema != -1)
	{
		
		id = dfa->states[idStateLexema].idInArray;
		idTokenType = tokens[id].idTokenType;
		strcpy_s(lexema, (rsize_t)_countof(lexema), tokens[id].value);
		createToken(&codeTokens[sizeCodeTokens] , id, idTokenType, lexema);
		codeTokens[sizeCodeTokens++].idType = tokens[id].idType;


		if (idTokenType == 3)
		{
			if (sizeConstant == 0)
			{
				constant = (Constant*)malloc(sizeof(Constant));
			}
			else
			{
				constant = (Constant*)realloc(constant, sizeof(Constant) * (sizeConstant + 1));
			}
			
			constant[sizeConstant].idType = tokens[id].idType;
			strcpy_s(constant[sizeConstant++].value, (rsize_t)_countof(lexema), value);

		}
	}
	else
	{
		//לא לקסמה ולא מזהה
		//לסמן שגיאה
		idTokenType = 5;
		createToken(&codeTokens[sizeCodeTokens++]  , -1, idTokenType, "exeption :(");
	}
}

void lex(DFA* dfa, char* code)
{
	char* s = code;
	char* stateLexemaIndex = ' ';
	char* identifierIndex = ' ';
	char* finalIndex = ' ';
	char  identifierValue[255];//(constants - ערך הלקסמה (רלוונטי למשתנה ולערכים קבועים
	


	//identifierValue[0] = "\0";
	//char* ch = "y\n";

	int final;
	int idStateLexema = -1;//שמירת המצב הסופי האחרון אליו הגענו
	int identifier = 1;
	int idStateLexemaLength = 0;
	int identifierLength = 0;

	int count = 0;

	int create = 0;


	while(' ' == *s || '\t' == *s || '\n' == *s /*|| ' \r ' == *s */ || '\f' == *s)
	{
		s++;
		
	}

	if (*s != '_' && !isalpha(*s))
	{
		identifier = 0;
	}
	//else
	//{
	//	//strncpy_s(identifierValue, _countof(identifierValue), s , 1);
	//}


	

	dfa->currentState = 0;

	while (*s)
	{
		if (' ' == *s || '\t' == *s || '\n' == *s /*|| ' \r ' == *s */ || '\f' == *s)
		{
			s++;
			
			identifierIndex++;
			stateLexemaIndex++;
			identifier = 0;
			dfa->currentState = -1;

			count++;
		}
		else
		{ 

			if (dfa->currentState != -1)
			{
				dfa->currentState = (dfa->states[dfa->currentState]).transitions[hash(*s)].idState;
				
			}

			if (dfa->currentState != -1)
			{
				idStateLexemaLength++;
			}

			if (identifier==1 && *s != '_' && !isalpha(*s) && !isdigit(*s))
			{
				identifier = 0;
			}
			else if (identifier == 1)
			{
				
				//ch[0] = *s;
				if (identifierLength == 0)
				{
					strncpy_s(identifierValue, _countof(identifierValue), s, 1);
				}
				else 
				{ 
					strncat_s(identifierValue, _countof(identifierValue), s, 1); 
					//printf(":)"); 
				}
				identifierIndex = s+1;
				identifierLength++;
			}

			if (dfa->currentState == -1 && identifier == 0)
			{
				
				//סוף הלקסמה זה מה שנמצא בפינאל
				
				if (!(identifierLength > idStateLexemaLength) && idStateLexema != -1)
				{
					final = idStateLexema;
					finalIndex = stateLexemaIndex;
					if (final!= -1 && tokens[dfa->states[final].idInArray].idTokenType == 3)
					{
						//strncpy_s(identifierValue, (rsize_t)_countof(identifierValue), code + (strlen(code) - idStateLexemaLength), idStateLexemaLength);

						strncpy_s(identifierValue, (rsize_t)_countof(identifierValue), code +(strlen(code) -strlen( stateLexemaIndex ) -count-idStateLexemaLength), idStateLexemaLength );
					}
				}
				else /*if (identifierLength > idStateLexemaLength)*/
				{
					final = -1;
					finalIndex = identifierIndex;
					identifier = 1;
				}

				addLexema(dfa, final , identifier , identifierValue);
				count = 0;
				//printf("\n %s ->  %s\n", codeTokens[sizeCodeTokens-1].value, tokenTypes[codeTokens[sizeCodeTokens-1].idTokenType].name);
				if (codeTokens[sizeCodeTokens - 1].idTokenType == 5)
				{
					finalIndex = s;
				}
				//אם זה רווח או טאב או ירידת שורה אפשר להתקדם הלאה. אם לא- לחזור אחורה עד האינדקס שבו זיהינו לקסמה לאחרונה

				s = finalIndex -1;
				dfa->currentState = 0;
				identifierLength = 0;
				idStateLexemaLength = 0;
				idStateLexema = -1;
				identifier = 1;
				if (*(s+1) != '_' && !isalpha(*(s+1)))
				{
					identifier = 0;
				}
				
			}


			else if(dfa->currentState!= -1 && (dfa->states[dfa->currentState]).isFinal)
			{
				stateLexemaIndex = s+1;
				idStateLexema = dfa->currentState;
			}
			
			
			
			s++;
		}
	
	}

	
	
		if (!(identifierLength > idStateLexemaLength) && idStateLexema != -1)
		{				

			final = idStateLexema;
			finalIndex = stateLexemaIndex;
			if (tokens[dfa->states[final].idInArray].idTokenType == 3)
			{
				strncpy_s(identifierValue, (rsize_t)_countof(identifierValue), code + (strlen(code) - idStateLexemaLength), idStateLexemaLength);
			}
			identifier = 0;
		}
		else 
		{
			final = -1;
			finalIndex = identifierIndex;
			identifier = 1;
	
		}

		addLexema(dfa, final, identifier, identifierValue);
	

	

	//printf("\n%d\n", sizeCodeTokens);

	int i;

	for(i = 0; i < sizeCodeTokens; i++)
	{
		printf("\n %s ->  %s\n", codeTokens[i].value, tokenTypes[codeTokens[i].idTokenType].name);
	}
	printf("-----------------------------------------");
	for (i = 0; i < sizeSymbolTable; i++)
	{
		printf("\n %d - %s \n", symbolTable[i].id,  symbolTable[i].name);
	}
	
}

#endif // !_Token_C_



void fillTokensTypes()
{
	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = ",";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת

	int tokenTypeId = 0;

	char* name = '\0';

	FILE* f3;

	//errno_t error = fopen_s(&f3, "X:\\סמינר\\התמחויות\\הנדסת תוכנה\\שנה ב\\תלמידות\\אביה גריידי\\פרוייקט\\פרוייקט\\kinds.csv", "r");
	errno_t error = fopen_s(&f3, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\tokensTypes.txt", "r");
	if (error != 0)
	{
		printf("\nCould not open the file tokensTypes :(");
		return;
	}

	printf("\nThe file tokensTypes was opened successfully !!! :)");

	while (fgets(&line, 255, f3))//הלולאה עוברת שורה שורה
	{

		//fscanf_s(f, "%s", &line, 255);//שורה מהקובץ
		//fgets(&line, 255, f);
		slot = strtok_s(line, delimiter, &context);//משבצת

		//printf("\n");


		if (sizeTokenTypes == 0)
		{
			tokenTypes = (TokenType*)malloc(sizeof(TokenType));
		}
		else
		{
			tokenTypes = (TokenType*)realloc(tokenTypes, sizeof(TokenType) * (sizeTokenTypes + 1));
		}
		createTokenType(&tokenTypes[sizeTokenTypes++] , tokenTypeId, slot);
		tokenTypeId++;
		printf("\n%d %s ", tokenTypes[sizeTokenTypes - 1].id, tokenTypes[sizeTokenTypes - 1].name);

	}
	fclose(f3);


}


void fillTypes()
{
	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = "\t";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת

	int typeId = 0;

	char* name = '\0';

	FILE* f3;

	//errno_t error = fopen_s(&f3, "X:\\סמינר\\התמחויות\\הנדסת תוכנה\\שנה ב\\תלמידות\\אביה גריידי\\פרוייקט\\פרוייקט\\kinds.csv", "r");
	errno_t error = fopen_s(&f3, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\Types.txt", "r");
	if (error != 0)
	{
		printf("\nCould not open the file tokensTypes :(");
		return;
	}

	printf("\nThe file tokensTypes was opened successfully !!! :)");

	while (fgets(&line, 255, f3))//הלולאה עוברת שורה שורה
	{

		slot = strtok_s(line, delimiter, &context);//משבצת

		if (sizeTokenTypes == 0)
		{
			types = (Type*)malloc(sizeof(Type));
		}
		else
		{
			types = (Type*)realloc(types, sizeof(Type) * (sizeTypes + 1));
		}
		createType(&types[sizeTypes++] , typeId, slot);
		typeId++;
		printf("\n%d %s ", types[sizeTypes - 1].id, types[sizeTypes - 1].name);

	}
	fclose(f3);
}


void fillTokens(DFA* dfa)
{
	char line[255] = "line";//שורה מהקובץ
	char delimiter[] = ",";//סימן הפרדה
	char* context = NULL;//תוכן מופרד מהשורה
	char* slot = NULL;//משבצת

	int tokenId = 1;
	char* value;
	int tokenTypeId;

	int stateId;

	char* name = '\0';

	FILE* f1;

	errno_t error = fopen_s(&f1, "D:\\אביה\\הנדסת תוכנה\\פרוייקט\\Tokens.csv", "r");
	if (error != 0)
	{
		printf("\nCould not open the file Tokens :(");
		return;
	}

	printf("\nThe file Tokens was opened successfully !!! :)");

	while (fgets(&line, 255, f1))//הלולאה עוברת שורה שורה
	{
		slot = strtok_s(line, delimiter, &context);//משבצת
		value = slot;
		slot = strtok_s(NULL, delimiter, &context);
		tokenTypeId = atoi(slot);
		slot = strtok_s(NULL, delimiter, &context);
		stateId = atoi(slot);

		dfa->states[stateId].idInArray = tokenId;

		if (sizeTokens == 0)
		{
			tokens = (Token*)malloc(sizeof(Token));
			createToken(&tokens[sizeTokens++], 0, 0, "identifier");
			tokens = (Token*)realloc(tokens, sizeof(Token) * (sizeTokens + 1));
		}
		else
		{
			tokens = (Token*)realloc(tokens, sizeof(Token) * (sizeTokens + 1));
		}
		if (tokens)
		{
			createToken(&tokens[sizeTokens++], tokenId, tokenTypeId, value);
			tokenId++;
		}
	}
}




void checkToken(DFA* dfa, char* lexema)
{
	char* l = lexema;
	int idToken = -1;
	int idTokenType;
	char name[80];
	char value[80];
	char stringBuffer[80];
	int identifier = 1;
	int flag = 0;
	dfa->currentState = dfa->startState;
	if (*l != '_' && !isalpha(*l))
	{
		identifier = 0;
	}
	while (*l)
	{
		if (*l != '_' && !isalpha(*l) && !isdigit(*l))
		{
			identifier = 0;
		}
		if ((dfa->states[dfa->currentState]).transitions[hash(*l)].value == *l)
		{
			dfa->currentState = (dfa->states[dfa->currentState]).transitions[hash(*l)].idState;
			l++;
		}
		else
		{
			flag = 1;
			break;
		}
	}
	if (sizeCodeTokens == 0)
	{
		codeTokens = malloc(sizeof(Token));
	}
	else
	{
		codeTokens = (Token*)realloc(codeTokens, sizeof(Token) * (sizeCodeTokens + 1));
	}
	if (flag == 0 && dfa->states[dfa->currentState].isFinal)
	{
		idToken = dfa->states[dfa->currentState].idInArray;
		idTokenType = tokens[idToken].idTokenType;
		strcpy_s(value, (rsize_t)_countof(stringBuffer), tokens[idToken].value);
		createToken(&codeTokens[sizeCodeTokens++], idToken, idTokenType, value);
	}
	else if (identifier == 1)
	{
		for (int i = 0; i < sizeSymbolTable; i++)
		{
			if (strcmp(symbolTable[i].name, lexema))
			{
				idToken = symbolTable[i].id;
				break;
			}
		}
		if (idToken == -1)
		{
			idToken = sizeSymbolTable;
		}

		if (sizeSymbolTable == 0)
		{
			symbolTable = malloc(sizeof(Symbol));
		}
		else
		{
			symbolTable = (Symbol*)realloc(symbolTable, sizeof(Symbol) * (sizeSymbolTable + 1));
		}
		createSymbol(&symbolTable[sizeSymbolTable++] ,idToken , lexema);
		createToken(&codeTokens[sizeCodeTokens++], idToken, 2, lexema);
	}
	else
	{
		//לסמן כשגיאה
		createToken(&codeTokens[sizeCodeTokens++] , -1, -1, "exeption :(");
	}
}


//צריך להפריד בין אם זה אופרטור או אות או מספר או # או " לחשוב על רעיון לאלגוריתם חכם
		//איך מסמנים אם הטוקן הוא שם משתנה או שם פונקציה

		//להעביר את הטוקנים למערך שאותו ננתח בשלב הניתוח הבא



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//void lex(DFA* dfa, char* code)
//{
//	char* s = code;
//	char lexema[255] = "lexema";
//	int number = 0;
//	int alpha = 0;
//	int operator = 0;
//	int punctuation = 0;
//	int comment = 0;
//	int string = 0;
//	int longComment = 0;
//	int identifier = 1;
//	int create = 0;
//	int line = 0;//מספר שורות
//	int index = 0;
//	int firstIndex = 0;
//	int lastIndex = 0;
//	int length = 0;
//	errno_t err;
//	while (*s != '\0')//אני עוברת על כל הקוד עד הסוף
//	{
//		length++;
//		if (longComment == 1)//אם זה הערה ארוכה לא משנה איזה תו יש בהמשך רק נחכה לסיום ## ואז נשלח לזיהוי טוקן
//		{
//			if (*s == '#' && *(s + 1) == '#')
//			{
//				create = 1;
//				index++;
//				s++;
//				length++;
//			}
//		}
//		        //הערה קצרה או מחרוזת
//		else if (comment == 1)
//		{
//			if (*s == '\n')
//			{
//				create == 1;
//			}
//		}
//		else if (string == 1)
//		{
//			if (*s == '"')
//			{
//				create == 1;
//			}
//		}
//		else if (*s == '#')
//		{
//			if (alpha == 1 || number == 1 || operator == 1 || punctuation == 1)
//			{
//				create = 1;
//				s--;
//				index--;
//				length--;
//			}
//			if (s[1] == '#')
//			{
//				longComment = 1;
//			}
//			else
//			{
//				comment = 1;
//			}
//		}
//
//		else if (*s == '"')
//		{
//			string = 1;
//		}
//		else if (isdigit(*s))
//		{
//			if (operator == 1)
//			{
//				create = 1;
//				s--;
//				index--;
//				length--;
//			}
//			else
//			{
//				number = 1;
//			}
//		}
//		else if (isalpha(*s) || *s == '_')
//		{
//			if (operator == 1)
//			{
//				create = 1;
//				s--;
//				index--;
//				length--;
//			}
//			else
//			{
//				alpha = 1;
//			}
//		}
//		//תווים שמדלגים עליהם
//		else if ('  ' == *s || ' \t ' == *s || ' \n ' == *s /*|| ' \r ' == *s */|| ' \f ' == *s)
//		{
//			if (number == 1 || alpha == 1 || operator == 1)
//			{
//				create = 1;
//			}
//			else
//			{
//				firstIndex++;
//			}
//			if (*s == '\n')
//			{
//				line++;
//			}
//		}
//		//סימני פיסוק
//		else if (*s == ';' || *s == '(' || *s == ')' || *s == '{' || *s == '}' || *s == '[' || *s == ']' || *s == ':' || *s == '?' || *s == ',' || *s == '.')
//		{
//			if ((*s == '.' && number == 1))
//			{
//				number = 1;
//			}
//			else if (number == 1 || alpha == 1 || operator == 1)
//			{
//				create = 1;
//				s--;
//				index--;
//				length--;
//			}
//			else
//			{
//				punctuation = 1;
//				create = 1;
//			}
//		}
//
//		//אופרטור
//		else
//		{
//			if (number == 1 || alpha == 1 || punctuation== 1)
//			{
//				create = 1;
//				s--;
//				index--;
//				length--;
//			}
//			else
//			{
//				operator = 1;
//			}
//		}
//		if (create == 1)
//		{
//			err = strncpy_s(lexema ,_countof(lexema), code + firstIndex, length);
//			lexema[length] = '\0';
//			checkToken(dfa, lexema);
//			//איפוס הדגלים
//			number = 0;
//			alpha = 0;
//			operator = 0;
//			comment = 0;
//			string = 0;
//			longComment = 0;
//			punctuation = 0;
//			create = 0;
//			firstIndex = index+1;
//			length = 0;
//		}
//		s++;
//		index++;
//	}
//	if (length != 0)
//	{
//		err = strncpy_s(lexema, _countof(lexema), code + firstIndex, length);
//		lexema[length] = '\0';
//		checkToken(dfa, lexema);
//	}
//	for (int i = 0; i< sizeCodeTokens; i++)
//	{
//		printf("\n %s ->  %s\n", codeTokens[i].value , tokenTypes[codeTokens[i].idTokenType].name);
//	}
//}