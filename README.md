# another_path_tracer
Yet another path tracer


Installation:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
make install
```

Link with `$ANOTHER_PATH_TRACER_DIR/lib`  
Include `$ANOTHER_PATH_TRACER_DIR/include`  
You can add `$ANOTHER_PATH_TRACER_DIR/bin` to your path.  

To build tests, add `-DBUILD_TESTS=ON` to cmake call. Then:

```bash
make test
./unit_tests
```

To include in a CMake project:

```bash
find_package(APTracer 1.1.0 REQUIRED)
target_link_libraries(example APTracer::APTracer)
```