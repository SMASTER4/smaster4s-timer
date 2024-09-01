#pragma once
#include <smaster4s-inis.h>

// Parses hours:minutes:seconds into seconds
// beOF
extern void parse_delay(const char* delay_as_string, int delay_buffer[3]);

// 0FC
extern const unsigned short int get_lenght_as_string(const unsigned int value);

// Rc
// rnOF
extern char* get_config_path(const char* additional_config_path);

extern void create_layout_if_required();

// 0FC
extern char* format_delay(const int delay[3]);

extern char* get_tranlation_file_text();

extern void get_language(char language_buffer[INI_LINE_DATA_SIZE]);

extern void get_translation(char translation_buffer[INI_LINE_DATA_SIZE], const char* language, const char* translation_name, const char* fallback);
