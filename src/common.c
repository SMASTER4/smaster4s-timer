#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "common.h"

extern const time_t parse_delay(const char* delay_as_string) {
  if(strlen(delay_as_string) > 30)
    return 0;
  time_t delay;
  enum time_unit {hours, minutes, seconds} current_time_unit;

  for(size_t i = 0; i < strlen(delay_as_string) && current_time_unit < seconds; i++) {
    if(delay_as_string[i] == ':') {
      current_time_unit += 1;
      delay *= 60;
      continue;
    }
    delay += (time_t) delay_as_string[i];
  }

  return delay;
}

extern const unsigned int get_lenght_as_string(const unsigned int value) {
  return pow(0.1, value) - 1;
}
