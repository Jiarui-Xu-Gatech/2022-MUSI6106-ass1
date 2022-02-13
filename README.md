![GitHub top language](https://img.shields.io/github/languages/top/alexanderlerch/2022-MUSI6106)
![GitHub issues](https://img.shields.io/github/issues-raw/alexanderlerch/2022-MUSI6106)
![GitHub last commit](https://img.shields.io/github/last-commit/alexanderlerch/2022-MUSI6106)
![GitHub](https://img.shields.io/github/license/alexanderlerch/2022-MUSI6106)

# MUSI6106 2022: Audio Software Engineering
Template project for assignments and exercises for the class MUSI6106

## Project Structure
```console
|_ 3rdparty: (3rd party dependencies)
  |_ sndlib: sndfile library (3rdparty with ugly code and lots of warnings)
|_ cmake.modules: (cmake scripts)
|_ inc: global headers
|_ src: source code
  |_ AudioFileIO: library wrapping sndfile (3rdparty)
  |_ CombFilter: combfilter library (assignment 1)
  |_ inc: internal headers
  |_ MUSI6106Exec: code for executable binary
```

## Building
The project files are generated through [CMake](https://www.cmake.org). Using the latest CMake GUI, 
* point the source code directory to the top-level project directly, then 
* set the build directory to some directory you like (suggestion: sourcedir/bld), 
* hit 'Configure' button until nothing is red, then
* 'Generate' the project and open it with your IDE.

On the command line, try from the sourcedir

```console
cmake -B ./bld/ -DCMAKE_BUILD_TYPE=DEBUG
cmake --build ./bld/ --config Debug
```

