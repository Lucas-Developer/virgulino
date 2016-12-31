/*
 * caesar.c
 *
 * Copyright (C) 2016 - userx
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* @note: Compiling instructions
 * gcc -pedantic -Wall -O2 -fPIC -shared caesar.* -o caesar.so -ldl
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils.h"
#include "../crypt_plugin_api.h"

#define HIGHER_CHAR                 126     // see ascii table (ENG) - Veja a tabela ascii (PT-BR)
#define LOWER_CHAR                  32      //  "    "     "

struct plugin_state_ {	/* empty */ };

static plugin_state *
vigenere_init (const char * config_filepath) {
	return (plugin_state *) wr_calloc (1, sizeof (plugin_state));
}

static void
vigenere_reload (plugin_state * state) {
	free (state);
	state = vigenere_init (NULL);
}

static char *
vigenere_encrypt (const plugin_state * state, char * pure_msg, void * key) {
	char * vkey = (char *) key;
    
     int cn = 0;
    
    for (int i = 0; i <= strlen (pure_msg); i++) {

        if (i > 0) 
            pure_msg [(i - 1)] = cn;
        
        for (cn = ((int)pure_msg[i]) + ((int)(vkey[i])) ; cn > HIGHER_CHAR; cn = LOWER_CHAR + (cn % HIGHER_CHAR)); 
    }
   
	return pure_msg;
}	

static char *
vigenere_decrypt (const plugin_state * state , char * encrypted_msg, void * key) {
	char * vkey = (char *) key;

    int cn = 0;

    for (int i = 0; i <= strlen (encrypted_msg); i++) {
        if (i > 0) 
            encrypted_msg [(i - 1)] = cn;

        for (cn = ((int) encrypted_msg[i] - ((int)vkey[i])); cn < LOWER_CHAR; cn = HIGHER_CHAR - (LOWER_CHAR - cn));
   }

	return encrypted_msg;
}

static void 
vigenere_unload (plugin_state * state) { /* empty */ }

static void
vigenere_finalize (plugin_state * state) {
	if (state) { free (state); }
	state = NULL;
}

const crypt_plugin_api CRYPT_PLUGIN_API = {
	.init = vigenere_init,
	.reload = vigenere_reload,
	.unload = vigenere_unload,
	.finalize = vigenere_finalize,

	.encrypt = vigenere_encrypt,
	.decrypt = vigenere_decrypt,
};
