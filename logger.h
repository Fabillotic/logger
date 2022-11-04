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

#include <stdlib.h>
#include <stdbool.h>

#define SECTION_LEN 50
#define NONE_TEXT_COLOR  "      "
#define INFO_TEXT_COLOR  "\033[97;1mINFO  \033[0m"
#define WARN_TEXT_COLOR  "\033[35;1mWARN  \033[0m"
#define ERROR_TEXT_COLOR "\033[91;1mERROR \033[0m"
#define DEBUG_TEXT_COLOR "\033[92;1mDEBUG \033[0m"
#define NONE_TEXT_PLAIN  "      "
#define INFO_TEXT_PLAIN  "INFO  "
#define WARN_TEXT_PLAIN  "WARN  "
#define ERROR_TEXT_PLAIN "ERROR "
#define DEBUG_TEXT_PLAIN "DEBUG "

/* These are the verbosity levels. They are used as bitmasks! */
#define NUM_VERBOSITY_LEVELS 5
#define NONE  (1 << 0)
#define INFO  (1 << 1)
#define WARN  (1 << 2)
#define ERROR (1 << 3)
#define DEBUG (1 << 4)

#define LOG_ALL (~0)

struct verbosity_level {
	size_t id;
	char* color_text;
	char* plain_text;
};

typedef struct logger_profile LOGGER;
struct logger_profile {
	struct logger_profile *next;
	char *logfile;
	size_t verbosity;
	bool colored_output;
};

/* Initialize a logging profile. If fn is NULL, log to stdout. */
LOGGER *log_init(size_t max_verbosity, char *fn, bool color);
void log_del(LOGGER *profile);
void log_start_section(char* name);
void log_print(size_t prefix, const char *format, ...);
void log_end_section();
