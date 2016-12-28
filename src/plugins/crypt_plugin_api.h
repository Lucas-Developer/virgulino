/*
 * crypt_plugin_api.h
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
#ifndef _CRYPT_PLUGIN_API_H_
#define _CRYPT_PLUGIN_API_H_

#include "plugin_api.h"

#define CRYPT_PLUGIN_API(obj) 	((crypt_plugin_api *)obj)
//typedef struct plugin_state_  plugin_state;

typedef struct crypt_plugin_api_t crypt_plugin_api;
struct crypt_plugin_api_t {
 	plugin_state * (* init) (const char * config_filepath);
	void (* finalize) (plugin_state * state);
	void (* reload) (plugin_state * state);
	void (* unload) (plugin_state * state);

	char * (* encrypt) (const plugin_state * state, char * pure_msg, void * key);
	char * (* decrypt) (const plugin_state * state, char * encrypted_msg, void * key);
 };

extern const crypt_plugin_api CRYPT_PLUGIN_API;

#endif /* _CRYPT_PLUGIN_API_H_ */
