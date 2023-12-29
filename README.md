# Sphere-in-OpenGL
---
## About

**Small program generating a sphere in OpenGL, written in C.**

Long story short. While I was learning Computer Graphics from book ["_OpenGL i GLSL (nie taki krótki kurs)_"](https://helion.pl/ksiazki/opengl-i-glsl-nie-taki-krotki-kurs-czesc-i-przemyslaw-kiciak,e_1ha3.htm#format/e) by Przemysław Kiciak I thought of making a sphere. And that's it. What did you think, that I will write some hearbreaking story? It was just an idea and I made it real.

---

## Dependencies

#### To build the project you'll need:

* git
* CMake
* C/C++ compiler (gcc, clang, etc.)

##### Additionally on Linux (tested on Ubuntu):

  * libgl1-mesa-dev
  * mesa-utils
  * libx11-dev
  * libxrandr-dev
  * libxinerama-dev
  * libxcursor-dev
  * libxi-dev

---

## Building the project

* #### Clone the repository

```shell
git clone --recursive https://github.com/marcel-zdziechowicz/Sphere-in-OpenGL
```

* #### Build the project

```shell
cd Sphere-in-OpenGL
mkdir build
cd build
cmake -S .. -B .
make # or msbuild or something else
```

* #### Run the program

    * ##### On Linux

    ```shell
    ./Sphere-in-OpenGL
    ```

    * ##### On Windows

    **NOTE:** You might want to **move** the executable to `build` directory or **make a symlink**.

    ```shell
    Path/To/Your/Executable/Sphere-in-OpenGL.exe
    ```

---

## Controls


This program have controls to navigate which effect on your sphere to turn on or off.

* `W key` to display only points (vertices)
* `K key` to display only mesh
* `S key` to display whole sphere
* `L key` to turn on/off light
* `Space` to turn on'off animation
* `ESCAPE` to close the program