# another_path_tracer
Yet another path tracer


Installation:

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=..
make
make install

Linking:

After installation you should export the install location in your .bashrc, like so:

export ANOTHER_PATH_TRACER_DIR=/path/to/your/install/directory

Link with $ANOTHER_PATH_TRACER_DIR/lib
Include $ANOTHER_PATH_TRACER_DIR/include
You can add $ANOTHER_PATH_TRACER_DIR/bin to your path.

To build tests, add -DBUILD_TESTS=ON to cmake call. Then:

make test
./unit_tests