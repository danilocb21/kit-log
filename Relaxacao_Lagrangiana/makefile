#detecta se o sistema é de 32 ou 64 bits
BITS_OPTION = -m64

#### define o compilador
CPPC = g++
#############################

#### Flag condicional para Debug (Padrão é 0/Release)
DEBUG ?= 0

ifeq ($(DEBUG), 1)
    # Modo Debug: Sem otimização (-O0), máximo de símbolos de debug (-g3), mantém os asserts
    CCOPT = $(BITS_OPTION) -Iinclude -Wall -O0 -g3 -fPIC -fexceptions -DIL_STD -std=c++17 -MMD -MP
    CCLNFLAGS = $(BITS_OPTION) -lpthread -lm
else
    # Modo Release: Otimização máxima (-O3), LTO, ignora asserts (-DNDEBUG)
    CCOPT = $(BITS_OPTION) -Iinclude -Wall -O3 -g -march=native -flto -fPIC -fexceptions -DNDEBUG -DIL_STD -std=c++17 -MMD -MP
    CCLNFLAGS = $(BITS_OPTION) -O3 -flto -march=native -lpthread -lm
endif

CONCERTINCDIR = $(CONCERTDIR)/include
CCFLAGS = $(CCOPT)
#############################

#### diretorios com os source files e com os objs files
SRCDIR = src
OBJDIR = obj
#############################

#### lista de todos os srcs e todos os objs
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
#############################

#### regra principal, gera o executavel
tsp: $(OBJS) 
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CPPC) $(OBJS) -o $@ $(CCLNFLAGS)
############################

#inclui os arquivos de dependencias
-include $(OBJS:.o=.d)

#regra para cada arquivo objeto: compila e gera o arquivo de dependencias do arquivo objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p obj
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CPPC) $(CCFLAGS) -c $< -o $@

#delete objetos e arquivos de dependencia
clean:
	@echo "\033[31mcleaning obj directory \033[0m"
	@rm tsp -f $(OBJDIR)/*.o $(OBJDIR)/*.d

rebuild: clean tsp