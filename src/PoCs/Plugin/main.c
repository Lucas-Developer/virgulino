/* @note: Compiling instructions 
 * gcc -pedantic -Wall -O2 -fPIC -shared plugin.c plugin_api.h -o plugin.so -ldl
 * gcc -pedantic -Wall -O2 -fPIC main.c -o main -ldl 
 */

#define _DEFAULT_SOURCE // usleep
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "plugin_controller.h"

int 
main (int argc, char ** argv) {
    char msg[24] = "Eu sou uma pessoa legal!";
    if (argc < 2) { 
        fprintf (stderr, "Usage: %s \"./plugin.so\"\n", argv[0]);
        exit(1);
    }

    plugin_t plugin = { 0 };

    int i = 1;
    plugin_load (&plugin, argv[1], (void *)&i, NULL);
    
    if (plugin.handle) { 
        printf ("%s\n", plugin.api.encrypt (plugin.state, msg)); 
        printf ("%s\n", plugin.api.decrypt (plugin.state, msg));
    } 

    plugin_unload(&plugin);

    return 0;
}
