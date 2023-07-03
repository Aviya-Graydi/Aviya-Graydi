#pragma once
#include"../SEMANTIC/FillSemanticConstraints.c"

char codeGeneration[1000] = "\0";
char codeCopy[1000] = "\0";
char codeFunctions[1000] = "\0";

int currentConstant = 0;

AST** functionsDefinitions = NULL;
int sizeFunctionsDefinitions = 0;


void build_C_Sharp(DFA* CFG_DFA);
void Generate();
