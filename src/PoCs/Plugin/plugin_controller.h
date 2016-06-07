/*
 * @note: I would like to thank to Gjuniioor for letting 
 * me know about the danger that dangling points
 * can bring to the project.
 *
 * Thank you boy!.
 *
 * P.S.: You see, no dangling points anymore.
 */ 

/* @note: Compiling instructions 
 * gcc -pedantic -Wall -O2 -fPIC -shared plugin.* -o plugin.so -ldl
 * gcc -pedantic -Wall -O2 -fPIC main.c -o main -ldl 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dlfcn.h>

#include "plugin_api.h"

typedef struct plugin_  plugin_t;
struct plugin_ {
    void * handle;
    ino_t id;
    plugin_api api;
    plugin_state * state;
};

static void
plugin_load (plugin_t * plugin, const char * plugin_path, void * key, const char * config_filepath) {

    // to file stat ::
    struct stat attr;
    if ( (stat (plugin_path, &attr) == 0) && (plugin->id != attr.st_ino) ) {
        if (plugin->handle) {
            plugin->api.unload (plugin->state);
            dlclose (plugin->handle);
        }

        void * handle = dlopen (plugin_path, RTLD_NOW);
        if (!handle) {
            fprintf (stderr, "%s\n", dlerror ());
            plugin->handle = NULL;
            plugin->id = 0;
            exit (1);
        }

        plugin->handle = handle;
        plugin->id = attr.st_ino;
        const plugin_api * api = dlsym (plugin->handle, "PLUGIN_API");
        if (!api) {
            fprintf (stderr, "%s\n", dlerror());
            dlclose (plugin->handle);
            plugin->handle = NULL;
            plugin->id = 0;
            exit (1);
        }

        plugin->api = *api;
        
        if (plugin->state == NULL) { plugin->state = plugin->api.init (key, config_filepath); }
        plugin->api.reload (plugin->state);
    }
}

void 
plugin_unload (plugin_t * plugin) {
    if (!plugin->handle) { return; }

    plugin->api.finalize(plugin->state);
    plugin->state = NULL;
    dlclose(plugin->handle);
    plugin->handle = NULL; 
    plugin->id = 0;
    free(plugin->handle);

}

