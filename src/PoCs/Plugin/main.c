/* @note: Compiling instructions 
 * gcc -pedantic -Wall -O2 -fPIC -shared plugin.c plugin_api.h -o plugin.so -ldl
 * gcc -pedantic -Wall -O2 -fPIC main.c -o main -ldl 
 */

#include <stdio.h>

#include "plugin_controller.h"

int 
main (int argc, char ** argv) {
    if (argc < 2) { 
        fprintf (stderr, "Usage: %s \"./plugin.so\"\n", argv[0]);
        exit(1);
    }

    plugin_t plugin = { 0 };

    plugin_load (&plugin, argv[1]);

    if (plugin.handle) { plugin.api.print(plugin.state); }
    
    plugin_unload(&plugin);

    return 0;
}
