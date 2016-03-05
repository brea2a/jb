#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include "json.h"

/*
 * Copyright (C) 2016 Jan-Piet Mens <jpmens@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef TRUE
# define TRUE 	(1)
# define FALSE	(0)
#endif

/*
 * Attempt to "sniff" the type of data in `str' and return
 * a JsonNode of the correct JSON type.
 */

JsonNode *vnode(char *str)
{

	if (strlen(str) == 0) {
		return json_mknull();
	}

	if (strspn(str, "01234567890") == strlen(str)) {
		return json_mknumber(atol(str));
	}

	if (strspn(str, "01234567890.") == strlen(str)) {
		if (strchr(str, '.') == strrchr(str, '.')) {
			return json_mknumber(atof(str));
		} else {
			return json_mkstring(str);
		}
	}

	return json_mkstring(str);
}

/*
 * Attempt to sniff `str' into a boolean; return a
 * corresponding JsonNode for it.
 */

JsonNode *boolnode(char *str)
{
	if (strlen(str) == 0) {
		return json_mknull();
	}

	if (tolower(*str) == 't') {
		return json_mkbool(1);
	}

	return json_mkbool(atoi(str));
}

int usage(char *prog)
{
	fprintf(stderr, "Usage: %s [-a] [-p] word [word...]\n", prog);
	fprintf(stderr, "\tword is key=value or key:value\n");
	fprintf(stderr, "\t-a creates an array of words, -p pretty-prints\n");

	return (-1);
}

int main(int argc, char **argv)
{
	int c, array = FALSE;
	char *kv, *js_string, *progname, *pretty = NULL;
	JsonNode *json;

	progname = (progname = strrchr(*argv, '/')) ? ++progname : *argv;

	while ((c = getopt(argc, argv, "ap")) != EOF) {
		switch (c) {
			case 'a':
				array = TRUE;
				break;
			case 'p':
				pretty = " ";
				break;
			default:
				exit(usage(progname));
		}
	}

	argc -= optind;
	argv += optind;

	if (argc == 0) {
		exit(usage(progname));
	}

	json = (array) ? json_mkarray() : json_mkobject();

	while ((kv = *argv++)) {
		if (array) {
			json_append_element(json, vnode(kv));
		} else {
			/* we expect key=value or key:value (boolean on last) */
			char *p = strchr(kv, '=');
			char *q = strchr(kv, ':');

			if (!p && !q) {
				fprintf(stderr, "%s: Argument `%s' is neither k=v nor k:v\n", progname, kv);
				continue;
			}

			if (p) {
				*p = 0;

				json_append_member(json, kv, vnode(p+1));
			} else {
				*q = 0;
				json_append_member(json, kv, boolnode(q+1));
			}
		}
	}

	if ((js_string = json_stringify(json, pretty)) == NULL) {
		fprintf(stderr, "Invalid JSON\n");
		exit(2);
	}

	printf("%s\n", js_string);
	free(js_string);
	json_delete(json);
	return (0);
}
