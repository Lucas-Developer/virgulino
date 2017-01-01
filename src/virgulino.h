/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * virgulino.c
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

#ifndef _VIRGULINO_H_
#define _VIRGULINO_H_

#include "plugins/plugin_controller.h"
#include "output.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

#define LOCAL_CONFIG_FILE 		"./virgulino.conf"
#define WIDESYSTEM_CONFIG_FILE 	"/etc/virgulino.conf"
#define MAX_SIZE 				512

typedef struct message_ message_t;
struct message_ {
	char input_file[128];
	char output_file[128];
	char config_file[128];
	char crypt_plugin_name[32];
	char steg_plugin_name[32];

	char * key;
	char * msg;

	bool crypt,
		 decrypt,
		 steg;
};

char * search_plugin_dir (void);
int save_to_file (const char * filename, const char * content);
void message_free_elements (message_t * message);
void virgulino_exit (message_t * message, int status);
void encrypt (message_t * message);
void decrypt (message_t * message);
void exec (message_t * message, void (* action) (message_t * message));
void opt_handler (int argc, char ** argv);

//functions ::
char *
search_plugin_dir (void) {
	struct stat st = { 0 };
	FILE * fp = NULL;
	char * cp;
	char c;
	if (!stat (LOCAL_CONFIG_FILE, &st)) {
 		cp = (char *) wr_calloc(st.st_size, sizeof (char));
		fp = fopen (LOCAL_CONFIG_FILE, "r");
		if (fp) {
			int match = 0;
			int i = 0;
			while (!feof (fp)) {
				c = fgetc (fp);
				if (c == 0x0a) {
					break;
				}
				if (match == 2) {
					cp [i] = c;
					i++;
				} else if (match > 2) {
					break;
				}
				if (c == ' ') {
					match ++;
				}
			}
			fclose (fp);
			fflush (fp);
			debug (1);
			return cp;
		}
	} else 	if (!stat (WIDESYSTEM_CONFIG_FILE, &st)) {
 		cp = (char *) wr_calloc(st.st_size, sizeof (char));
		fp = fopen (WIDESYSTEM_CONFIG_FILE, "r");
		if (!fp) {
			fprintf (stderr, "ERROR OPENING THE FILE %s : ", WIDESYSTEM_CONFIG_FILE);
			perror ("");
			return NULL;
		}
		if (fp) {
			int match = 0;
			int i = 0;
			while (!feof (fp)) {
				c = fgetc (fp);
				if (c == 0x0a) {
					break;
				}
				if (match == 2) {
					cp [i] = c;
					i++;
				} else if (match > 2) {
					break;
				}
				if (c == ' ') {
					match ++;
				}
			}
			fclose (fp);
			fflush (fp);
		
			debug (2);
			return cp;
		}
	}
	debug (3);
	return NULL;
}

int
save_to_file (const char * filename, const char * content) {
	if (!content) {
		fprintf (stderr, "[!!] Invalid content [!!]\n");
		return -1;
	}

	FILE * fp = fopen (filename, "a");
	if (!fp) {
 		fprintf (stderr, "[!!] Error opening the file %s : ", filename);
		perror ("");
		return -1;
	}
	fprintf (fp, "%s", content);
	fclose (fp);
	fflush (fp);

	return 0;
}

void
message_free_elements (message_t * message) {
	if (message->key) { free (message->key); message->key = NULL; }
	if (message->msg) { free (message->msg); message->msg = NULL; }
}

void
virgulino_exit (message_t * message, int status) {
	message_free_elements (message);
	exit (status);
}

void
opt_handler (int argc, char ** argv) {
	int c = 0;
	message_t message = { 0 };

	if (argc == 1) {
	   usage ();
	   exit (EXIT_SUCCESS);
	}

	while (1) {
		static struct option long_options [] =  {
		   {"help", 	no_argument, 			0, 'h'},
		   {"encrypt", 	required_argument, 		0, 'e'},
		   {"decrypt", 	required_argument, 		0, 'd'},
		   {"key", 		required_argument, 		0, 'k'},
		   {"message", 	required_argument, 		0, 'm'},
		   {"stego", 	required_argument, 		0, 's'},
		   {"input", 	required_argument, 		0, 'i'},
		   {"output",  	optional_argument, 		0, 'o'},
		   {"config", 	required_argument, 		0, 'c'},
		   {0, 			0, 						0, 	0 }
	   };

		
       int option_index;
	   c = getopt_long (argc, argv, ":he:d:k:m:s:i:o::", long_options, &option_index);

       if (c == -1) 
           break;

       switch (c) {
           case 0: {
                if (long_options[option_index].flag != 0)
                    break;
                printf ("option %s", long_options [option_index].name);
                if (optarg)
                    printf ("with arg %s\n", optarg);
                break;

           } case 'e': {
			   message.crypt = true;
			   strncpy (message.crypt_plugin_name, 
						optarg, 
						sizeof (message.crypt_plugin_name));		   
			   break;
           } case 'd': {
			   message.decrypt = true;
			   strncpy (message.crypt_plugin_name, 
						optarg, 
						sizeof (message.crypt_plugin_name));
               break;

           } case 'k': {
			   message.key = strdup (optarg);
               break;

           } case 'm': {

			   message.msg = strdup (optarg);
               break;

           } case 's': {
			   message.steg = true;
			   strncpy (message.steg_plugin_name, 
						optarg, 
						sizeof (message.steg_plugin_name));
               break;
           } case 'i': {
			   strncpy (message.input_file, 
						optarg, 
						sizeof (message.input_file));
               break;

           } case 'o': {
			   if ( (!optarg 
					&& NULL != argv[optind]
					&& '-' != argv[optind][0]) ) {

					strncpy (message.output_file, 
							argv[optind++], 
							sizeof (message.output_file) );

			   }
			   break;

		   } case 'c': {
			   strncpy (message.config_file, 
						optarg, 
						sizeof (message.config_file));
			   break;

		   } case '?': {
               help ();
			   virgulino_exit(&message, EXIT_SUCCESS);
               break;

           } case 'h': {
               help ();
			   virgulino_exit(&message, EXIT_SUCCESS);
               break;

           } default: {
               usage ();
			   virgulino_exit(&message, EXIT_SUCCESS);
               break;

           }
       }
   }
   
   if (optind < argc) {
       printf ("non-option argv-elements: ");
       while (optind < argc) {
           printf ("%s ", argv[optind ++]);
       }
       putchar ('\n');
       usage ();
       exit (EXIT_SUCCESS);
   }
	//fun_stuff (&message);
	if (message.crypt) 
		exec (&message, encrypt);
	else
		exec (&message, decrypt);

	virgulino_exit (&message, EXIT_SUCCESS);
}



void 
exec (message_t * message, void (* action) (message_t * message)) {
	char * plugin_path = search_plugin_dir ();
	
	if (plugin_path == NULL) {
		fprintf (stderr, "[!!] No config file found, please take a look at the manpage [!!]\n");
		free (plugin_path);
		virgulino_exit (message, EXIT_SUCCESS);
	}
	free (plugin_path);
	if (!message->crypt && !message->decrypt) {
		usage ();

		free (plugin_path);
		virgulino_exit (message, EXIT_SUCCESS);
	}

	if (!message->key || 
				(!message->msg && !strlen (message->input_file) ) || 
				!strlen (message->crypt_plugin_name) ) {

			usage ();

			free (plugin_path);
			virgulino_exit(message, EXIT_SUCCESS);
	}

	action (message);
}

void encrypt (message_t * message) {
	crypt_plugin_t * crypt_plugin;
	steg_plugin_t * steg_plugin;

	char plugin_full_path [MAX_SIZE];
	char * plugin_path = NULL;
	plugin_path = search_plugin_dir ();
	
	snprintf (plugin_full_path, sizeof (plugin_full_path), 
			"%s/crypt/%s.so",plugin_path, message->crypt_plugin_name);
	printf ("%s\n", plugin_full_path);
	crypt_plugin = (crypt_plugin_t *) plugin_load (CRYPTO_PLUGIN, 
														plugin_full_path, 
														message->config_file);
	if (message->msg) {
		crypt_plugin->api.encrypt (crypt_plugin->parent.state, 
									message->msg, message->key);

		plugin_unload (crypt_plugin, CRYPTO_PLUGIN);
		free (message->key);
		message->key = NULL;

			if (message->steg) {
			snprintf (plugin_full_path, sizeof (plugin_full_path), "%s/steg/%s.so", 
														plugin_path,
														message->steg_plugin_name);
				steg_plugin = plugin_load (STEG_PLUGIN, plugin_full_path, message->config_file);

				if (!message->output_file) {
					fprintf (stderr, "[!!] no output_file paramenter found [!!]\n");
					plugin_unload (steg_plugin, STEG_PLUGIN);
					free (plugin_path);
					virgulino_exit (message, EXIT_SUCCESS);

				}

				steg_plugin->api.hide (steg_plugin->parent.state,
										message->msg, 
										message->output_file);

			plugin_unload (steg_plugin, STEG_PLUGIN);
		}
	}
	free (plugin_path);
}

void
decrypt (message_t * message) {
	crypt_plugin_t * crypt_plugin;
	steg_plugin_t * steg_plugin;

	char plugin_full_path [MAX_SIZE];
	char * plugin_path = NULL;
	plugin_path = search_plugin_dir ();
	snprintf (plugin_full_path, sizeof (plugin_full_path), "%s/steg/%s.so", 
												plugin_path,
												message->steg_plugin_name);
	steg_plugin = plugin_load (STEG_PLUGIN, plugin_full_path, message->config_file);
	if (!message->output_file) {
		fprintf (stderr, "[!!] no output_file paramenter found [!!]\n");
		plugin_unload (steg_plugin, STEG_PLUGIN);
		free (plugin_path);
		virgulino_exit (message, EXIT_SUCCESS);
	}
	char * content = steg_plugin->api.unhide (steg_plugin->parent.state, 
												message->input_file);
	plugin_unload (steg_plugin, STEG_PLUGIN);
	if (!content || !message->crypt_plugin_name) {
		free (plugin_path);
		virgulino_exit(message, EXIT_FAILURE);
	}
	snprintf (plugin_full_path, sizeof (plugin_full_path), "%s/crypt/%s.so",
												plugin_path,
												message->crypt_plugin_name);
	crypt_plugin = (crypt_plugin_t *) plugin_load (CRYPTO_PLUGIN, 
												plugin_full_path, 
												message->config_file);
	crypt_plugin->api.decrypt (crypt_plugin->parent.state, 
								content, message->key);
	plugin_unload (crypt_plugin, CRYPTO_PLUGIN);

	if (strlen (message->output_file) > 0) {
		save_to_file (message->output_file, content);
	} else {
		printf ("BEGIN MESSAGE\n\n%s\n\nEND MESSAGE\n", content);
	}

	free (content);
	free (plugin_path);
}


#endif /* _VIRGULINO_H_ */

