#
#
# UFRGS - Compiladores B - Marcelo Johann - 2022/1
# 
# Arthur Prochnow Baumgardt <apbaumgardt@inf.ufrgs.br>
# Tiago Lucas Flach <tlflach@inf.ufrgs.br>
# 
#
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

MAIN = main
OUT = compiler
HASH = hash
AST = ast
LEX = lex.yy
YAC = parser.tab
SEM = semantic
TAC = tacs

IN = test/test_math.txt
OU = output.txt
ASM = output

# List of object files
OBJS = $(OBJ_DIR)/$(YAC).o $(OBJ_DIR)/$(LEX).o $(OBJ_DIR)/$(MAIN).o $(OBJ_DIR)/$(HASH).o $(OBJ_DIR)/$(AST).o $(OBJ_DIR)/$(SEM).o $(OBJ_DIR)/$(TAC).o

# Executable path
TARGET = $(BIN_DIR)/$(OUT)

FLAGS=-d

default: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	gcc -o $(TARGET) $(OBJS)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	gcc -c $< -o $@ -I$(SRC_DIR)

$(OBJ_DIR)/$(LEX).o: $(SRC_DIR)/$(LEX).c | $(OBJ_DIR)
	gcc -c $< -o $@ -I$(SRC_DIR)

$(OBJ_DIR)/$(YAC).o: $(SRC_DIR)/$(YAC).c | $(OBJ_DIR)
	gcc -c $< -o $@ -I$(SRC_DIR)

$(SRC_DIR)/$(LEX).c: $(SRC_DIR)/scanner.l $(SRC_DIR)/$(YAC).h
	flex --outfile=$(SRC_DIR)/$(LEX).c --header-file=$(SRC_DIR)/$(LEX).h $(SRC_DIR)/scanner.l

$(SRC_DIR)/$(YAC).c $(SRC_DIR)/$(YAC).h: $(SRC_DIR)/parser.y
	bison $(FLAGS) --output=$(SRC_DIR)/$(YAC).c $(SRC_DIR)/parser.y

run:		$(TARGET)
			./$(TARGET) $(IN) $(OU)
gasm:		
			gcc -S -o $(ASM).s $(ASM).c 
gexe:			
			gcc $(ASM).s -o $(ASM).exe
grun:			
			./$(ASM).exe
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f $(SRC_DIR)/$(LEX).* $(SRC_DIR)/$(YAC).*
	rm -f $(ASM).* *.output *.out