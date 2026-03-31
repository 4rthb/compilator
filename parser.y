%{
	/*
	* 
	* UFRGS - Compiladores B - Marcelo Johann - 2022/1
	* 
	* Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
	* Tiago Lucas Flach <tlflach@inf.ufrgs.br>
	* 
	*/
	#include "hash.h"
	#include "ast.h"
	#include "semantic.h"

	int yylex(void); 
	int yyerror(const char* s);
	int getLineNumber();

	AST *ASTroot = NULL;
%}

%union
{
	HASH_NODE *symbol;
	AST *ast;
}

/* Tokens */
%token<symbol> KW_CHAR
%token<symbol> KW_INT
%token<symbol> KW_FLOAT

%token KW_IF
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token ASSIGNMENT
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TOKEN_ERROR


/* Types */
%type<ast> dec
%type<ast> blck
%type<ast> argl

%type<ast> decl
%type<ast> exprl
%type<ast> msgl
%type<ast> lcmd
%type<ast> lcmdt

%type<ast> expr
%type<ast> lit
%type<ast> vsize
%type<ast> kw_t
%type<ast> var
%type<ast> initv

%type<ast> msg
%type<ast> cmd


/* Associations */
%left '|' '&'
%left '+' '-'
%left '<' '>'
%left '~'
%left '.' '/'

%left OPERATOR_LE OPERATOR_GE
%left OPERATOR_EQ OPERATOR_DIF

%nonassoc IFX
%nonassoc KW_ELSE

%%

program: decl					{ ASTroot = $1; }
		;

decl:   dec decl				{ $$ = astCreate(AST_DECL,0,$1,$2,0,0); }
		|						{ $$ = 0; }
		;

dec:	kw_t TK_IDENTIFIER '(' lit ')' ';'			{ $$ = astCreate(AST_DECVAR,$2,$1,$4,0,0); }
		| kw_t TK_IDENTIFIER '(' argl ')' blck		{ $$ = astCreate(AST_DECFUN,$2,$1,$4,$6,0); }
		| kw_t TK_IDENTIFIER '[' vsize ']' initv	{ $$ = astCreate(AST_DECVEC,$2,$1,$4,$6,0); }
		;

vsize:  LIT_INTEGER				{ $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
		;

/* Type */
kw_t:   KW_CHAR   				{ $$ = astCreate(AST_CHAR,0,0,0,0,0);  }  
		| KW_FLOAT				{ $$ = astCreate(AST_FLOAT,0,0,0,0,0); }
		| KW_INT				{ $$ = astCreate(AST_INT,0,0,0,0,0);   }
		;

/* Literal */
lit:	LIT_CHAR				{ $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
		| LIT_FLOAT				{ $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
		| LIT_INTEGER			{ $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
		;

/* Init vector */
initv:  lit initv				{ $$ = astCreate(AST_INTV,0,$1,$2,0,0); }
		| ';'					{ $$ = 0; }
		;

/* Argument list */
argl:   kw_t TK_IDENTIFIER argl	{ $$ = astCreate(AST_ARGL,$2,$1,$3,0,0); }
		|						{ $$ = 0; }
		;

cmd:	blck									{ $$ = $1; }
		| var ASSIGNMENT expr					{ $$ = astCreate(AST_ASSIGN,0,$1,$3,0,0);   }
		| KW_IF '(' expr ')' cmd %prec IFX		{ $$ = astCreate(AST_IF,0,$3,$5,0,0);	   }
		| KW_IF '(' expr ')' cmd KW_ELSE cmd	{ $$ = astCreate(AST_IF_ELSE,0,$3,$5,$7,0); }
		| KW_WHILE '(' expr ')' cmd				{ $$ = astCreate(AST_WHILE,0,$3,$5,0,0);	}
		| KW_READ var							{ $$ = astCreate(AST_READ,0,$2,0,0,0);	  }
		| KW_PRINT msgl							{ $$ = astCreate(AST_PRINT,0,$2,0,0,0);	 }
		| KW_RETURN expr						{ $$ = astCreate(AST_RETURN,0,$2,0,0,0);	}
		|										{ $$ = 0; }
		;

/* Message */
msg:	LIT_STRING				{ $$ = astCreate(AST_SYMBOL,$1,0,0,0,0); }
		| expr					{ $$ = $1; }
		;

/* Message list */
msgl:	msg msgl				{ $$ = astCreate(AST_MSGL,0,$1,$2,0,0); }
		| 						{ $$ = 0; }
		;

blck:   '{' lcmd '}'			{ $$ = astCreate(AST_BLCK,0,$2,0,0,0); }
		;

/* List command */
lcmd:   cmd lcmdt				{ $$ = astCreate(AST_LCMD,0,$1,$2,0,0); }
		;

/* List command tail */
lcmdt: ';' cmd lcmdt 			{ $$ = astCreate(AST_LCMDT,0,$2,$3,0,0); }
		|						{ $$ = 0; }
		;

var:	TK_IDENTIFIER							{ $$ = astCreate(AST_VAR,$1,0,0,0,0);	 }
		| TK_IDENTIFIER '[' expr ']'			{ $$ = astCreate(AST_VECTOR,$1,$3,0,0,0); }
		;

expr:   lit									{ $$ = $1; }
		| var								{ $$ = $1; }
		| expr '+' expr						{ $$ = astCreate(AST_ADD,0,$1,$3,0,0);  }
		| expr '-' expr						{ $$ = astCreate(AST_SUB,0,$1,$3,0,0);  }
		| expr '/' expr						{ $$ = astCreate(AST_DIV,0,$1,$3,0,0);  }
		| expr '.' expr						{ $$ = astCreate(AST_MUL,0,$1,$3,0,0);  }
		| expr '<' expr						{ $$ = astCreate(AST_LSR,0,$1,$3,0,0);  }
		| expr '>' expr						{ $$ = astCreate(AST_GTR,0,$1,$3,0,0);  }
		| expr '&' expr						{ $$ = astCreate(AST_AND,0,$1,$3,0,0);  }
		| expr '|' expr						{ $$ = astCreate(AST_OR ,0,$1,$3,0,0);  }
		| '~' expr							{ $$ = astCreate(AST_NOT,0,$2,0,0,0);   }
		| expr OPERATOR_LE expr				{ $$ = astCreate(AST_LSE,0,$1,$3,0,0);  }
		| expr OPERATOR_GE expr				{ $$ = astCreate(AST_GTE,0,$1,$3,0,0);  }
		| expr OPERATOR_EQ expr				{ $$ = astCreate(AST_EQU,0,$1,$3,0,0);  }
		| expr OPERATOR_DIF expr			{ $$ = astCreate(AST_DIF,0,$1,$3,0,0);  }
		| '(' expr ')'						{ $$ = astCreate(AST_EXPN,0,$2,0,0,0);  }
		| TK_IDENTIFIER '(' exprl ')'		{ $$ = astCreate(AST_CALL,$1,$3,0,0,0); }
		;

exprl:  expr exprl							{ $$ = astCreate(AST_EXPL,0,$1,$2,0,0); }
		|									{ $$ = 0; }
		;

%%

#include <stdio.h>
#include <stdlib.h>

extern char* yytext;

int yyerror(const char* s)
{
	fprintf(stderr, "%s", s);
	fprintf(stderr, " at line %d.\n", getLineNumber());
	fprintf(stderr, "Error here: %s\n", &yytext[0]);
	exit(3);
}