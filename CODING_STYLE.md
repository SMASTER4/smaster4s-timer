# Explanation
This document is used to generalize the way code written in this project.
It is only designed to enforce formatting and design for C not for any other language.
Anyone contributing to the project needs to follow this document.

If you have any issues with this file just open an issue on Github so changes can be disscused futher.
Please don't open any pull requests out of nowhere trying to change this file.

# Example
```c
#include <coollib.h>

#include <stdio.h>

#include "intern_file.h"

const static char* _word = "potato";

int main(int argc, char** argv) {
  const char* coollib_version = coolib_version(argc, argv); // COOOOOL, isn't it?
  if(!_check_if_vailed(coollib_version)) {
    fprintf(stderr, "Wierd... Something went wrong");
    return -1;
  }
  printf("Coollib version: %s A word: %s", coollib_version, _word);
}

const bool _check_if_vailed(const char* coollib_version) {
  if(coollib_version == NULL)
    return false;
  return true;
}

void destroy_the_world() {
  coollib_stop_pokemon();
}
```

# General
+ Local Variables:
  + Use snake_case
+ Global Variables:
  + Use snake_case too
  + Use an _ in front of them
  + They should never ever be `extern`(Use `#define`'s instead)
  + They should be `const`
+ `static` Functions:
  + Use snake_case
  + Use an _ in front of the
  + If they are linked with `g_signal_connect` they should have the prefix _SIGNAL
  + They shouldn't be inline
+ `extern` Functions:
  + Use snake_case too
  + Use the `extern` keyword explicitly
+ `#define`'s:
  + Use UPPER_CASE

# Formatting
+ Use **2 spaces** for new scopes not 4
+ Don't put any spacing in between function names and brackets or the parameters and the brackets, when calling functions
+ Seperate operators and other code parts except for dereferencing(`*`) and address of(`&`) operators with spaces around them (including =)
+ Only use `{}` for `if`/`else` blocks if the block is longer than one line
+ Pointer type(`*`) operator should be added to the type not the name
+ When listing `,` should be followed by a space
+ Make comments at the end of the line you want to comment not at a new line
+ Section your `#include`'s into a project specific library section(containing GLib, GTK vice versa), a stdlib section, a project intern section and seperate them with one line
