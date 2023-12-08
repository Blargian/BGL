# Setup 

To get the examples working you will need CMake installed on your machine. Next we will use vcpkg to integrate boost into the project. 

from the root directory clone vcpkg:

`git clone https://github.com/Microsoft/vcpkg.git`

Once done, cd to the vcpkg folder and run:

`./bootstrap-vcpkg.sh`

Finally, run:

`./vcpkg integrate install`

`cd build` and run `cmake ..  -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake`
