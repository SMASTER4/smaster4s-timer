# About
This is a easy to use GTK based timer primarly for Linux. That's it, nothing revolutionary.
I just made it, because I miss a timer.
I know that there are about 1,000 other open source timers for Linux, but **I DON'T CARE**.
It's my little simplicit timer. No fancy cool looking UI just GTK.
I don't think there is much else to say.

# Important information
This project is still in developement so it is maybe not the best or stable timer and using this as a all day tool and replacement for more advanced timers like KTimer or Gnome Clocks is not recommended, but if you think you can improve it opening a pull request with a new cool feature is always good.

This project is using parts of the interface of the [GTK library](https://gtk.org/) and linking the [GTK library](https://gtk.org/) while compilation,
which is licensed under the [GNU Lesser General Public License v2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html) upwards.

# Building from source

### Building requirements
**You need**:
+ **GTK4**
+ **S-Master4s-INIs**
+ **CMake** 3.12 or higher
+ A **C11 compiler**(We recommend GCC, because it ships with many Distros preinstalled.)
+ A building system compatible with your compiler and cmake(When using GCC make is the best option, because it's part of the GCC suit)
+ **git**(optional, but still recommended)

**If you know that you have all of those things already installed, you can skip to *Acctually Building the project*.**

#### GTK4
I don't know anyway of making sure someone has GTK installed,
but most Distros ship with it on default so there is mostly no need to install it,
but if you encounter issues just run:

**on Debian**:
```bash
sudo apt install libgkt-4-dev
```
and **on Arch Linux**:
```bash
sudo pacman -S gtk4
```

#### S-Master4s-INIs
S-Master4s-INIs is a INI parsing library and its used to parse some config files for the project.
It is currently required, but there is work being done to make it optional.
The only thing you need to do is build and install it with this [guide](https://github.com/SMASTER4/smaster4s-inis/blob/main/README.md).

#### CMake
To verify that CMake is installed on the right version try running:
```bash
cmake --version
```
If it says 3.12 upwards, you can proceed. If it says anything other than that, install or update CMake, please.
You do this this **on Debian** based Distros(Ubuntu included) like this:
```bash
sudo apt install cmake
```
**On Arch Linux** based Distros you would do this like this:
```bash
sudo pacman -S cmake
```
If you have neither of those, I recommend you just try to install a package called cmake with your package manager.
You can also try the official [download site](https://cmake.org/download/), if that didn't work or you have no package manager.

#### C11 Compiler
You will also need a C11 compiler. I recommend GCC, but really anything supporting C11 like clang or whatever else should also work.
When assuming you chose GCC, the next step would be verifying if it's installed. For that run:
```bash
gcc --version
```
If it prints out the version, you can skip installing GCC.
If it prints out anything else, then installing GCC is required first.

Do this **on Debian** and it's derivates by running:
```bash
sudo apt install gcc
```
**On Arch Linux** and it's derivates it is:
```bash
sudo pacman -S gcc
```
Like I said before if neither of those is your operating system trying to install a package called gcc with your package manager should on most Distros install gcc.
If you Windows using MinGW-w64 should also work.

#### Git
This point is optional, but if you haven't allready installed the sources.
The recommended way of doing this is by using Git.
If you don't, if you installed Git jus run:
```bash
git --version
```
If it outputs the version, you can proceed. If it doesn't then
**On Debian** derivates run:
```bash
sudo apt install git
```
**On Arch Linux** deratives run:
```bash
sudo pacman -S git
```

### Building the binary
1. Now we can start buidling the project. First clone the project, if you haven't allready with:
```bash
git clone "https://github.com/SMASTER4/smaster4s-timer/"
```
2. To work more effectively changing your working directory makes sense(This is entierly optional.):
```bash
cd smaster4s-timer
```
3. Usally you'd want to create a new directory to build your project in.(This is also entierly optional.) Do this by running:
```bash
mkdir out # You can change the name to be anything you want
```
4. Now we can just jump inside the directory we created:
```bash
cd out # Keep in mind that you need to change the name if you've used a different name
```
5. It's time to finally start building the CMake stuff by running:
```bash
cmake ..
```
Now some new files and directory's should appear inside your out or whatever name you have choosen directory.
If it doesn't, try analysing the issue based on the output of the last command.
6. Now we can finally build the real binary. We do this differently depending on the compiler and building tool you chose.
Look at the documentation of your toolchain if you are unsure what to do, but if we are using GCC and Make you can just do:
```bash
make
```
7. You are done. The final binary should now appear inside your choosen output directory.
