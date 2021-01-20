# another_path_tracer

[![GitHub license](https://img.shields.io/github/license/guillaumetousignant/another_path_tracer.svg)](https://github.com/guillaumetousignant/another_path_tracer/blob/master/LICENSE) [![GitHub release](https://img.shields.io/github/release/guillaumetousignant/another_path_tracer.svg)](https://GitHub.com/guillaumetousignant/another_path_tracer/releases/) [![Documentation Status](https://readthedocs.org/projects/another-path-tracer/badge/?version=latest)](https://another-path-tracer.readthedocs.io/en/latest/?badge=latest) [![build-ubuntu Actions Status](https://github.com/guillaumetousignant/another_path_tracer/workflows/Ubuntu/badge.svg)](https://github.com/guillaumetousignant/another_path_tracer/actions) [![build-windows Actions Status](https://github.com/guillaumetousignant/another_path_tracer/workflows/Windows/badge.svg)](https://github.com/guillaumetousignant/another_path_tracer/actions) [![build-macos Actions Status](https://github.com/guillaumetousignant/another_path_tracer/workflows/macOS/badge.svg)](https://github.com/guillaumetousignant/another_path_tracer/actions)

Yet another path tracer

Dependencies:

- freeglut
- libpng
- libjpeg
- libtiff

If installing dependencies with vcpkg under windows, don't forget to add the CMake toolchain path to the cmake call:

```bash
-DCMAKE_TOOLCHAIN_FILE:FILEPATH=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

Installation:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
cmake --install .
```

Link with `$ANOTHER_PATH_TRACER_DIR/lib`  
Include `$ANOTHER_PATH_TRACER_DIR/include`  
You can add `$ANOTHER_PATH_TRACER_DIR/bin` to your path.  

To build tests, add `-DBUILD_TESTING=ON` to cmake call. Then:

```bash
make unit_tests
ctest
```

To generate documentation, add `-DBUILD_DOC=ON` to cmake call. Additional dependencies:

- Doxygen
- dot (graphviz)
- Sphinx
- Breathe
- Read the Docs Sphinx Theme

To include in a CMake project:

```bash
find_package(APTracer 1.2.0 REQUIRED)
target_link_libraries(example APTracer::APTracer)
```
