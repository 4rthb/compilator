/*
 * 
 * UFRGS - Compiladores B - Marcelo Johann - 2022/1
 * 
 * Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
 * Tiago Lucas Flach <tlflach@inf.ufrgs.br>
 * 
 */

#pragma once
#include "hash.h"
#include "ast.h"

enum tacSymbol {
	TAC_SYMBOL,
	TAC_MOVE,

	TAC_LABEL,
	TAC_IFZ,
	TAC_JUMP,
	TAC_JUMPZ,
	TAC_JFALSE,
	TAC_CALL,
	TAC_VECTOR,
	TAC_ARG,
	TAC_RET,
	TAC_EXPN,
	// Functions
	TAC_READ,
	TAC_PRINT,
	TAC_RETURN,
	TAC_COPY,
	TAC_BEGINFUN,
	TAC_ENDFUN,
	// Mathematics
	TAC_ADD,
	TAC_SUB,
	TAC_DIV,
	TAC_MUL,
	// Logic
	TAC_AND,
	TAC_OR,
	TAC_NOT,
	// Operators
	TAC_LSE,
	TAC_GTE,
	TAC_EQU,
	TAC_DIF,
	// Comparison
	TAC_LSR,
	TAC_GTR,
	TAC_VAR,
	// Initialization
	TAC_INTV,
	// Lists
	TAC_MSGL,
	TAC_EXPL,
	TAC_ARGL,
};

typedef struct tac_node
{
	int type;
	HASH_NODE * res;
	HASH_NODE * op1;
	HASH_NODE * op2;
	struct tac_node* prev;
	struct tac_node* next;
} TAC;


TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2);
void tacPrint(TAC* tac);
void tacPrintBackwards(TAC* tac);
TAC* tacJoin(TAC* l1, TAC* l2);

// CODE GENERATE
TAC* generateCode(AST *node);

// ASM GENERATE
TAC* tacReverse(TAC *tac);
void generateAsm(TAC *first);