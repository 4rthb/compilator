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

#define MAX_SONS 4

// AST
// ----------
enum astConst {
	AST_SYMBOL,
	// Mathematics
	AST_ADD,
	AST_SUB,
	AST_DIV,
	AST_MUL,
	// Comparison
	AST_LSR,
	AST_GTR,
	// Logic
	AST_AND,
	AST_OR,
	AST_NOT,
	// Operators
	AST_LSE,
	AST_GTE,
	AST_EQU,
	AST_DIF,
	AST_ASSIGN,
	// Commands
	AST_DECVAR,
	AST_DECVEC,
	AST_DECFUN,
	AST_CMD,
	AST_LCMD,
	AST_LCMDT,
	AST_EXPN,
	// Variables
	AST_VAR,
	AST_VECTOR,
	// Message
	AST_MSG,
	AST_MSGL,
	// Conditions
	AST_IF,
	AST_IF_ELSE,
	AST_WHILE,
	// Functions
	AST_READ,
	AST_PRINT,
	AST_RETURN,
	AST_CALL,
	// Type
	AST_CHAR,
	AST_FLOAT,
	AST_INT,
	// Initialization
	AST_INTV,
	// Lists
	AST_EXPL,
	AST_ARGL,
	AST_DECL,
	AST_BLCK,
	AST_PROGRAM
};

typedef struct ast_node
{
	int type;
	HASH_NODE* symbol;
	struct ast_node *son[MAX_SONS];
} AST;

AST* astCreate(int type, HASH_NODE* symbol, AST* s0, AST* s1, AST* s2, AST* s3);
void astPrint(AST* node, int level);
void astDecompile(AST *node);
