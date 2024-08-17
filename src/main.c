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
      free(layout_path);
      free(layout_dir_path);
      return -1;
    }

    fprintf(file, default_layout);
    fclose(file);

    free(layout_path);
    free(layout_dir_path);
  }

  status = gui_run(argc, argv);
  return status;
}
