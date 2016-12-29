/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2016 userx <user_x@riseup.net>
 *
 * virgulino is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * virgulino is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* @note: Compiling instructions
 * gcc -pedantic -Wall -O2 -fPIC -shared plugin.* -o plugin.so -ldl
 * gcc -pedantic -Wall -O2 -fPIC main.c -o main -ldl
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "virgulino.h"
//#include "utils.h"
//#include "plugins/plugin_controller.h"

int
main (int argc, char ** argv) {
	opt_handler(argc, argv);
	
	return 0;
}

/*
int
main (int argc, char ** argv) {
    char msg[32] = "Victor";
    if (argc < 2) {
      printf ("Usage: %s \"plugin_name\"\n", argv[0]);
      exit (EXIT_SUCCESS);
    }

	char plugin_path [256];
	snprintf (plugin_path, sizeof (plugin_path), "./plugins/crypt/%s.so", argv[1]);
	crypt_plugin_t * plugin;

    plugin = (crypt_plugin_t *) plugin_load (CRYPTO_PLUGIN, plugin_path, NULL);
    if (PARENT(plugin)->handle) {
      	char key[2] = "1";
      	printf ("encrypted: %s\n", plugin->api.encrypt (PARENT(plugin)->state, msg, (void *)key));
     	printf ("ORIGINAL: %s\n", plugin->api.decrypt (PARENT(plugin)->state, msg, (void *)key));
    }
  	
    plugin_unload (plugin, CRYPTO_PLUGIN);

	bzero (plugin_path, sizeof (plugin_path));
	snprintf (plugin_path, sizeof (plugin_path), "./plugins/steg/%s.so", argv[2]);

	steg_plugin_t * plg;
	plg = (steg_plugin_t *) plugin_load (STEG_PLUGIN, plugin_path, NULL);

 	if (PARENT(plg)->handle) {
		char * content;
		plg->api.hide (PARENT(plg)->state, msg, "hide_testing");
		content = plg->api.unhide (PARENT(plg)->state, "hide_testing");
		printf ("RECOVERED: %s\n", content);
		free (content);
	}
	plugin_unload (plg, STEG_PLUGIN);

	return (0);
}*/

