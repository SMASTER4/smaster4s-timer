#include <stdlib.h>

#include "gui.h"
#ifdef __linux__
#include "cli.h"
#endif
#include "common.h"

int main(int argc, char** argv) {
  int status;
  create_layout_if_required();
  #ifdef __linux__
  if(getenv("DESKTOP_SESSION") == NULL)
    status = cli_run(argc, argv);
  #endif

  status = gui_run(argc, argv);
  return status;
}
