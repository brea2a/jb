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
	fprintf(stderr, "Usage: %s [-a] [-p] [word word...]\n", prog);
	fprintf(stderr, "\tword is key=value or key@value\n");
	fprintf(stderr, "\t-a creates an array of words, -p pretty-prints\n");

	return (-1);
}

int member_to_object(JsonNode *object, char *kv)
{
	/* we expect key=value or key:value (boolean on last) */
	char *p = strchr(kv, '=');
	char *q = strchr(kv, '@');

	if (!p && !q) {
		return (-1);
	}

	if (p) {
		*p = 0;

		json_append_member(object, kv, vnode(p+1));
	} else {
		*q = 0;
		json_append_member(object, kv, boolnode(q+1));
	}
	return (0);
}

/*
 * Append kv to the array or object.
 */

void append_kv(JsonNode *object_or_array, int isarray, char *kv)
{
	if (isarray) {
		json_append_element(object_or_array, vnode(kv));
	} else {
		if (member_to_object(object_or_array, kv) == -1) {
			fprintf(stderr, "Argument `%s' is neither k=v nor k@v\n", kv);
		}
	}
}

int main(int argc, char **argv)
{
	int c, isarray = FALSE;
	char *kv, *js_string, *progname, *pretty = NULL, buf[BUFSIZ];
	JsonNode *json;

	progname = (progname = strrchr(*argv, '/')) ? ++progname : *argv;

	while ((c = getopt(argc, argv, "ap")) != EOF) {
		switch (c) {
			case 'a':
				isarray = TRUE;
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

	json = (isarray) ? json_mkarray() : json_mkobject();

	if (argc == 0) {
		while (fgets(buf, sizeof(buf), stdin) != NULL) {
			if (buf[strlen(buf) - 1] == '\n')
				buf[strlen(buf) - 1] = 0;
			append_kv(json, isarray, buf);
		}
	} else {
		while ((kv = *argv++)) {
			append_kv(json, isarray, kv);
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
