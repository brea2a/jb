#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#ifndef WIN32
# include <err.h>
#endif
#include "json.h"
#include "base64.h"

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

#define SPACER		"   "
#define FLAG_ARRAY	0x01
#define FLAG_PRETTY	0x02
#define FLAG_NOBOOL	0x04
#define FLAG_BOOLEAN	0x08
#define FLAG_MASK	(FLAG_ARRAY | FLAG_PRETTY | FLAG_NOBOOL | FLAG_BOOLEAN)

static JsonNode *pile;		/* pile of nested objects/arrays */

#ifdef _WIN32
# define err(n, s)	{ fprintf(stderr, s); exit(n); }
# define errx(n, f, a)	{ fprintf(stderr, f, a); exit(n); }
# define fseeko	fseek
# define ftello	ftell
#endif

JsonTag flags_to_tag(int flags) {
	return flags / (FLAG_MASK + 1);
}

int tag_to_flags(JsonTag tag) {
	return (FLAG_MASK + 1) * tag;
}

void json_copy_to_object(JsonNode * obj, JsonNode * object_or_array, int clobber)
{
	JsonNode *node;

	if (obj->tag != JSON_OBJECT && obj->tag != JSON_ARRAY)
		return;

	json_foreach(node, object_or_array) {
		if (!clobber & (json_find_member(obj, node->key) != NULL))
			continue;	/* Don't clobber existing keys */
		if (obj->tag == JSON_OBJECT) {
			if (node->tag == JSON_STRING)
				json_append_member(obj, node->key, json_mkstring(node->string_));
			else if (node->tag == JSON_NUMBER)
				json_append_member(obj, node->key, json_mknumber(node->number_));
			else if (node->tag == JSON_BOOL)
				json_append_member(obj, node->key, json_mkbool(node->bool_));
			else if (node->tag == JSON_NULL)
				json_append_member(obj, node->key, json_mknull());
			else
				fprintf(stderr, "PANIC: unhandled JSON type %d\n", node->tag);
		} else if (obj->tag == JSON_ARRAY) {
			if (node->tag == JSON_STRING)
				json_append_element(obj, json_mkstring(node->string_));
			if (node->tag == JSON_NUMBER)
				json_append_element(obj, json_mknumber(node->number_));
			if (node->tag == JSON_BOOL)
				json_append_element(obj, json_mkbool(node->bool_));
			if (node->tag == JSON_NULL)
				json_append_element(obj, json_mknull());
		}
	}
}

char *slurp_file(FILE *fp, size_t *out_len, bool fold_newlines)
{
	char *buf, *bp;
	int ch;
	off_t file_len;

	if (fseeko(fp, 0, SEEK_END) != 0) {
		fclose(fp);
		return (NULL);
	}
	file_len = ftello(fp);
	fseeko(fp, 0, SEEK_SET);

	if ((bp = buf = malloc(file_len + 1)) == NULL) {
		fclose(fp);
		return (NULL);
	}
	while ((ch = fgetc(fp)) != EOF) {
		if (ch != '\n' || !fold_newlines)
			*bp++ = ch;
	}
	*bp = 0;
	*out_len = bp - buf;
	return (buf);
}

JsonNode *jo_mknull(JsonTag type) {
	switch (type) {
		case JSON_STRING:
			return json_mkstring("");
			break;
		case JSON_NUMBER:
			return json_mknumber(0);
			break;
		case JSON_BOOL:
			return json_mkbool(false);
			break;
		default:
			return json_mknull();
			break;
	}
}

JsonNode *jo_mkbool(bool b, JsonTag type) {
	switch (type) {
		case JSON_STRING:
			return json_mkstring(b ? "true" : "false");
			break;
		case JSON_NUMBER:
			return json_mknumber(b ? 1 : 0);
			break;
		default:
			return json_mkbool(b);
			break;
	}
}

JsonNode *jo_mkstring(char *str, JsonTag type) {
	switch (type) {
		case JSON_NUMBER:
			/* Length of string */
			return json_mknumber(strlen(str));
			break;
		case JSON_BOOL:
			/* True if not empty */
			return json_mkbool(strlen(str) > 0);
			break;
		default:
			return json_mkstring(str);
			break;
	}
}

JsonNode *jo_mknumber(char *str, JsonTag type) {
	/* ASSUMPTION: str already tested as valid number */
	double n = strtod(str, NULL);

	switch (type) {
		case JSON_STRING:
			/* Just return the original representation */
			return json_mkstring(str);
			break;
		case JSON_BOOL:
			return json_mkbool(n != 0);
			break;
		default:
			/* ASSUMPTION: str already tested as valid number */
			return json_mknumber(n);
			break;
	}
}

/*
 * Attempt to "sniff" the type of data in `str' and return
 * a JsonNode of the correct JSON type.
 */

JsonNode *vnode(char *str, int flags)
{
	JsonTag type = flags_to_tag(flags);

	if (strlen(str) == 0) {
		return jo_mknull(type);
	}

	/* If str begins with a double quote, keep it a string */

	if (*str == '"') {
#if 0
		char *bp = str + strlen(str) - 1;

		if (bp > str && *bp == '"')
			*bp = 0;		/* Chop closing double quote */
		return json_mkstring(str + 1);
#endif
		return jo_mkstring(str, type);
	}

	char *endptr;
	double num = strtod(str, &endptr);

	if (!*endptr && isfinite(num)) {
		return jo_mknumber(str, type);
	}

	if (!(flags & FLAG_NOBOOL)) {
		if (strcmp(str, "true") == 0) {
			return jo_mkbool(true, type);
		} else if (strcmp(str, "false") == 0) {
			return jo_mkbool(false, type);
		} else if (strcmp(str, "null") == 0) {
			return jo_mknull(type);
		}
	}

	if (*str == '@' || *str == '%') {
		char *filename = str + 1, *content;
		bool binmode = (*str == '%');
		size_t len = 0;
		JsonNode *j;
		FILE *fp;

		if ((fp = fopen(filename, binmode ? "rb" : "r")) == NULL) {
			errx(1, "Cannot open %s for reading", filename);
		}

		if ((content = slurp_file(fp, &len, false)) == NULL) {
			errx(1, "Error reading file %s", filename);
		}

		fclose(fp);

		if (binmode) {
			char *encoded;

			if ((encoded = base64_encode(content, len)) == NULL) {
				errx(1, "Cannot base64-encode file %s", filename);
			}

			j = json_mkstring(encoded);
			free(encoded);
		} else {
			char *bp = content + strlen(content) - 1;

			if (*bp == '\n') *bp-- = 0;
			if (*bp == '\r') *bp = 0;
			j = json_mkstring(content);
		}
		free(content);
		return (j);
	}

	if (*str == '{' || *str == '[') {
		JsonNode *obj = json_decode(str);

		if (obj == NULL) {
			/* JSON cannot be decoded; return the string */
			// fprintf(stderr, "Cannot decode JSON from %s\n", str);

			obj = json_mkstring(str);
		}

		return (obj);
	}

	return jo_mkstring(str, type);
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

	if (tolower((unsigned char) *str) == 't') {
		return json_mkbool(1);
	}

	return json_mkbool(atoi(str));
}

int usage(char *prog)
{
	fprintf(stderr, "Usage: %s [-a] [-B] [-p] [-v] [-V] [word...]\n", prog);
	fprintf(stderr, "\tword is key=value or key@value\n");
	fprintf(stderr, "\t-a creates an array of words\n");
	fprintf(stderr, "\t-B disable boolean true/false\n");
	fprintf(stderr, "\t-p pretty-prints JSON on output\n");
	fprintf(stderr, "\t-v show version\n");
	fprintf(stderr, "\t-V show version in JSON\n");

	return (-1);
}

/*
 * Check whether we're being given nested arrays or objects.
 * `kv' contains the "key" such as "number" or "point[]" or
 * "geo[lat]". `value' the actual value for that element.
 */

bool nested(int flags, char *key, char *value)
{
	char *member = NULL, *bo, *bc;		/* bracket open, close */
	JsonNode *op;
	int found = false;

	/* Check for geo[] or geo[lat] */
	if ((bo = strchr(key, '[')) != NULL) {
		if (*(bo+1) == ']') {
			*bo = 0;
		} else if ((bc = strchr(bo + 1, ']')) == NULL) {
			fprintf(stderr, "missing closing bracket on %s\n", key);
			return (-1);
		} else {
			*bo = *bc = 0;
			member = bo + 1;
		}

		/*
		 * key is now `geo' for both `geo[]` and `geo[lat]`
		 * member is null for the former and "lat" for the latter.
		 * Find an existing object in the pile for this member name
		 * or create a new one if we don't have it.
		 */

		if ((op = json_find_member(pile, key)) != NULL) {
			found = true;
		} else {
			op = (member == NULL) ? json_mkarray() : json_mkobject();
		}

		if (member == NULL) {		/* we're doing an array */
			if (flags & FLAG_BOOLEAN) {
				json_append_element(op, boolnode(value));
			} else {
				json_append_element(op, vnode(value, flags));
			}
		} else {			/* we're doing an object */
			if (flags & FLAG_BOOLEAN) {
				json_append_member(op, member, boolnode(value));
			} else {
				json_append_member(op, member, vnode(value, flags));
			}
		}

		if (!found) {
			json_append_member(pile, key, op);
		}

		return (true);
	}
	return (false);
}

int member_to_object(JsonNode *object, int flags, char *kv)
{
	/* we expect key=value or key:value (boolean on last) */
	char *p = strchr(kv, '=');
	char *q = strchr(kv, '@');
	char *r = strchr(kv, ':');

	if ((r && *(r+1) == '=') && !q) {
		FILE *fp;
		char *filename = p + 1;
		char *content;
		size_t len;

		if ((fp = fopen(filename, "r")) == NULL) {
			errx(1, "Cannot open %s for reading", filename);
		}
		if ((content = slurp_file(fp, &len, false)) == NULL) {
			errx(1, "Error reading file %s", filename);
		}

		JsonNode *o = json_decode(content);
		free(content);
		fclose(fp);

		if (o == NULL) {
			errx(1, "Cannot decode JSON in file %s", filename);
		}

		*r = 0;		/* Chop at ":=" */
		json_append_member(object, kv, o);
		return (0);
	}

	if (!p && !q && !r) {
		return (-1);
	}


	if (p) {
		if (p) {
			*p = 0;

			if (nested(flags, kv, p+1))
				 return (0);
			json_append_member(object, kv, vnode(p+1, flags));
		}
	} else {
		if (q) {
			*q = 0;

			if (nested(flags | FLAG_BOOLEAN, kv, q+1))
				 return (0);
			json_append_member(object, kv, boolnode(q+1));
		}
	}
	return (0);
}

/*
 * Append kv to the array or object.
 */

void append_kv(JsonNode *object_or_array, int flags, char *kv)
{
	if (flags & FLAG_ARRAY) {
		json_append_element(object_or_array, vnode(kv, flags));
	} else {
		if (member_to_object(object_or_array, flags, kv) == -1) {
			fprintf(stderr, "Argument `%s' is neither k=v nor k@v\n", kv);
		}
	}
}

#ifdef _WIN32
#include <windows.h>
char* utf8_from_locale(const char *str, size_t len)
{
	wchar_t* wcsp;
	char* mbsp;
	size_t mbssize, wcssize;

	if (len == 0) {
		return strdup("");
	}
	if (len == -1) {
		len = strlen(str);
	}
	wcssize = MultiByteToWideChar(GetACP(), 0, str, len,  NULL, 0);
	wcsp = (wchar_t*) malloc((wcssize + 1) * sizeof(wchar_t));
	if (!wcsp) {
		return NULL;
	}
	wcssize = MultiByteToWideChar(GetACP(), 0, str, len, wcsp, wcssize + 1);
	wcsp[wcssize] = 0;

	mbssize = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR) wcsp, -1, NULL, 0, NULL, NULL);
	mbsp = (char*) malloc((mbssize + 1));
	if (!mbsp) {
		free(wcsp);
		return NULL;
	}
	mbssize = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR) wcsp, -1, mbsp, mbssize, NULL, NULL);
	mbsp[mbssize] = 0;
	free(wcsp);
	return mbsp;
}
# define utf8_free(p) free(p)

char* locale_from_utf8(const char *utf8, size_t len)
{
	wchar_t* wcsp;
	char* mbsp;
	size_t mbssize, wcssize;

	if (len == 0) {
		return strdup("");
	}
	if (len == -1) {
		len = strlen(utf8);
	}
	wcssize = MultiByteToWideChar(CP_UTF8, 0, utf8, len,  NULL, 0);
	wcsp = (wchar_t*) malloc((wcssize + 1) * sizeof(wchar_t));
	if (!wcsp) {
		return NULL;
	}
	wcssize = MultiByteToWideChar(CP_UTF8, 0, utf8, len, wcsp, wcssize + 1);
	wcsp[wcssize] = 0;
	mbssize = WideCharToMultiByte(GetACP(), 0, (LPCWSTR) wcsp, -1, NULL, 0, NULL, NULL);
	mbsp = (char*) malloc((mbssize + 1));
	if (!mbsp) {
		free(wcsp);
		return NULL;
	}
	mbssize = WideCharToMultiByte(GetACP(), 0, (LPCWSTR) wcsp, -1, mbsp, mbssize, NULL, NULL);
	mbsp[mbssize] = 0;
	free(wcsp);
	return mbsp;
}
# define locale_free(p) free(p)
#else
# define utf8_from_locale(p, l) (p)
# define utf8_free(p)
# define locale_from_utf8(p, l) (p)
# define locale_free(p)
#endif

char *stringify(JsonNode *json, int flags)
{
	int pretty = flags & FLAG_PRETTY;

	return json_stringify(json, (pretty) ? SPACER : NULL);
}

int version(int flags)
{
	JsonNode *json = json_mkobject();
	char *js;

	json_append_member(json, "program", json_mkstring("jo"));
	json_append_member(json, "author", json_mkstring("Jan-Piet Mens"));
	json_append_member(json, "repo", json_mkstring("https://github.com/jpmens/jo"));
	json_append_member(json, "version", json_mkstring(PACKAGE_VERSION));

	if ((js = stringify(json, flags)) != NULL) {
		printf("%s\n", js);
		free(js);
	}
	json_delete(json);
	return (0);
}

int main(int argc, char **argv)
{
	int c;
	bool showversion = false;
	char *kv, *js_string, *progname, buf[BUFSIZ], *p;
	int ttyin = isatty(fileno(stdin)), ttyout = isatty(fileno(stdout));
	int flags = 0;
	JsonNode *json, *op;

#if HAVE_PLEDGE
	if (pledge("stdio rpath", NULL) == -1) {
		err(1, "pledge");
	}
#endif

	progname = (progname = strrchr(*argv, '/')) ? progname + 1 : *argv;

	while ((c = getopt(argc, argv, "aBhpvV")) != EOF) {
		switch (c) {
			case 'a':
				flags |= FLAG_ARRAY;
				break;
			case 'B':
				flags |= FLAG_NOBOOL;
				break;
			case 'h':
				usage(progname);
				return (0);
			case 'p':
				flags |= FLAG_PRETTY;
				break;
			case 'v':
				printf("jo %s\n", PACKAGE_VERSION);
				exit(0);
			case 'V':
				showversion = true;
				break;
			default:
				exit(usage(progname));
		}
	}

	if (showversion) {
		return(version(flags));
	}

	argc -= optind;
	argv += optind;

	pile = json_mkobject();
	json = (flags & FLAG_ARRAY) ? json_mkarray() : json_mkobject();

	if (argc == 0) {
		while (fgets(buf, sizeof(buf), stdin) != NULL) {
			if (buf[strlen(buf) - 1] == '\n')
				buf[strlen(buf) - 1] = 0;
			p = ttyin ? utf8_from_locale(buf, -1) : buf;
			append_kv(json, flags, p);
			if (ttyin) utf8_free(p);
		}
	} else {
		while ((kv = *argv++)) {
			if (kv[0] == '-') {
				/* Set one-shot coerce flag */
				switch (kv[1]) {
					case 'b':
						flags |= tag_to_flags(JSON_BOOL);
						break;
					case 's':
						flags |= tag_to_flags(JSON_STRING);
						break;
					case 'n':
						flags |= tag_to_flags(JSON_NUMBER);
						break;
					default:
						exit(usage(progname));
				}
			} else {
				p = utf8_from_locale(kv, -1);
				append_kv(json, flags, p);
				utf8_free(p);
				/* Reset any one-shot coerce flags */
				flags &= FLAG_MASK;
			}
		}
	}

	/*
	 * See if we have any nested objects or arrays in the pile,
	 * and copy these into our main object if so.
	 */

	json_foreach(op, pile) {
		JsonNode *o;

		if (op->tag == JSON_ARRAY) {
			o = json_mkarray();
		} else if (op->tag == JSON_OBJECT) {
			o = json_mkobject();
		} else {
			continue;
		}
		json_copy_to_object(o, op, 0);
		json_append_member(json, op->key, o);
	}


	if ((js_string = stringify(json, flags)) == NULL) {
		fprintf(stderr, "Invalid JSON\n");
		exit(2);
	}

	p = ttyout ? locale_from_utf8(js_string, -1) : js_string;
	printf("%s\n", p);
	if (ttyout) locale_free(p);
	free(js_string);
	json_delete(json);
	json_delete(pile);
	return (0);
}
