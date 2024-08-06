#include <stdlib.h>
#include <stdio.h>
#ifdef __unix__
#include <unistd.h>
#endif

#ifdef _WIN32
#include <io.h>

#include "file_flags.h"
#endif
#include "gui.h"
#ifdef __linux__
#include "cli.h"
#endif
#include "common.h"

#include "../res/default_layout.h"

int main(int argc, char** argv) {
  int status;
  #ifdef __linux__
  if(getenv("DESKTOP_SESSION") == NULL)
    status = cli_run(argc, argv);
  #endif
  char* config_path = get_config_path("main.ui");

  if(
    #ifdef __unix__
    access(config_path, R_OK)
    #endif
    #ifdef _WIN32
    _access(config_path, R_OK) // Yeah really its _access: https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/access-waccess
    #endif
    !=
    0
  ) {
    FILE* file = fopen(config_path, "w");
    fprintf(file, default_layout);
    fclose(file);
  }

  status = gui_run(argc, argv);
  return status;
}
