/*
 * ascii.c
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
 * gcc -pedantic -Wall -O2 -fPIC -shared ascii.c -o ascii.so -ldl
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/stat.h>

#include "../../utils.h"
#include "../steg_plugin_api.h"

/*
 * to see more about plugin_state_, take a look at plugin_api.h
 */
struct plugin_state_ { /* empty */ };

static plugin_state *
init (const char * config_filepath) {
	return (plugin_state *) malloc (sizeof (plugin_state));
}

static void
reload (plugin_state * state) {
	free(state);
	state = init (NULL);
}

// unused
static void
unload (plugin_state * state) { /* ... */ }

static void
finalize (plugin_state * state) {
	if (!state) { free (state); }
	state = NULL;
}

static void
char_to_bin (char * bin, const char c) {
	for (int j = 0, i = 7; i >= 0; i--, j++) {
		bin[j] = ( (c & (1 << i) ) ? '1' : '0');
	}
	bin [8] = 0x00;
}

static char
bin_to_char (const char * bin) {
	char c = 0x00;

	for (int i = 0; i <= 7; i++) {
		if (bin[i] == '1') { c |= 1 << (7 - i); }
	}

	return c;
}

static void
load_file_content (const char * filename, char * content, size_t size) {
    FILE * fp = fopen (filename, "r");
	if (!fp) {
		fprintf (stderr, "ERROR OPENING THE FILE %s : not found\n", filename);
		exit(EXIT_FAILURE);
	}
	size_t i = 0;
	while (i < size) {
		content [i] = fgetc (fp);
		i++;
	}
	fclose(fp);
	fflush(fp);
}

static void
save_to_file (const char * filename , const char * content) {
	FILE * fp = fopen (filename, "w");
	if (!fp) {
		fprintf (stderr, "Error opening the file %s : not found!\n", filename);
		exit (EXIT_FAILURE);
	}
//	fputc ('\b', fp);
	fputs (content, fp);
//	fputc ('\b', fp);
 	fclose (fp);
	fflush(fp);
}

static void
translate (StegStatus status, char * content) {
	size_t size = strlen (content);

	if (status == ASCII) {
		for (int i = 0; i < size; i++) {
		   content[i] = (content[i] == '1' ? 0x09 : 0x20);
		}
	} else {
		for (int i = 0; i < size; i++) {
		   content[i] = (content[i] == 0x09 ? '1' : '0');
		}
	}
}

static void
hide (plugin_state * state, const char * content, const char * dst_filename) {
	int size = strlen (content);
	char * hidden  = (char *)  wr_calloc (((size*8)+1), sizeof (char));
	bzero (hidden, sizeof (hidden));

	char bin[8];

	for (int i = 0; i < size; i++) {
		char_to_bin (bin, content[i]);
		strcat (hidden, bin);
	}

	translate (ASCII, hidden);
	save_to_file (dst_filename, hidden);

	free (hidden);
}

/*
 * **@Note:
 * 		char * content is dynamicaly allocated and must be freed.
 */
static char *
unhide (plugin_state * state, const char * src_filename) {
	struct stat attr;

	if (stat (src_filename, &attr) != 0) {
		fprintf (stderr, "Error opening the file %s : ", src_filename);
		perror ("");
		return NULL;
	}
 	//printf ("FILE: %s\n SIZE: %ld\n", src_filename, attr.st_size);
	char ascii[attr.st_size] ;
		//= (char *) wr_calloc (attr.st_size, sizeof (char));

	load_file_content (src_filename, ascii, attr.st_size);

	translate (HIDDEN, ascii);

 	char * hidden = wr_calloc ( (strlen(ascii)/8), sizeof (char) );

 	char bin[8];
	int j = 0;
	int k = 0;	
	for (int i = 0; i < attr.st_size; i++) {
		bin [j] = ascii [i];
		if (j == 7) {
			hidden [k] = bin_to_char (bin);
			k++;
			j = 0;
			continue;
		}
		j++;
	}
	return hidden;
}

const steg_plugin_api STEG_PLUGIN_API = {
	.init = init,
	.finalize = finalize,
	.reload = reload,
	.unload = unload,
 	
	.hide = hide,
	.unhide = unhide,
};

