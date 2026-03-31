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
#include "ast.h"
extern FILE *out;

char * nodeType[AST_PROGRAM+1] = {
	"AST_SYMBOL",
	"AST_ADD",
	"AST_SUB",
	"AST_DIV",
	"AST_MUL",
	"AST_LSR",
	"AST_GTR",
	"AST_AND",
	"AST_OR",
	"AST_NOT",
	"AST_LSE",
	"AST_GTE",
	"AST_EQU",
	"AST_DIF",
	"AST_ASSIGN",
	"AST_DECVAR",
	"AST_DECVEC",
	"AST_DECFUN",
	"AST_CMD",
	"AST_LCMD",
	"AST_LCMDT",
	"AST_EXPN",
	"AST_VAR",
	"AST_VECTOR",
	"AST_MSG",
	"AST_MSGL",
	"AST_IF",
	"AST_IF_ELSE",
	"AST_WHILE",
	"AST_READ",
	"AST_PRINT",
	"AST_RETURN",
	"AST_CALL",
	"AST_CHAR",
	"AST_FLOAT",
	"AST_INT",
	"AST_INTV",
	"AST_EXPL",
	"AST_ARGL",
	"AST_DECL",
	"AST_BLCK",
	"AST_PROGRAM"
};

AST* astCreate(int type, HASH_NODE* symbol, AST* s0, AST* s1, AST* s2, AST* s3)
{
	AST* newNode;
	newNode = (AST*) calloc(1, sizeof(AST));
	newNode->type = type;
	newNode->symbol = symbol;
	newNode->son[0] = s0;
	newNode->son[1] = s1;
	newNode->son[2] = s2;
	newNode->son[3] = s3;
	return newNode;
}

void astPrint(AST* node, int level)
{
	if(!node) return;
	for(int i = 0; i < level; i++) fprintf(stderr, "  ");
	fprintf(stderr, "AST(");

	if(node->type <= AST_PROGRAM) fprintf(stderr, "%s", nodeType[node->type]);
 	else fprintf(stderr, "AST_UNKNOWN");

	if(node->symbol) fprintf(stderr, ",%s\n", node->symbol->text);
	else fprintf(stderr, ",0\n");

	for(int i = 0; i < MAX_SONS; i++) astPrint(node->son[i], level+1);
}

void astDecompile(AST *node)
{
	if (!node) return;
	
	switch (node->type)
	{
		case AST_SYMBOL:
			fprintf(out,"%s",node->symbol->text);
			break;
		case AST_ADD:
			astDecompile(node->son[0]);
			fprintf(out, " + ");
			astDecompile(node->son[1]);
			break;
		case AST_SUB:
			astDecompile(node->son[0]);
			fprintf(out, " - ");
			astDecompile(node->son[1]);
			break;
		case AST_DIV:
			astDecompile(node->son[0]);
			fprintf(out, " / ");
			astDecompile(node->son[1]);
			break;
		case AST_MUL:
			astDecompile(node->son[0]);
			fprintf(out, " . ");
			astDecompile(node->son[1]);
			break;
		case AST_LSR:
			astDecompile(node->son[0]);
			fprintf(out, " < ");
			astDecompile(node->son[1]);
			break;
		case AST_GTR:
			astDecompile(node->son[0]);
			fprintf(out, " > ");
			astDecompile(node->son[1]);
			break;
		case AST_AND:
			astDecompile(node->son[0]);
			fprintf(out, " & ");
			astDecompile(node->son[1]);
			break;
		case AST_OR:
			astDecompile(node->son[0]);
			fprintf(out, " | ");
			astDecompile(node->son[1]);
			break;
		case AST_NOT:
			fprintf(out, "~");
			astDecompile(node->son[0]);
			break;
		case AST_LSE:
			astDecompile(node->son[0]);
			fprintf(out, " <= ");
			astDecompile(node->son[1]);
			break;
		case AST_GTE:
			astDecompile(node->son[0]);
			fprintf(out, " >= ");
			astDecompile(node->son[1]);
			break;
		case AST_EQU:
			astDecompile(node->son[0]);
			fprintf(out, " == ");
			astDecompile(node->son[1]);
			break;
		case AST_DIF:
			astDecompile(node->son[0]);
			fprintf(out, " != ");
			astDecompile(node->son[1]);
			break;
		case AST_ASSIGN:
			astDecompile(node->son[0]);
			fprintf(out, " <- ");
			astDecompile(node->son[1]);
			break;
		case AST_DECVAR:
			astDecompile(node->son[0]);
			fprintf(out,"%s(",node->symbol->text);
			astDecompile(node->son[1]);
			fprintf(out,")");
			fprintf(out,";\n");
			break;
		case AST_DECVEC:
			astDecompile(node->son[0]);
			fprintf(out,"%s",node->symbol->text);
			if(node->son[2] && node->son[2]->type == AST_INTV)
			{
				fprintf(out,"[%s] ",node->son[1]->symbol->text);
				astDecompile(node->son[2]);
				break;
			}
			fprintf(out,"[%s]",node->son[1]->symbol->text);
			fprintf(out,";\n");
			break;
		case AST_DECFUN:
			astDecompile(node->son[0]);
			fprintf(out,"%s",node->symbol->text);
			if (node->son[1])
			{
				fprintf(out,"(");
				astDecompile(node->son[1]);
				fprintf(out,")");
			}
			else
				fprintf(out,"()");
				
			if(node->son[2])
				astDecompile(node->son[2]);
			break;
		case AST_LCMD:
			astDecompile(node->son[0]);
			astDecompile(node->son[1]);
			break;
		case AST_LCMDT:
			fprintf(out,";\n");
			astDecompile(node->son[0]);
			astDecompile(node->son[1]);
			break;
		case AST_EXPN:
			fprintf(out,"(");
			astDecompile(node->son[0]);
			fprintf(out,")");
			break;
		case AST_VAR:
			fprintf(out,"%s",node->symbol->text);
			break;
		case AST_VECTOR:
			fprintf(out,"%s[",node->symbol->text);
			astDecompile(node->son[0]);
			fprintf(out,"]");
			break;
		case AST_IF:
			fprintf(out,"if(");
			astDecompile(node->son[0]);
			fprintf(out,") ");
			astDecompile(node->son[1]);
			break;
		case AST_IF_ELSE:
			fprintf(out,"if(");
			astDecompile(node->son[0]);
			fprintf(out,") ");
			astDecompile(node->son[1]);
			fprintf(out,"else ");
			astDecompile(node->son[2]);
			break;
		case AST_WHILE:
			fprintf(out,"while(");
			astDecompile(node->son[0]);
			fprintf(out,") ");
			astDecompile(node->son[1]);
			break;
		case AST_READ:
			fprintf(out,"read ");
			astDecompile(node->son[0]);
			break;
		case AST_PRINT:
			fprintf(out,"print ");
			astDecompile(node->son[0]);
			break;
		case AST_RETURN:
			fprintf(out,"return ");
			astDecompile(node->son[0]);
			break;
		case AST_CALL:
			fprintf(out,"%s(",node->symbol->text);
			astDecompile(node->son[0]);
			fprintf(out,")");
			break;
		case AST_CHAR:
			fprintf(out, "char ");
			break;
		case AST_FLOAT:
			fprintf(out, "float ");
			break;
		case AST_INT:
			fprintf(out, "int ");
			break;
		case AST_INTV:
			astDecompile(node->son[0]);
			if(node->son[1]) { fprintf(out, " "); astDecompile(node->son[1]); }
			else fprintf(out, ";\n");
			break;
		case AST_EXPL:
			astDecompile(node->son[0]);
			if(node->son[1])
			{
				fprintf(out," ");
				astDecompile(node->son[1]);
			}
			break;
		case AST_MSGL:
			astDecompile(node->son[0]);
			if(node->son[1])
			{
				fprintf(out," ");
				astDecompile(node->son[1]);
			}
			break;
		case AST_ARGL:
			astDecompile(node->son[0]);
			fprintf(out,"%s",node->symbol->text);
			if(node->son[1])
			{
				fprintf(out," ");
				astDecompile(node->son[1]);
			}
			break;
		case AST_DECL:
			astDecompile(node->son[0]);
			if(node->son[1])
				astDecompile(node->son[1]);
			break;
		case AST_BLCK:
			fprintf(out,"{\n");
			astDecompile(node->son[0]);
			fprintf(out,"}\n");
			break;
	}  
}