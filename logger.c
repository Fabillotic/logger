/*
 * logger v1.0.5
 *
 * The MIT License (MIT)
 * Copyright (c) 2022 Fabillotic
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the “Software”), to deal in the Software without 
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include "logger.h"

struct verbosity_level levels[NUM_VERBOSITY_LEVELS] = {
	{ NONE , NONE_TEXT_COLOR,  NONE_TEXT_PLAIN  },
	{ INFO , INFO_TEXT_COLOR,  INFO_TEXT_PLAIN  },
	{ WARN , WARN_TEXT_COLOR,  WARN_TEXT_PLAIN  },
	{ ERROR, ERROR_TEXT_COLOR, ERROR_TEXT_PLAIN },
	{ DEBUG, DEBUG_TEXT_COLOR, DEBUG_TEXT_PLAIN },
};

void _log_print(char *s);

LOGGER *profiles = NULL;

char *section = NULL;
int indent = -1;

LOGGER *log_init(size_t max_verbosity, char *fn, bool color) {
	FILE *f;
	LOGGER *profile, *parent;
	
	profile = malloc(sizeof(LOGGER));
	profile->next = NULL;
	profile->verbosity = max_verbosity;
	profile->logfile = fn;
	profile->colored_output = color;
	
	for(parent = profiles; parent && parent->next; parent = parent->next);
	if(parent) parent->next = profile;
	else profiles = profile;
	
	/* Clear logfile */
	if(profile->logfile) {
		f = fopen(profile->logfile, "w");
		fclose(f);
	}
	
	return profile;
}

void log_del(LOGGER *profile) {
	LOGGER *parent;
	
	if(profile == profiles) profiles = profile->next;
	else {
		for(parent = profiles; parent; parent = parent->next) {
			if(parent->next == profile) parent->next = profile->next;
		}
	}
	free(profile);
}

void _log_print(char *s) {
	LOGGER *profile;
	FILE *f;
	
	for(profile = profiles; profile; profile = profile->next) {
		if(profile->logfile) {
			f = fopen(profile->logfile, "a");
			fprintf(f, "%s", s);
			fclose(f);
		}
		else {
			printf("%s", s);
		}
	}
}

void log_start_section(char* name) {
	unsigned int i;
	int j;
	size_t len, dp;
	char* data;
	
	section = name;
	indent++;
	dp = 0;
	
	data = malloc(sizeof(char) * (SECTION_LEN + (indent * 4) + 3));
	
	data[dp++] = '\n';
	
	for(j = 0; j < indent * 4; j++) data[dp++] = ' ';
	for(len = 0; section[len] != '\0'; len++);
	for(i = 0; i < (SECTION_LEN - len) / 2 - 1; i++) data[dp++] = '=';
	data[dp++] = ' ';
	for(i = 0; i < len; i++) data[dp++] = section[i];
	data[dp++] = ' ';
	for(i = 0; i < (SECTION_LEN - len) / 2 + (SECTION_LEN - len) % 2 - 1; i++) data[dp++] = '=';
	data[dp++] = '\n';
	data[dp++] = '\0';
	
	_log_print(data);
	free(data);
}

void log_print(size_t level, const char *format, ...) {
	va_list args;
	LOGGER *profile;
	int i, j, none_prefix_len, format_len, nl, dp, chars_alloc;
	FILE *f;
	char *prefix_text = NULL, *nformat = NULL;
	bool no_trailing_nl, trailing_nl_missing;
	
	for(format_len = 0; format[format_len] != '\0'; format_len++);
	for(none_prefix_len = 0; NONE_TEXT_PLAIN[none_prefix_len] != '\0'; none_prefix_len++);
	
	nl = 0;
	for(i = 0; i < format_len; i++) {
		if(format[i] == '\n') nl++;
	}
	
	no_trailing_nl = format[format_len-1] != '\n';
	
	nl+=no_trailing_nl; /* Final total new lines */
	
	chars_alloc = (
		format_len +
		1 + /* null-term */
		no_trailing_nl +
		(nl - 1) * (none_prefix_len + (indent < 0 ? 0 : indent) * 4) /* extra prefixes minus initial, indentation */
	);
	nformat = malloc(sizeof(char) * chars_alloc);
	dp = 0;
	
	for(i = 0; i < format_len; i++) {
		nformat[dp++] = format[i];
		trailing_nl_missing = (no_trailing_nl && format[i+1] == '\0');
		if(trailing_nl_missing) {
			nformat[dp++] = '\n';
		}
		else if(format[i] == '\n' && format[i+1] != '\0') {
			for(j = 0; j < 4 * indent; j++) nformat[dp++] = ' ';
			for(j = 0; j < none_prefix_len; j++) nformat[dp++] = NONE_TEXT_PLAIN[j];
		}
	}
	nformat[dp++] = '\0';
	
	for(profile = profiles; profile; profile = profile->next) {
		if(!(level & profile->verbosity)) continue;
		
		for(i = 0; i < NUM_VERBOSITY_LEVELS; i++) {
			if(levels[i].id == level) {
				if(profile->colored_output) prefix_text = levels[i].color_text;
				else prefix_text = levels[i].plain_text;
				break;
			}
		}
		if(!prefix_text) {
			for(i = 0; i < NUM_VERBOSITY_LEVELS; i++) {
				if(levels[i].id == NONE) {
					if(profile->colored_output) prefix_text = levels[i].color_text;
					else prefix_text = levels[i].plain_text;
					break;
				}
			}
		}
		
		if(profile->logfile) {
			f = fopen(profile->logfile, "a");
			for(i = 0; i < indent * 4; i++) fputc(' ', f);
			fprintf(f, prefix_text);
			va_start(args, format);
			vfprintf(f, nformat, args);
			va_end(args);
			fclose(f);
		}
		else {
			for(i = 0; i < indent * 4; i++) putchar(' ');
			printf(prefix_text);
			va_start(args, format);
			vprintf(nformat, args);
			va_end(args);
		}
	}
	
	free(nformat);
}

void log_end_section() {
	int i;
	size_t dp = 0;
	char* data;

	if(indent > -1) {
		data = malloc(sizeof(char) * (SECTION_LEN + (indent * 4) + 3));
		
		for(i = 0; i < indent * 4; i++) data[dp++] = ' ';
		for(i = 0; i < SECTION_LEN; i++) data[dp++] = '-';
		data[dp++] = '\n';
		data[dp++] = '\n';
		data[dp++] = '\0';
		
		_log_print(data);
		
		free(data);
		indent--;
	}
}
