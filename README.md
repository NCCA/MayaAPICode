# Maya API demos used in Lectures

The lecture notes to accompany this code are here https://nccastaff.bournemouth.ac.uk/jmacey/MayaAPI/ 



The dev kits are located here https://www.autodesk.com/developer-network/platform-technologies/maya and are now not part of the default maya install.


Once the DevKit has been extracted note the location and add the following environment variable to your system
    
``` 
MAYA_DEVKIT_LOCATION = [location where extracted]
```

This is used in the CMakeLists.txt to locate the devkit include files and libraries and CMake will be used to build all projects on Mac, Linux and Windows.

For each of the projects you will need to create a build directory and run cmake to generate the build files. For example on Linux or Mac

```
mkdir build
cd build
cmake -G Ninja ..
ninja
```

If you use the top level CMakeLists.txt then all the projects will be built and the resultant plugins will be placed in the plugins folder for ease.

You can set the MAYA_PLUG_IN_PATH in the Maya.env file to the plugins directory and then the plugins will be loaded when maya starts

```
MAYA_PLUG_IN_PATH = [location of plugins folder]
```


