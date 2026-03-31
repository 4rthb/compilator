/*
 * 
 * UFRGS - Compiladores B - Marcelo Johann - 2022/1
 * 
 * Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
 * Tiago Lucas Flach <tlflach@inf.ufrgs.br>
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "semantic.h"
#include "tacs.h"
#include "lex.yy.h"

extern int yyparse();
extern void hashPrint();

void initMe(void);
int isRunning(void);
int getLineNumber(void);

extern FILE *yyin;
typedef struct ast_node AST;
FILE *out;
extern void astDecompile(AST *node);
extern AST *ASTroot;
TAC *code;

int main(int argc, char** argv)
{
	int token = 0;
	int semanticErrors = 0;

	initMe();

	// Falta do arquivo de entrada
	if (argc < 3)
	{
		printf("Call: ./compiler input.txt output.txt\n");
		exit(1);
	}

	// Falha ao abrir o arquivo
	if ((yyin = fopen(argv[1], "r")) == 0)
	{
		printf("Cannot open file %s... \n", argv[1]);
		exit(2);
	}

	// Falha ao abrir o arquivo
	if ((out = fopen(argv[2], "w")) == 0)
	{
		printf("Cannot open file %s... \n", argv[1]);
		exit(2);
	}

	yyparse();
	
	astDecompile(ASTroot);
	check_semantic(ASTroot);

	hashPrint();
	astPrint(ASTroot, 0);

	code = generateCode(ASTroot);
	tacPrintBackwards(code);
	code = tacReverse(code);
	generateAsm(code);


	if((semanticErrors = get_semantic_errors()) != 0)
	{
		printf("Compilation aborted! %d semantic errors!\n", semanticErrors);
		exit(4);
	}

	printf("File has %d lines\n", getLineNumber());

	exit(0);
}