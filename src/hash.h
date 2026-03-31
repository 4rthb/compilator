/*
 * 
 * UFRGS - Compiladores B - Marcelo Johann - 2022/1
 * 
 * Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
 * Tiago Lucas Flach <tlflach@inf.ufrgs.br>
 * 
 */
#pragma once

#include <stdio.h>

#define HASH_SIZE 997

enum hashSymbol {
	HASH_IDT,
	HASH_VAR,
	HASH_VEC,
	HASH_FUN,

	HASH_LIT_I,
	HASH_LIT_C,
	HASH_LIT_F,
	HASH_LIT_S,

	HASH_LABEL
};

enum hashData {
	HASH_UNKNOW,
	HASH_DATA_I,
	HASH_DATA_C,
	HASH_DATA_F
};

typedef struct ast_node AST;

typedef struct hash_node
{
	int type;
	int datatype;
	char *text;
	AST *dec;
	struct hash_node *next;
} HASH_NODE;

void hashInit(void);
int hashAddresses(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
void hashPrintByType(int type);
int hashCheckUndeclared();
HASH_NODE* makeTemp(void);
HASH_NODE* makeLabel(void);

void printAsm(FILE *fout);