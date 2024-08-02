#pragma once

#include <time.h>

// Parses hours:minutes:seconds into seconds
extern const time_t parse_delay(const char* delayAsString);

extern const unsigned short int get_lenght_as_string(const unsigned int value);
