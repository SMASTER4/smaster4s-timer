#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef __unix__
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <io.h>

#include "file_flags.h"
#endif
#include "common.h"
#include "default_layout.h"

#ifdef __linux__
static const char* _config_relative_path = "/.config/smaster4s-timer/";
#endif
#ifdef _WIN32
static const char* _config_relative_path = "\\smaster4s-timer\\";
#endif

extern void parse_delay(const char* delay_as_string, int parsed_delay_buffer[3]) {
  for(size_t i = 0; i < 3; i++)
      parsed_delay_buffer[i] = 0;
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

extern char* get_config_path(const char* additional_path) {
  #ifdef __linux__
  const char* home_path = getenv("HOME");
  #endif
  #ifdef _WIN32
  const char* home_path = getenv("APPDATA");
  #endif
  if(home_path == NULL) {
    #ifdef __linux__
    fprintf(stderr, "You have no HOME enviorement variable. Please define HOME to be your home directory.\n");
    #endif
    #ifdef _WIN32
    fprintf(stderr, "You have no APPDATA enviorement variable. Please define APPDATA to be your app data folder. (It should mostly be C:\\Users\\!YOUR_USER_NAME!\\AppData\\Roaming\\\n)"); // btw folder != directory
    #endif
    return NULL;
  }
  const size_t config_full_path_length = strlen(home_path) + strlen(_config_relative_path) + strlen(additional_path) + 1;
  char* config_full_path = malloc(config_full_path_length);
  snprintf(config_full_path, config_full_path_length, "%s%s%s", home_path, _config_relative_path, additional_path);
  return config_full_path;
}

extern void create_layout_if_required() {
    char* layout_path = get_config_path("main.ui");
    char* layout_dir_path = get_config_path("");
    if(
        #ifdef __unix__
        access(layout_path, R_OK)
        #endif
        #ifdef _WIN32
        _access(layout_path, R_OK) // Yeah really its _access: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/access-waccess
        #endif
        !=
        0
      ) {
        struct stat st = {0};
        if(stat(layout_dir_path, &st) != 0)
          mkdir(layout_dir_path, 0777);

        FILE* file = fopen(layout_path, "w");
        if(file == NULL) {
          printf("A fialure while trying to write to %s accured. Maybe you don't have the required premissions.\n", layout_path);
          goto clean;
        }

        fprintf(file, default_layout);
        fclose(file);

        clean:
        free(layout_path);
        free(layout_dir_path);
      }
}
