INCLUDE = -I ./pccts
FLAGS= -Wno-write-strings

all: antlr dlg interpreter

antlr: interpreter.g
		antlr -gt interpreter.g

dlg: parser.dlg
		dlg -ci parser.dlg scan.c

interpreter: interpreter.c scan.c err.c
		g++ $(FLAGS) -o interpreter interpreter.c scan.c err.c $(INCLUDE)



clean:
	rm -f *.o interpreter.c scan.c err.c parser.dlg tokens.h mode.h
