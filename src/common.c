#include <string.h>
#include <math.h>

#include "common.h"

extern void parse_delay(const char* delay_as_string, int parsed_delay_buffer[3]) {
  if(strlen(delay_as_string) > 30)
    return;
  short unsigned int current_time_unit = 0;

  for(size_t i = 0; i < strlen(delay_as_string) && current_time_unit < 3; i++) {
    if(delay_as_string[i] == ':') {
      current_time_unit++;
      continue;
    }
    parsed_delay_buffer[current_time_unit] *= 10;
    parsed_delay_buffer[current_time_unit] += delay_as_string[i] - '0';
  }
}

extern const unsigned short int get_lenght_as_string(const unsigned int value) {
  return (value == 0 ? 1 : (int) (log10(value) + 1));
}
