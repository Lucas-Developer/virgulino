/*
 * I would like to thank to Gjuniioor for letting 
 * me know about the danger that dangling points
 * can bring to the project.
 *
 * Thank you boy!.
 *
 * P.S.: You see, no dangling points anymore.
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "plugin.h"

#define    LEN     32

struct plug_state_ {
    char str[LEN];
};

static inline char *
get (plug_state * state) {
    return state->str;
}

static inline void
set(plug_state * state, const char * str) {
    strncpy (state->str, str, (sizeof(state->str)));
}

static plug_state * 
plug_init() {
    plug_state * state = malloc (sizeof(*state));
    memset (state->str, 0x00, sizeof(state->str));

    return state;
}

static void
plug_reload (plug_state * state) {
    memset (state->str, 0x00, sizeof(state->str));
}


static bool
plug_print (plug_state * state) {
    printf ("Plugin being printed !\n");
    return true;
}

static void
plug_unload(plug_state * state) {
    
}

static void
plug_finalize (plug_state * state) {
    if (state) { free(state); }

    state = NULL;
}

const plug_api PLUG_API = {
    .init = plug_init,
    .reload = plug_reload,
    .print = plug_print,
    .unload = plug_unload,
    .finalize = plug_finalize,
};
