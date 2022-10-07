/*
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
#include "logger.h"

int main() {
	char *dangerous = "dangerous >:)\n";
	
	/* Log all verbosities to stdout, enable color */
	log_init(LOG_ALL, NULL, true);
	
	/* Only log INFO and DEBUG to 'test.txt', disable color */
	log_init(INFO | DEBUG, "test.txt", false);
	
	/* Simple example */
	log_start_section("Test 1");
	log_print(INFO, "Hello, world!\n");
	log_print(INFO, "Hello, world 2!\n");
	log_print(NONE, "Can't have -shit- prefixes in Detroit!\n");
	
	/* Nested section */
	log_start_section("Test 2");
	log_print(INFO, "Second.\n");
	log_print(DEBUG, "FILLER TEXT\n");
	log_print(WARN, "stuffs\n");
	log_end_section();
	
	log_print(INFO, "More stuff\n");
	log_print(DEBUG, "But wait, there's more!\n");
	/* printf-style formatting */
	log_print(ERROR, "Dangerously poking into memory: %d\n", *((int*) ((void*) dangerous)));
	log_print(INFO, "Hello: %s\n", "hello");
	log_end_section();
	
	log_start_section("Test 3");
	log_print(INFO, "MOAR TEXXT\n");
	log_print(WARN, "I'm running out of examples...\n");
	log_end_section();
	
	log_start_section("Section A");
	log_start_section("Section B");
	log_start_section("Section C");
	log_start_section("Section D");
	log_print(ERROR, "Having text here looks better.\n");
	log_end_section();
	log_end_section();
	log_end_section();
	log_print(INFO, "Test!\n");
	log_end_section();
	log_print(WARN, "Testing...\n");
	/* Ending a section that didn't start yet... */
	log_end_section();
	log_print(INFO, "S0me testing...\n");
	
	return 0;
}
