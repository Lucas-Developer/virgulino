SRC=src/main.c
DEST=virgulino
DESTDIR=/usr/bin/
CC=gcc
SWITCH=-o
CVERSION=-std=c11
DOC_SOURCE=doc
DOC_FILE_1=virgulino.1
DOC_DEST_1=/usr/share/man/man1
DOC_FILE_3=virgulino.3
DOC_DEST_3=/usr/share/man/man3
LIB_DEST=/usr/include/virgulino
LIB_SOURCE=src
OBJ_SOURCE=bin
OBJS=	$(OBJ_SOURCE)/cypher.o \
		$(OBJ_SOURCE)/cypher_utils.o \
		$(OBJ_SOURCE)/definitions.o \
		$(OBJ_SOURCE)/hide.o \
		$(OBJ_SOURCE)/output.o \
		$(OBJ_SOURCE)/virgulino.o \

all: default

default: $(OBJS) $(LIB_SOURCE)/main.c
	$(CC) $^ $(CVERSION) $(SWITCH) $(DEST)

$(OBJ_SOURCE)/%.o: $(LIB_SOURCE)/%.c
	$(CC) -c $< -o $@

clean:
	rm -f $(DEST) 

install:
	mv $(DEST) $(DESTDIR)
	cp $(DOC_SOURCE)/$(DOC_FILE_1) $(DOC_DEST_1)
	mkdir -p $(LIB_DEST)
	cp $(LIB_SOURCE)/*.h $(LIB_DEST)
	cp $(DOC_SOURCE)/$(DOC_FILE_3) $(DOC_DEST_3)

uninstall:
	rm -f $(DESTDIR)/$(DEST)
