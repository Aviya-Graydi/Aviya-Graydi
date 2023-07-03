#ifndef _Tokens_H_

#include <stdbool.h>
#include<stdio.h>
#include "DFA.h"
#include "DFA.c"

typedef struct {

	int id;
	char name[255];

}TokenType;

typedef struct{

	int id;
	int idTokenType;
	int idType;
	char value[255];

}Token;

typedef struct {
	int id;
	char name[255];
}Type;

//typedef struct {
//	int id;
//	char name[255];
//	bool isDefined;
//	bool isInitialis;
//	int* idTypes;
//	bool canChange;
//	int count;
//}Symbol;

typedef struct {
	bool isDefined;
	bool isInitialis;
	int* idTypes;
	int count;
}SymbolAttributes;

typedef struct {
	int id;
	SymbolAttributes* symbols;
}Scope;

typedef struct {
	int id;
	char name[255];
	bool isFunc;
	bool correctScopes[100];
	int idType;
	bool isGenerate[100];//האם הוגדר בקוד בסקופ המתאים
}Symbol;

typedef struct {
	int idType;
	char value[255];
}Constant;

Constant* constant;
int sizeConstant = 0;

TokenType* tokenTypes;
Token* tokens;
Token* codeTokens;
Type* types;
Symbol* symbolTable;
Scope* scopes;

int sizeTokenTypes = 0;
int sizeTokens = 0;
int sizeCodeTokens = 0;
int sizeTypes = 0;
int sizeSymbolTable = 0;



void createToken(Token* token , int id, int idTokenType, char* value);
void createTokenType(TokenType* tokenType ,int id, char* name);
void createSymbol(Symbol* symbol , int id, char* name);
void createType(Type* type ,int id ,char* name);

void fillTokensTypes();
void fillTypes();


void addLexema(DFA* dfa, int idStateLexema, int identifier, char* value);

void lex(DFA* dfa, char* s);


//---------------------------
void fillTokens(DFA* dfa);//לא בשימוש?
void checkToken(DFA* dfa ,char* lexema);//לא בשימוש?


#endif // !1