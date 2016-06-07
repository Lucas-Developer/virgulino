#ifndef _PLUGIN_API_H_
#define _PLUGIN_API_H_ 

typedef struct plugin_state_ plugin_state;

typedef struct plugin_api_ plugin_api;
struct plugin_api_ {
    plugin_state * (* init)(void * key, const char * config_filepath);
    void (* finalize) (plugin_state * state);
    void (* reload) (plugin_state * state);
    void (* unload) (plugin_state * state);

    char * (* encrypt) (plugin_state * state, char * pure_msg);
    char * (* decrypt) (plugin_state * state, char * encrypted_msg);

};


extern const plugin_api PLUGIN_API;

#endif /* _PLUGIN_API_H_ */
