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
PLUGINS_DIR=$(LIB_SOURCE)/plugins
PLUGINS_INSTALL_PLACE=/usr/share/virgulino/
CRYPT_DIR=$(PLUGINS_DIR)/crypt
STEG_DIR=$(PLUGINS_DIR)/steg
CONFIG_FILE=virgulino.conf
CONFIG_DIST=/etc
OBJ_SOURCE=bin
OBJS=	$(OBJ_SOURCE)/cypher.o \
		$(OBJ_SOURCE)/cypher_utils.o \
		$(OBJ_SOURCE)/definitions.o \
		$(OBJ_SOURCE)/hide.o \
		$(OBJ_SOURCE)/output.o \
		$(OBJ_SOURCE)/virgulino.o \

all: default

default:
	make -C $(LIB_SOURCE) all
	make -C $(CRYPT_DIR) plugins
	make -C $(STEG_DIR) plugins

clean:
	make -C $(LIB_SOURCE) clean
	make -C $(CRYPT_DIR) clean
	make -C $(STEG_DIR) clean


install:
	mv $(DEST) $(DESTDIR)
	cp $(DOC_SOURCE)/$(DOC_FILE_1) $(DOC_DEST_1)
	mkdir -p $(LIB_DEST)
	cp $(LIB_SOURCE)/*.h $(LIB_DEST)
	cp $(DOC_SOURCE)/$(DOC_FILE_3) $(DOC_DEST_3)
	echo set plugin_dir $(PLUGINS_INSTALL_PLACE) > $(CONFIG_DIST)/$(CONFIG_FILE)
	mkdir -p $(PLUGINS_INSTALL_PLACE)/crypt
	mkdir -p $(PLUGINS_INSTALL_PLACE)/steg
	cp -rf $(PLUGINS_DIR)/crypt/*.so $(PLUGINS_INSTALL_PLACE)/crypt
	cp -rf $(PLUGINS_DIR)/steg/*.so $(PLUGINS_INSTALL_PLACE)/steg

uninstall:
	rm -f $(DESTDIR)/$(DEST)
	rm -f $(CONFIG_DIST)/$(CONFIG_FILE)
