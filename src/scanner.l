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
#include "parser.tab.h"

int running;
int lineNumber;

int isRunning() 	{ return running; 	 }
int getLineNumber() { return lineNumber; }

%}

%x COMMENT
DIGIT	[0-9]

%%

char					{ return KW_CHAR;  }
int						{ return KW_INT;   }
float					{ return KW_FLOAT; }

if 						{ return KW_IF;	 }
else					{ return KW_ELSE;   }
while					{ return KW_WHILE;  }
read					{ return KW_READ;   }
print					{ return KW_PRINT;  }
return					{ return KW_RETURN; }

"<-"					{ return ASSIGNMENT;   }
"<="					{ return OPERATOR_LE;  }
">="					{ return OPERATOR_GE;  }
"=="					{ return OPERATOR_EQ;  }
"!="					{ return OPERATOR_DIF; }

{DIGIT}+				{ yylval.symbol = hashInsert(yytext, HASH_LIT_I); return LIT_INTEGER; }
{DIGIT}","{DIGIT}		{ yylval.symbol = hashInsert(yytext, HASH_LIT_F); return LIT_FLOAT;   }
\'.\'					{ yylval.symbol = hashInsert(yytext, HASH_LIT_C); return LIT_CHAR;	}
\"(\\.|[^\\"])*\"		{ yylval.symbol = hashInsert(yytext, HASH_LIT_S); return LIT_STRING;  }

[a-zA-Z_]*				{ yylval.symbol = hashInsert(yytext, HASH_IDT); return TK_IDENTIFIER; }

[;()\[\]{}+-./<>&|~]	{ return yytext[0]; }

"##".*					{ 				  }
"\\#"					{ BEGIN(COMMENT); }
"\n"					{ ++lineNumber;   }	
[ \t\r]					{ 				  }

.						{ return TOKEN_ERROR; }

<COMMENT>"\n"			{ ++lineNumber;   }
<COMMENT>"#\\"			{ BEGIN(INITIAL); }
<COMMENT>.				{ 				  }

%%

void initMe()
{
	lineNumber = 1; running = 1;
	hashInit();
}

int yywrap()
{
	running = 0;
	return 1;
}