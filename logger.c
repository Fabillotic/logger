/*
 * logger v1.0.1
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
#include <stdarg.h>
#include "logger.h"

struct verbosity_level levels[NUM_VERBOSITY_LEVELS] = {
	{.id = NONE , .color_text = NONE_TEXT_COLOR,  .plain_text = NONE_TEXT_PLAIN  },
	{.id = INFO , .color_text = INFO_TEXT_COLOR,  .plain_text = INFO_TEXT_PLAIN  },
	{.id = WARN , .color_text = WARN_TEXT_COLOR,  .plain_text = WARN_TEXT_PLAIN  },
	{.id = ERROR, .color_text = ERROR_TEXT_COLOR, .plain_text = ERROR_TEXT_PLAIN },
	{.id = DEBUG, .color_text = DEBUG_TEXT_COLOR, .plain_text = DEBUG_TEXT_PLAIN },
};

struct logger_profile *profiles = NULL;

char *section = NULL;
int indent = -1;

void log_init(int max_verbosity, char *fn) {
	FILE *f;
	struct logger_profile *profile, *parent;
	
	profile = malloc(sizeof(struct logger_profile));
	profile->next = NULL;
	profile->verbosity = max_verbosity;
	profile->logfile = fn;
	
	for(parent = profiles; parent && parent->next != NULL; parent = parent->next);
	if(parent) parent->next = profile;
	else profiles = profile;
	
	//Clear logfile
	if(profile->logfile) {
		f = fopen(profile->logfile, "w");
		fclose(f);
	}
}

void _log_print(char *s) {
	struct logger_profile *profile;
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
	int i;
	size_t len, dp;
	char* data;
	
	section = name;
	indent++;
	dp = 0;
	
	data = malloc(sizeof(char) * (SECTION_LEN + (indent * 4) + 3));
	
	data[dp++] = '\n';
	
	for(i = 0; i < indent * 4; i++) data[dp++] = ' ';
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
	struct logger_profile *profile;
	int i;
	FILE *f;
	char* prefix_text = NULL;
	
	for(profile = profiles; profile; profile = profile->next) {
		if(!(level & profile->verbosity)) continue;
		
		if(profile->logfile) {
			f = fopen(profile->logfile, "a");
			for(i = 0; i < indent * 4; i++) fputc(' ', f);
			for(i = 0; i < NUM_VERBOSITY_LEVELS; i++) {
				if(levels[i].id == level) {
					prefix_text = levels[i].plain_text;
					break;
				}
			}
			if(!prefix_text) {
				for(i = 0; i < NUM_VERBOSITY_LEVELS; i++) {
					if(levels[i].id == NONE) {
						prefix_text = levels[i].plain_text;
						break;
					}
				}
			}
			fprintf(f, prefix_text);
			va_start(args, format);
			vfprintf(f, format, args);
			va_end(args);
			fclose(f);
		}
		else {
			for(i = 0; i < indent * 4; i++) putchar(' ');
			for(i = 0; i < NUM_VERBOSITY_LEVELS; i++) {
				if(levels[i].id == level) {
					prefix_text = levels[i].color_text;
					break;
				}
			}
			if(!prefix_text) {
				for(i = 0; i < NUM_VERBOSITY_LEVELS; i++) {
					if(levels[i].id == NONE) {
						prefix_text = levels[i].color_text;
						break;
					}
				}
			}
			printf(prefix_text);
			va_start(args, format);
			vprintf(format, args);
			va_end(args);
		}
	}
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
