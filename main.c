#include <stdlib.h>
#include "logger.h"

int main() {
	char *dangerous = "dangerous >:)\n";
	log_init("test.txt", 1);
	
	log_start_section("Test 1");
	log_print(INFO, "Hello, world!\n");
	log_print(INFO, "Hello, world 2!\n");
	log_print(NONE, "Can't have -shit- prefixes in Detroit!\n");
	
	log_start_section("Test 2");
	log_print(INFO, "Second.\n");
	log_print(DEBUG, "FILLER TEXT\n");
	log_print(WARN, "stuffs\n");
	log_end_section();
	
	log_print(INFO, "More stuff\n");
	log_print(DEBUG, "But wait, there's more!\n");
	log_print(ERROR, "Dangerously poking into memory: %d\n", *((int*) ((void*) dangerous)));
	log_print(INFO, "Hello: %s\n", "hello");
	log_end_section();
	
	log_start_section("Test 3");
	log_print(INFO, "MOAR TEXXT\n");
	log_print(WARN, "I'm running out of examples...\n");
	log_end_section();
	
	return 0;
}
