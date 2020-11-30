# another_path_tracer

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
./unit_tests
```

To include in a CMake project:

```bash
find_package(APTracer 1.1.0 REQUIRED)
target_link_libraries(example APTracer::APTracer)
```
