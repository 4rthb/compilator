/*
 * 
 * UFRGS - Compiladores B - Marcelo Johann - 2022/1
 * 
 * Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
 * Tiago Lucas Flach <tlflach@inf.ufrgs.br>
 * 
 */
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

HASH_NODE *Table[HASH_SIZE];

void hashInit()
{
	int i;
	for (i = 0; i < HASH_SIZE; ++i)
		Table[i] = 0;
}

int hashAddress(char *text)
{
	int i;
	int address = 1;
	for (i = 0; i < strlen(text); ++i)
		address = (address * text[i]) % HASH_SIZE + 1;
	
	return address - 1;
}

HASH_NODE *hashFind(char *text)
{
	HASH_NODE *node;
	int address = hashAddress(text);
	for (node = Table[address]; node; node = node->next)
		if (strcmp(node->text, text) == 0)
			return node;

	return 0;
}

HASH_NODE *hashInsert(char *text, int type)
{
	HASH_NODE *newnode;
	int address = hashAddress(text);

	if ((newnode = hashFind(text)) != 0)
		return newnode;

	newnode = (HASH_NODE *) calloc(1, sizeof(HASH_NODE));
	newnode->type = type;
	newnode->text = (char *) calloc(strlen(text) + 1, sizeof(char));
	strcpy(newnode->text, text);

	newnode->next = Table[address];
	Table[address] = newnode;

	return newnode;
}

void hashPrint(void)
{
	int i;
	HASH_NODE *node;
	for (i = 0; i < HASH_SIZE; ++i)
		for (node = Table[i]; node; node = node->next)
			printf("Table[%d] has %s: type %d, datatype %d\n", i, node->text, node->type, node->datatype);
}

void printAsm(FILE *fout)
{
	int i;
	HASH_NODE *node;
	fprintf(fout, "## DATA SECTION\n"
		"	.section	__DATA,__data\n\n");
	for (i = 0; i < HASH_SIZE; ++i)
		for (node = Table[i]; node; node = node->next)
			if (node->type == HASH_VAR)
				fprintf(fout, " 	%s:	.long	%s\n", node->text, 
						node->dec? node->dec->son[1]? node->dec->son[1]->symbol->text : "0" : "0");
}

void hashPrintByType(int type)
{
	int i;
	HASH_NODE *node;
	printf("\nType %d:\n", type);
	for (i = 0; i < HASH_SIZE; ++i)
		for (node = Table[i]; node; node = node->next)
			if (node->type == type)
				printf("%s\n", node->text);
}

int hashCheckUndeclared()
{
	int undeclared = 0;
	int i;
	HASH_NODE *node;
	for (i = 0; i < HASH_SIZE; ++i)
		for (node = Table[i]; node; node = node->next)
			if(node->type == HASH_IDT)
			{
				fprintf(stderr,"Semantic ERROR: identifier %s undeclared\n", node->text);
				++undeclared;
			}
	return undeclared;
}

HASH_NODE* makeTemp(void)
{
	static int serial = 0;
	char buffer[256] = "";

	sprintf(buffer, "mYWeeirT_emp%d", serial++);
	return hashInsert(buffer, HASH_VAR);
}

HASH_NODE* makeLabel(void)
{
	static int serial = 0;
	char buffer[256] = "";

	sprintf(buffer, "mYLabule%d", serial++);
	return hashInsert(buffer, HASH_LABEL);
}