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

struct plugin_state_ {	/* empty */ };

static plugin_state *
caesar_init (const char * config_filepath) {
	return (plugin_state *) wr_calloc (1, sizeof (plugin_state));
}

static void
caesar_reload (plugin_state * state) {
	free (state);
	state = caesar_init (NULL);
}

static char *
caesar_encrypt (const plugin_state * state, char * pure_msg, void * key) {
	int salt = atoi ((char *)key);
    
    size_t size = strlen (pure_msg);
	for (int i = 0; i < size; i++) {
		pure_msg [i] = pure_msg[i] + salt;
	}
	
	return pure_msg;
}	

static char *
caesar_decrypt (const plugin_state * state , char * encrypted_msg, void * key) {
	int salt = atoi ( (char *)key);

    size_t size = strlen (encrypted_msg);
	for (int i = 0; i < size; i++) {
		encrypted_msg [i] = encrypted_msg [i] - salt;
	}

	return encrypted_msg;
}

static void 
caesar_unload (plugin_state * state) { /* empty */ }

static void
caesar_finalize (plugin_state * state) {
	if (state) { free (state); }
	state = NULL;
}

const crypt_plugin_api CRYPT_PLUGIN_API = {
	.init = caesar_init,
	.reload = caesar_reload,
	.unload = caesar_unload,
	.finalize = caesar_finalize,

	.encrypt = caesar_encrypt,
	.decrypt = caesar_decrypt,

};
