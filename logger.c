#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "logger.h"

struct prefix prefixes[NUM_PREFIXES] = {
	{.id = NONE , .color_text = NONE_TEXT_COLOR,  .plain_text = NONE_TEXT_PLAIN  },
	{.id = INFO , .color_text = INFO_TEXT_COLOR,  .plain_text = INFO_TEXT_PLAIN  },
	{.id = WARN , .color_text = WARN_TEXT_COLOR,  .plain_text = WARN_TEXT_PLAIN  },
	{.id = ERROR, .color_text = ERROR_TEXT_COLOR, .plain_text = ERROR_TEXT_PLAIN },
	{.id = DEBUG, .color_text = DEBUG_TEXT_COLOR, .plain_text = DEBUG_TEXT_PLAIN },
};

char *section = NULL;
char *logfile = NULL;
int indent = -1;
size_t log_stdout = 0;

void log_init(char *fn, size_t mirror) {
	FILE *f;
	
	logfile = fn;
	log_stdout = mirror;
	
	//Clear logfile
	if(logfile) {
		f = fopen(logfile, "w");
		fclose(f);
	}
}

void _log_print(char *s) {
	FILE *f;
	
	if(logfile) {
		f = fopen(logfile, "a");
		fprintf(f, "%s", s);
		fclose(f);
	}
	if(log_stdout) {
		printf("%s", s);
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
	for(i = 0; i < (SECTION_LEN - len) / 2; i++) data[dp++] = '=';
	for(i = 0; i < len; i++) data[dp++] = section[i];
	for(i = 0; i < (SECTION_LEN - len) / 2 + (SECTION_LEN - len) % 2; i++) data[dp++] = '=';
	data[dp++] = '\n';
	data[dp++] = '\0';
	
	_log_print(data);
	free(data);
}

void log_print(size_t prefix, const char *format, ...) {
	va_list args;
	int i;
	FILE *f;
	char* prefix_text = NULL;
	
	if(logfile) {
		f = fopen(logfile, "a");
		for(i = 0; i < indent * 4; i++) fputc(' ', f);
		for(i = 0; i < NUM_PREFIXES; i++) {
			if(prefixes[i].id == prefix) {
				prefix_text = prefixes[i].plain_text;
				break;
			}
		}
		if(!prefix_text) {
			for(i = 0; i < NUM_PREFIXES; i++) {
				if(prefixes[i].id == NONE) {
					prefix_text = prefixes[i].plain_text;
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
	if(log_stdout) {
		for(i = 0; i < indent * 4; i++) putchar(' ');
		for(i = 0; i < NUM_PREFIXES; i++) {
			if(prefixes[i].id == prefix) {
				prefix_text = prefixes[i].color_text;
				break;
			}
		}
		if(!prefix_text) {
			for(i = 0; i < NUM_PREFIXES; i++) {
				if(prefixes[i].id == NONE) {
					prefix_text = prefixes[i].color_text;
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

void log_end_section() {
	int i;
	size_t dp = 0;
	char* data = malloc(sizeof(char) * (SECTION_LEN + (indent * 4) + 3));
	
	for(i = 0; i < indent * 4; i++) data[dp++] = ' ';
	for(i = 0; i < SECTION_LEN; i++) data[dp++] = '-';
	data[dp++] = '\n';
	data[dp++] = '\n';
	data[dp++] = '\0';
	
	_log_print(data);
	
	free(data);
	indent--;
}
