#pragma once

// Parses hours:minutes:seconds into seconds
extern void parse_delay(const char* delay_as_string, int delay_buffer[3]);

extern const unsigned short int get_lenght_as_string(const unsigned int value);
