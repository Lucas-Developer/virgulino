/*
 * crypto_plugin_controller.h
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dlfcn.h>

#include "plugin.h"
#include "crypt_plugin_api.h"
#include "steg_plugin_api.h"

#include "utils.h"

#define PARENT(obj) 	((plugin_t *)obj)
#define PLUGIN(obj) 	((plugin_t)&obj)
#define CRYPT(obj) 		((crypt_plugin_t *)obj)
#define STEG(obj) 		((steg_plugin_t *)obj)

/*
 * **@Note:
 * 	In order to provide an agnostic plugin controller, it was need to build 
 * a kind of inheritence so that the same plugin controller's methods can be called
 * to every kind of plugin, the only thing the may change is the parameters.
 */
typedef enum {
	CRYPTO_PLUGIN,
	STEG_PLUGIN,
} PluginType;

typedef struct crypt_plugin_ crypt_plugin_t;
struct crypt_plugin_ {
	plugin_t parent;
	crypt_plugin_api api;
};

typedef struct steg_plugin_ steg_plugin_t;
struct steg_plugin_ {
 	plugin_t parent;
	steg_plugin_api api;
};

static void *
plugin_load (PluginType type , const char * plugin_path, const char * config_filepath) {
	struct stat attr;
	void * plugin = NULL;

	if ( (stat (plugin_path, &attr) ) != 0 ) {
		fprintf (stderr, "Error opening the file file: ");
 		perror ("");
		exit (EXIT_FAILURE);
	}

 	//plugin_t * parent = (plugin_t *) wr_calloc (1, sizeof (plugin_t) );
 	plugin_t parent;
	parent.handle = NULL;

	parent.handle = dlopen (plugin_path, RTLD_NOW);
	if (!parent.handle) {
		fprintf (stderr, "%s\n", dlerror() );
		exit (EXIT_FAILURE);
	}

	if (type == CRYPTO_PLUGIN) {
		plugin = (crypt_plugin_t *) wr_calloc (1, sizeof (crypt_plugin_t) );
		crypt_plugin_api * api = dlsym (parent.handle, "CRYPT_PLUGIN_API");
		if (!api) {
			fprintf (stderr, "%s\n", dlerror () );
			dlclose (parent.handle);
			exit (EXIT_FAILURE);
		}

	
		CRYPT(plugin)->api = *api;
		CRYPT(plugin)->parent.handle = parent.handle;
		CRYPT(plugin)->parent.state = CRYPT(plugin)->api.init(config_filepath); 
		

	} else {

		plugin = (steg_plugin_t *) wr_calloc (1, sizeof (steg_plugin_t) );
		steg_plugin_api * api = dlsym (parent.handle, "STEG_PLUGIN_API");

		if (!api) {
			fprintf (stderr, "%s\n", dlerror () );
			dlclose (parent.handle);
			exit (EXIT_FAILURE);
		}

		
		STEG(plugin)->api = *api;
		STEG(plugin)->parent.handle = parent.handle;
		STEG(plugin)->parent.state = STEG(plugin)->api.init(config_filepath);	

	}

	return plugin;
}

static void
plugin_unload (void * plugin, PluginType type) {
	plugin_t * parent = &(CRYPT(plugin)->parent);
	
	if (!parent->handle) { return; }

	if (type == CRYPTO_PLUGIN) { CRYPT(plugin)->api.finalize (parent->state); }
	else { STEG(plugin)->api.finalize (parent->state); }

	dlclose (parent->handle);
	parent->handle = NULL;
	parent = NULL;

	if (type == CRYPTO_PLUGIN) { free (CRYPT(plugin)); } else { free (STEG(plugin)); }
	plugin = NULL;

}
