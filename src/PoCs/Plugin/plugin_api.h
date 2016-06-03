#ifndef _PLUGIN_API_H_
#define _PLUGIN_API_H_ 

typedef struct plugin_state_ plugin_state;

typedef struct plugin_api_ plugin_api;
struct plugin_api_ {
    plugin_state * (* init)();
    void (* finalize) (plugin_state * state);
    void (* reload) (plugin_state * state);
    void (* unload) (plugin_state * state);
    void (* print) (plugin_state * state);
};


extern const plugin_api PLUGIN_API;

#endif /* _PLUGIN_API_H_ */
