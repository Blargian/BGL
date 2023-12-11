# Setup 

To get the examples working you will need CMake installed on your machine. Next we will use vcpkg to integrate boost into the project. 

from the root directory clone vcpkg:

`git clone https://github.com/Microsoft/vcpkg.git`

Once done, cd to the vcpkg folder and run:

`./bootstrap-vcpkg.sh`

Finally, run:

`./vcpkg integrate install`

`cd build` and run `cmake ..  -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake`

You can now `cd build` and open the VS solution file for whichever example you want and you can build and run that specific example in VS. 

# Contribution 

Contributions are more than welcome and will help others to learn BGL even quicker. If you'd like to contribute an example you can do so easily. Follow the guide below:

- fork the repository 
- clone your fork to your local machine
- make a new git branch on which you will make your changes. Eg) `git checkout -b 'my-new-example'`
- create the `.cpp` file for your example under `/src`
- add a new target for your example to CMakeLists.txt:

  ```CMake
  add_executable(
  my-new-example-name
  "src/my-new-example.cpp")
  ```
- if your example will read in data from a file, add the file to `/src` and add a new custom command to copy across the file to the binary directory post build:

```CMake
add_custom_command(
TARGET my-new-example-name POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy
${CMAKE_SOURCE_DIR}/src/new-example-data.txt
${CMAKE_CURRENT_BINARY_DIR}/new-example-data.txt
)
```
- Finally update the README by adding a new tablerow to the table:

```
<tr>
    <td>my-new-example.cpp</td>
    <td>A short and useful description describing what your example does or what information it covers</td>
  </tr>
```

- Lastly make a commit with a helpful message `git commit -m "added my new example that cover xyz"`
- Open a pullrequest on this repository. You should see it show up on the main page.
- Wait patiently for a codereview by another contributor and for them to merge your example. 
  

