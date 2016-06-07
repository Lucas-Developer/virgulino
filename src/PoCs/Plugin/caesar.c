/*
 * I would like to thank to Gjuniioor for letting
 * me know about the danger that dangling points
 * can bring to the project.
 *
 * Thank you boy!.
 *
 * P.S.: You see, no dangling points anymore.
 *
 * To compile to plugin:
 * gcc  -pedantic -Wall -O2 -fPIC -shared plugin.c plugin_api.h -o plugin.so -ldl
 */


/*
 * *@Note: This is the actual plugin. Every plugin should follow this same
 * example style and architecture. Every plugin is a plugin_name.c file that
 * must includes "plugin_api.h" and everything will be handled by the
 * plugin_controller.h"
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "plugin_api.h"

#define    LEN     32

struct plugin_state_ {
    int salt;
};

static inline int
get (plugin_state * state) {
    return state->salt;
}

static inline void
set(plugin_state * state, int salt) {
    state->salt = salt;
}

static plugin_state *
plugin_init(void * key, const char * config_filepath) {
    plugin_state * state = malloc (sizeof(*state));
    state->salt = *(int *)key;

    return state;
}

static void
plugin_reload (plugin_state * state) {
//    state->salt = 0;
}

static char *
plugin_encrypt (plugin_state * state, char * pure_msg) {
    printf ("Plugin encrypt method called!\n");

    printf ("%s --> ", pure_msg);
    for (int i = 0; i < strlen (pure_msg); i++) {

        if ((pure_msg[i] > 64 && pure_msg[i] < 91)) {
            pure_msg[i] = pure_msg[i] + state->salt;
            if (pure_msg[i] > 90) { pure_msg[i] = (pure_msg[i] - 90) + 64; }

        } else if ((pure_msg[i] > 96 && pure_msg[i] < 123)) {
            pure_msg[i] = pure_msg[i] + state->salt;
            if (pure_msg[i] > 122) { pure_msg[i] = (pure_msg[i] - 122) + 96; }
        }
    }

    return pure_msg;
}

static char *
plugin_decrypt (plugin_state * state, char * encrypted_msg) {
    printf ("Plugin decrypt amethod called!\n");
    
    printf ("%s --> ", encrypted_msg);
    for (size_t i = 0; i < strlen(encrypted_msg); i++) {
        if ((encrypted_msg[i] > 64 && encrypted_msg[i] < 91)) {
            encrypted_msg[i] = encrypted_msg[i] - state->salt;
            
            if (encrypted_msg[i] < 65) { encrypted_msg[i] = (91 - (64 - encrypted_msg[i])); }
            
        } else if ((encrypted_msg[i] > 96 && encrypted_msg[i] < 123)) {
            encrypted_msg[i] = encrypted_msg[i] - state->salt;

            if (encrypted_msg[i] < 97) { encrypted_msg[i] = (122 - (96 - encrypted_msg[i])); }
        }
    }

    return encrypted_msg;
}

static void
plugin_unload(plugin_state * state) {

}

static void
plugin_finalize (plugin_state * state) {
    if (state) { free(state); }

    state = NULL;
}

const plugin_api PLUGIN_API = {
    .init = plugin_init,
    .reload = plugin_reload,
    .encrypt = plugin_encrypt,
    .decrypt = plugin_decrypt,
    .unload = plugin_unload,
    .finalize = plugin_finalize,
};
