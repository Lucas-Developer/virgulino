/*
 * plugin_api.h
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
#ifndef _PLUGIN_API_H_
#define _PLUGIN_API_H_

/*
 * **@Note:
 * 	plugin_state aims to encapsulate all the plugin's
 * private data. It must be properly implemented by the
 * plugin. The plugin's functions unload, reload and init
 * might be used to initialize, allocate and deallocate the
 * plugin_state within the plugin itself.
 * 
 */ 
typedef struct plugin_state_ plugin_state;


#endif /* _PLUGIN_API_H_ */
