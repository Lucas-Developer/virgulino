#ifndef _PLUGIN_H_
#define _PLUGIN_H_ 

#include <stdbool.h>

typedef struct plug_state_ plug_state;

typedef struct plug_api_ plug_api;
struct plug_api_ {
    plug_state * (* init)();
    void (* finalize) (plug_state * state);
    void (* reload) (plug_state * state);
    void (* unload) (plug_state * state);
    bool (* print) (plug_state * state);
};


extern const plug_api PLUG_API;

#endif /* ifndef _PLUGIN_H_ */
