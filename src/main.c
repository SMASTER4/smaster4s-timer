#include <stdlib.h>
#include <string.h>

#include "gui.h"
#ifdef __linux__
#include "cli.h"
#endif

int main(int argc, char** argv) {
  int status;
  #ifdef __linux__
  if(getenv("DESKTOP_SESSION") == NULL)
    status = cli_run(argc, argv);
  #endif
  status = gui_run(argc, argv);
  return status;
}
