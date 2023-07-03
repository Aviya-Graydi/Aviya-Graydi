
#include<stdio.h>
#include <stdbool.h>

#include "Main.h"

#include "../CODE_GENERATION/CodeGeneration.c"

void main()
{
	DFA* tokensDFA;
	DFA* CFG_DFA;

	char code[1000] = "";

	printf("\n\nLEXICAL ANALYZER\n\n");

	fillTokensTypes();//מילוי סוגי האסימונים

	tokensDFA = dfa("tokens");//ממלא את האוטומט ואת המצבים הסופיים
	
	printf("\n\nSYNTACTIC ANALYZER\n\n");

	CFG_DFA = dfa("NonTerminals");//אוטומט הנונ-טרמינלים
	fillCFG(tokensDFA, CFG_DFA);//הגדרת הדקדוק התחבירי
	createFirstAndFollow();//בניית 
	buildTable();//----טבלת הניתוח

	//free(CFG_DFA);

	printf("\n\nSEMANTIC ANALYZER\n\n");

	fillTypes();//מילוי סוגי הטיפוסים
	fillTables(tokensDFA);//מילוי טבלאות ביטויי הטיפוסים התקינים

	printf("\n\n---CHECK YUOR CODE---\n\n");

	strcpy_s(code, (rsize_t)_countof(code), " x = 7  a = 5  b = 'number'  c =7.5 print('The_Numbers:', a + 5 ,  b, c)  while(b == 5){a=250} function myFunction (){ while(true.){print('true??')}   }	if(myFunction()..){	print('WOW!')   } elif(x==7){ nn =  input (  'Enter_A_Number' )	.  } else { for(x in range(444)){ yeah = input('yeah!').}	}  myFunction() a = myFunction().");
	
	printf("\n\nLEXICAL TEST\n\n");

	lex(tokensDFA, code);

	printf("\n\nSYNTACTIC TEST\n\n");

	parser();

	printf("\n\nSEMANTIC TEST\n\n");

	semanticCheck();

	printf("\n\nCODE GENERATION\n\n");

	GeneratingTheCode();

	printf("%s", codeGeneration);

}