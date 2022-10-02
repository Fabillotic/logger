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

#define NUM_PREFIXES 5
#define NONE  0
#define INFO  1
#define WARN  2
#define ERROR 3
#define DEBUG 4

struct prefix {
	size_t id;
	char* color_text;
	char* plain_text;
};

void log_init(char *fn, size_t mirror);
void _log_print(char *s);
void log_start_section(char* name);
void log_print(size_t prefix, const char *format, ...);
void log_end_section();
