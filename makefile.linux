# Makefile

# Nome do projeto
PROJ_NAME=Terreno

# Arquivos .c
CPP_SOURSE=$(wildcard ./src/*.cpp)

# Arquivos  .h
H_SOURSE=$(wildcard ./include/*.h)

# Compilador
CC=g++

# Flags pro compilador
CC_FLAGS=-lGL      \
         -lGLU     \
         -lGLEW    \
         -lglfw3   \
         -lX11     \
         -lXxf86vm \
         -lXrandr  \
         -lpthread \
         -lXi      \
         -ldl      \
         -lXinerama\
         -lXcursor

# Flags
M_FLags=-std=c++11\
        -c

# Comando para limpar o target
RM=rm -rf

#
#Compilaçãp e links
#  

all: clean objFolder $(PROJ_NAME)

$(PROJ_NAME): ./objects/main.o
	@ echo 'Construindo o binario com o linkador do g++'
	$(CC) $< -o $(PROJ_NAME) $(CC_FLAGS)
	@ echo 'Binario construindo'
	@ echo ' '

./objects/main.o: $(CPP_SOURSE)
	@ echo 'Compilando main.cpp sem linkar'
	$(CC) $(M_FLags) $< $(CC_FLAGS) -o $@
	@ echo 'Arquivo main.o gerado'
	@ echo ' '

objFolder:
	@ echo 'Criando a Pasta objects para arquivos .o'
	mkdir -p objects
	@ echo 'Pasta objects criada'
	@ echo ' '

clean:
	@ echo 'Limpando os arquivos .o da pasta objects'
	$(RM) ./objects/*.o $(PROJ_NAME)
	@ echo 'Arquivos .o da pasta objects limpos'
	@ echo ' '
