# linux-idletimer
A simple IDLE timer for any linux system whether it has a DE or it's headless

----

Because I have seen there is no easy way to detect system being IDLE without using Xorg or Wayland which actually mostly will not be a solution, for example when trying to see if system is Idle using from another user on Xorg it will fail, I came with this simple idea to read `/dev/input/event*` for any events the user perform, this will include keyboard or mouse movements, and it's easy to copy-paste this implementation to any codebase.

----

## Requirements
You only need to install `libevdev` library
```bash
# ubuntu
apt install libevdev-dev
# arch
pacman -S libevdev
# fedora
dnf install libevdev-devel
```

## Build and Run
to build the project simple run the command
```bash
make
```
now before running the file, you need whether to use sudo or give the file permissions to read & execute on /dev/input/event*


```bash
sudo ./idletimer
# or 
sudo setfacl -R -m u:`whoami`:rx /dev/input
```

----

to clean the compilition run the command
```bash
make clean
```
