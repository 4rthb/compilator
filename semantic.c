/*
 * 
 * UFRGS - Compiladores B - Marcelo Johann - 2022/1
 * 
 * Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
 * Tiago Lucas Flach <tlflach@inf.ufrgs.br>
 * 
 */

#include "semantic.h"
#include <stdlib.h>
#include <string.h>

int SemanticErrors = 0;

int get_semantic_errors() { return SemanticErrors; }

int get_datatype(int ast_const)
{
	switch (ast_const)
	{
		case AST_CHAR: 	return HASH_DATA_C; break;
		case AST_FLOAT: return HASH_DATA_F; break;
		case AST_INT: 	return HASH_DATA_I; break;
	}
}

AST* get_expr_leaf(AST* node){ while (node->son[0] != NULL) node = node->son[0]; return node; }

int is_number(AST *node)
{
	return 	(node->type == AST_ADD || node->type == AST_SUB || 
			(node->type == AST_SYMBOL && (node->symbol->type == HASH_LIT_I || 
			node->symbol->type == HASH_LIT_C) || (node->symbol->type == HASH_VAR && 
			(node->symbol->datatype == HASH_DATA_I || node->symbol->datatype == HASH_DATA_C))) ||
			(node->type == AST_CALL && (node->symbol->datatype == HASH_DATA_I || node->symbol->datatype == HASH_DATA_C)));
}

int is_float(AST *node)
{
	return 	(node->type == AST_SYMBOL && node->symbol->type == HASH_LIT_F) || 
			(node->symbol->type == HASH_VAR && node->symbol->datatype == HASH_DATA_F) ||
			(node->type == AST_CALL && node->symbol->datatype == HASH_DATA_F);
}

int is_bool(AST *node)
{
	return 	(node->type == AST_AND || node->type == AST_OR || node->type == AST_EQU || 
	   		 node->type == AST_DIF || node->type == AST_GTR || node->type == AST_LSR || 
	   		 node->type == AST_NOT || node->type == AST_LSE || node->type == AST_GTE);
}

int check_expr(AST *node, const char *expType)
{
	if(is_bool(node))
	{
		if(node->son[0] && node->son[1])
		{
			if(check_expr(node->son[0], expType) && check_expr(node->son[1], expType)) return 1;
			else if(!is_number(node->son[0]) && !is_float(node->son[0]))
					{
						fprintf(stderr, "Semantic ERROR: Invalid left operand for %s\n", expType);
						++SemanticErrors;
						return 0;
					}
			else if(!is_number(node->son[1]) && !is_float(node->son[1]))
					{
						fprintf(stderr, "Semantic ERROR: Invalid right operand for %s\n", expType);
						++SemanticErrors;
						return 0;
					}
			else if((is_number(node->son[0]) != is_number(node->son[1])) &&
			   		(is_float(node->son[0]) != is_float(node->son[1])))
					{
						fprintf(stderr, "Semantic ERROR: Operand types do not match for %s\n", expType);
						++SemanticErrors;
						return 0;
					}
			else return 1;
		}
	}
	return 0;
}	

void check_and_set_declarations(AST *node)
{
	int i;

	if (node == 0)
		return;

	switch (node->type)
	{
		case AST_DECVAR:
			if (node->symbol)
			{
				if (node->symbol->type != HASH_IDT)
				{
					fprintf(stderr, "Semantic ERROR: variable %s already declared\n", node->symbol->text);
					++SemanticErrors;
				}
				node->symbol->type = HASH_VAR;
				node->symbol->dec = node;
				if(node->son[0])
					node->symbol->datatype = get_datatype(node->son[0]->type);
			}
			break;
		case AST_DECVEC:
			if (node->symbol)
			{
				if (node->symbol->type != HASH_IDT)
				{
					fprintf(stderr, "Semantic ERROR: vector %s already declared\n", node->symbol->text);
					++SemanticErrors;
				}
				node->symbol->type = HASH_VEC;
				node->symbol->dec = node;
				if(node->son[0])
					node->symbol->datatype = get_datatype(node->son[0]->type);
			}
			break;
		case AST_DECFUN:
			if (node->symbol)
			{
				if (node->symbol->type != HASH_IDT)
				{
					fprintf(stderr, "Semantic ERROR: function %s already declared\n", node->symbol->text);
					++SemanticErrors;
				}
				node->symbol->type = HASH_FUN;
				node->symbol->dec = node;
				if(node->son[0])
					node->symbol->datatype = get_datatype(node->son[0]->type);
			}
			break;
		case AST_ARGL:
			if (node->symbol){
				node->symbol->type = HASH_VAR;
				node->symbol->dec = node;
				if(node->son[0])
					node->symbol->datatype = get_datatype(node->son[0]->type);
			}
			break;
	}

	for (i=0; i<MAX_SONS; ++i)
		check_and_set_declarations(node->son[i]);
}

void check_undeclared() { SemanticErrors += hashCheckUndeclared(); }

void check_operands(AST *node)
{
	int i;
	char stringType[5];

	if (node == 0)
		return;

	switch (node->type)
	{
		case AST_ADD: strcpy(stringType,"ADD"); break;
		case AST_AND: strcpy(stringType,"AND"); break;
		case AST_NOT: strcpy(stringType,"NOT"); break;
		case AST_OR:  strcpy(stringType,"OR");  break;
		case AST_SUB: strcpy(stringType,"SUB"); break;
		case AST_DIV: strcpy(stringType,"DIV"); break;
		case AST_MUL: strcpy(stringType,"MUL"); break;
		case AST_LSR: strcpy(stringType,"LSR"); break;
		case AST_GTR: strcpy(stringType,"GTR"); break;
		case AST_LSE: strcpy(stringType,"LSE"); break;
		case AST_GTE: strcpy(stringType,"GTE"); break;
		case AST_EQU: strcpy(stringType,"EQU"); break;
		case AST_DIF: strcpy(stringType,"DIF"); break;
	}
	if(!check_expr(node, stringType));
		for (i=0; i<MAX_SONS; ++i)
			check_operands(node->son[i]);
}

void check_vec_index(AST *node)
{
	int i;

	if (node == 0)
		return;
	
	if(node->type == AST_VECTOR && node->symbol->dec != NULL)
	{
		int decsize = atoi(node->symbol->dec->son[1]->symbol->text);
		int indsize, isInteger = 0;
		if(node->son[0]->symbol->type == HASH_LIT_I) { isInteger = 1; indsize = atoi(node->son[0]->symbol->text); }
		if(!is_number(node->son[0]))
		{
			fprintf(stderr, "Semantic ERROR: Invalid index datatype\n");
			++SemanticErrors;
		} 
		else if(isInteger && (indsize >= decsize)){
			fprintf(stderr, "Semantic ERROR: Index out of range\n");
			++SemanticErrors;
		}
	}

	for (i=0; i<MAX_SONS; ++i)
		check_vec_index(node->son[i]);
}

void check_vec(AST *node, int vsize, int current, int dtype)
{
	int i;

	if (node == 0)
		return;

	if(node->type == AST_DECVEC)
	{
		vsize = atoi(node->son[1]->symbol->text);
		dtype = get_datatype(node->son[0]->type);
		current = 0;
	}

	if(node->type == AST_INTV)
	{
		++current;
		if((dtype == HASH_DATA_F && node->son[0]->symbol->type != HASH_LIT_F) ||
		   (current > vsize) || node->son[0]->symbol->type == HASH_LIT_S || is_bool(node))
		{
			fprintf(stderr, "Semantic ERROR: Invalid vector initialization\n");
			++SemanticErrors;
		}
	}

	for (i=0; i<MAX_SONS; ++i)
		check_vec(node->son[i], vsize, current, dtype);
}

void check_return(AST *node, int ret)
{
	int i;

	if (node == 0)
		return;

	if(node->type == AST_DECFUN)
		ret = node->symbol->datatype;
	
	if(node->type == AST_RETURN)
	{	
		if(!node->son[0] || (node->son[0]->symbol && ((node->son[0]->symbol->type == HASH_UNKNOW && (node->son[0]->symbol->datatype != ret)) ||
		  ((ret == HASH_DATA_C || ret == HASH_DATA_I) && !is_number(node->son[0])) || 
		  (ret == HASH_DATA_F && (node->son[0]->symbol->type != HASH_LIT_F && node->son[0]->symbol->datatype != HASH_DATA_F)))) || 
		  is_bool(node->son[0]))
		{
			fprintf(stderr, "Semantic ERROR: Invalid return type\n");
			++SemanticErrors;
		}
	}

	for (i=0; i<MAX_SONS; ++i)
		check_return(node->son[i], ret);
}

void check_function_arguments(AST *node)
{
	int i;

	if (node == 0)
		return;

	if(node->type == AST_CALL && node->symbol->type == HASH_FUN)
	{
		AST* darg = node->symbol->dec->son[1];
		AST* carg = node->son[0];
		while((darg != NULL) && (carg != NULL))
		{
			AST* cleaf = get_expr_leaf(carg);
			if(((darg->symbol->datatype == HASH_DATA_C || darg->symbol->datatype == HASH_DATA_I) && !is_number(cleaf)) || 
		  		(darg->symbol->datatype == HASH_DATA_F && (cleaf->symbol->type != HASH_LIT_F && cleaf->symbol->datatype != HASH_DATA_F)))
			{
				fprintf(stderr, "Semantic ERROR: Incorrect argument datatype\n");
				++SemanticErrors;
			}
			darg = darg->son[1];
			carg = carg->son[1];
		}
		if((darg == NULL) && (carg != NULL))
		{
			fprintf(stderr, "Semantic ERROR: Too many arguments in function call\n");
			++SemanticErrors;
		}
		else if((darg != NULL) && (carg == NULL))
		{
			fprintf(stderr, "Semantic ERROR: Few arguments in function call\n");
			++SemanticErrors;
		}
	}

	for (i=0; i<MAX_SONS; ++i)
		check_function_arguments(node->son[i]);
}

void check_nature(AST *node)
{
	int i;

	if (node == 0)
		return;

	switch (node->type)
	{
		case AST_VECTOR:
			if((!node->son[0] || (node->symbol->type != HASH_VEC)) && node->symbol->type != HASH_IDT) 
			{
				fprintf(stderr, "Semantic ERROR: This object cannot be indexed\n");
				++SemanticErrors;
			}
			break;
		case AST_VAR: 
			if((node->son[0] || (node->symbol->type != HASH_VAR)) && node->symbol->type != HASH_IDT) 
			{
				fprintf(stderr, "Semantic ERROR: This object is not a variable\n");
				++SemanticErrors;
			}
			break;
		case AST_CALL: 
			if((node->symbol->type != HASH_FUN) && node->symbol->type != HASH_IDT) 
			{
				fprintf(stderr, "Semantic ERROR: This object is uncallable\n");
				++SemanticErrors;
			}
			break;
	}
	
	for (i=0; i<MAX_SONS; ++i)
		check_nature(node->son[i]);
}

void check_semantic(AST *node)
{
	check_and_set_declarations(node);
	check_undeclared();
	check_operands(node);
	check_vec_index(node);
	check_vec(node,0,0,0);
	check_nature(node);
	check_return(node, 0);
	check_function_arguments(node);
}