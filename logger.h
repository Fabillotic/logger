/*
 * logger v1.0.0
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

//These are the verbosity levels.
//When 'max_verbosity' in 'log_init' is set to a value lower than the id, the output is ignored.
//This allows you to filter out the messages that are unnecessary to you.
//This also means that setting 'max_verbosity' to 'NUM_VERBOSITY_LEVELS' will log every single message, assuming you don't skip over ids.
#define NUM_VERBOSITY_LEVELS 5
#define NONE  0
#define INFO  1
#define WARN  2
#define ERROR 3
#define DEBUG 4

struct verbosity_level {
	size_t id;
	char* color_text;
	char* plain_text;
};

void log_init(int max_verbosity, char *fn, size_t mirror);
void _log_print(char *s);
void log_start_section(char* name);
void log_print(size_t prefix, const char *format, ...);
void log_end_section();
