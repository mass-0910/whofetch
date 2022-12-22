# whofetch

Name card exchange system with neofetch style display

![image](https://user-images.githubusercontent.com/53329734/208696004-a33b6bcd-f6f4-42a6-8a27-3c39bcd960fb.png)

Inspired by [Neofetch](https://github.com/dylanaraps/neofetch) and [yuyufetch](https://github.com/Hiroya-W/yuyufetch)

## Build

You can build the project with CMake.

Prease note that this project depends on [Talinx/jp2a](https://github.com/Talinx/jp2a).  
So you must download `libpng` and `libjpeg` before building.

```
$ cmake -S . -B build
$ cmake --build build
```

## How to use

Before you can use it, you must make your profile with whofetch:

```
$ whofetch --make
```

Show your profile:

```
$ whofetch
```

Remake your profile:

```
$ whofetch --remake
```

Show usage:

```
$ whofetch --help
```