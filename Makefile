CC=gcc
OPCOES_COMPILADOR=-I. -lglut -lGLU -lGLEW -lGL
TODOS_ARQUIVOS_PONTOH =
TODOS_ARQUIVOS_OBJ = quadrados_coloridos.o

%.o: ../%.c $(TODOS_ARQUIVOS_PONTOH)
	$(CC) -o $@ -c $< $(OPCOES_COMPILADOR)

all: $(TODOS_ARQUIVOS_OBJ)
	gcc -o quadrados_coloridos $^ $(OPCOES_COMPILADOR)

run: all
	./quadrados_coloridos

clean:
	rm *.o quadrados_coloridos
