/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2016 userx <user_x@riseup.net>
 *
 * virgulino is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * virgulino is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* @note: Compiling instructions
 * gcc -pedantic -Wall -O2 -fPIC -shared plugin.* -o plugin.so -ldl
 * gcc -pedantic -Wall -O2 -fPIC virgulino.h virgulino.c -c 
 * gcc -pedantic -Wall -O2 -fPIC virgulino.o main.c -o main -ldl
 */

#include <stdio.h>
#include "crypt_plugin_controller.h"
void

