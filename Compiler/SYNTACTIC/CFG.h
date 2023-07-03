#ifndef _CFG_H_

#include<stdbool.h>
#include"../LEXICAL/DFA.h"

typedef struct {
	int id;
	char value[255];
	bool isTerminal;
	int idTerminalOrNonTerminal;
	int countValues;
}CFG_Value;

typedef struct {
	int id;
	char name[255];
	int countOptions;
	CFG_Value** options;
}NonTerminal;

typedef struct {
	char** rules;
	int size;
}ClosedGroupTable;//קבוצת הסגור לכל נונ-טרמינל

typedef struct {
	int index;
	int lastIndex;
	int idNonTerminal;
	int option;
	int idValue;
}ClosedGroup;//קבוצת הסגור שצריך לעדכן

//V-נונ-טרמינלים
//T-אסימונים
//P-כללי הגזירה
//S-מצב התחלה



typedef struct {
	char actionOrGOTO[255];
}Table;

NonTerminal* nonTerminals;
int** first;
int** follow;
Table** actionAndGOTO_Table;
ClosedGroupTable* closedGroupTable;//קבוצת הסגור לכל נונ-טרמינל
ClosedGroup* closedGroups;//קבוצת הסגור שצריך לעדכן


int sizeNonTerminals = 0;
int sizeClosedGroups = 0;
int sizeActionAndGOTO_Table = 0;

void createNonTerminal(NonTerminal* nonTerminal , int id, char* name);
void createClosedGroupTable(ClosedGroupTable* c);
void createClosedGroup(ClosedGroup* closedGroup , int index , int idNonTerminal);

void fillCFG(DFA* tokensDFA, DFA* CFG_DFA);
void createFirstAndFollow();
void findFirst(int id, int idNonTerminal, int idLast, int index);
void findFollow();
void buildTable();

#endif // !_CFG_H_