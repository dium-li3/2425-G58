CC=gcc
CFLAGS=-Wall -O3 -Iinclude/utils -Iinclude/entities -Iinclude/managers -Iinclude//validacao $(shell pkg-config --cflags glib-2.0)
LDFLAGS= $(shell pkg-config --libs glib-2.0)
EXE1=programa_principal
S=src/
INC=include/
SU=$(S)utils/
SE=$(S)entities/
SM=$(S)managers/
SV=$(S)validacao/


$(EXE1): $(S)main.o $(SU)parser.o $(SM)user_manager.o $(SE)users.o $(SE)musics.o $(SE)artists.o $(SV)sintatica.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(S)main.o: $(S)main.c

$(SU)parser.o: $(SU)parser.c

$(SM)user_manager.o: $(SM)user_manager.c

$(SE)users.o : $(SE)users.c

$(SE)musics.o : $(SE)musics.c

$(SE)artists.o : $(SE)artists.c

$(SV)sintatica.o : $(SV)sintatica.c
 

clean:
	rm $(S)*.o $(EXE1)