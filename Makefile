#
#
# UFRGS - Compiladores B - Marcelo Johann - 2022/1
# 
# Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
# Tiago Lucas Flach <tlflach@inf.ufrgs.br>
# 
#
TEST=0
MAIN=main
OUT=compiler
HASH=hash
AST=ast
LEX=lex.yy
YAC=parser.tab
SEM=semantic
TAC=tacs
IN=input.txt
OU=output.txt
ASM=output

FLAGS=-d
ifeq ($(TEST),1)
FLAGS+= -Wcounterexamples
endif

default: 	clean $(OUT) run

$(OUT):		$(YAC) $(LEX) $(MAIN) $(HASH) $(AST) $(SEM) $(TAC)
			gcc -o $(OUT) $(YAC).o $(LEX).o $(MAIN).o $(HASH).o $(AST).o $(SEM).o $(TAC).o
%:			%.c
			gcc -c $@.c
$(LEX).c: 	scanner.l
			flex --header-file=$(LEX).h scanner.l
$(YAC).c:	parser.y
			bison parser.y $(FLAGS)			
run:		$(OUT)
			./$(OUT) $(IN) $(OU)
gasm:		
			gcc -S -o $(ASM).s $(ASM).c 
gexe:			
			gcc $(ASM).s -o $(ASM).exe
grun:			
			./$(ASM).exe
clean:
			find . \( -name '*.o' -o -name '$(LEX).*' -o -name '$(OUT)' -o -name '$(ASM).*' -o -name '$(YAC).*' -o -name '*.output' -o -name '*.out' \) -type f -delete