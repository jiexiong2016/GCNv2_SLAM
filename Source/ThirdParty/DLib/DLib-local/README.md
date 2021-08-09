DLib
====

DLib is a collection of C++ classes to solve common tasks in C++ programs, as well as to offer additional functionality to use OpenCV data and to solve computer vision problems.

I have found myself using these functions very often, so I hope they may be useful to other researchers and developers too.

## Documentation

DLib is divided into 3 libraries for different purposes:

  * `DUtils`: utilities for several common C++ program requirements. It includes these classes:
    * `BinaryFile`: reads/writes binary files
    * `LineFile`: reads/writes text files by lines
    * `ConfigFile`: reads/writes text files with the format `key = value`
    * `FileFunctions`: mkdir, rmdir, dir... functionality
    * `Math`: math functions
    * `Random`: pseudo-random number functions
    * `STL`: functions for STL containers
    * `StringFunctions`: functions to manipulae strings
    * `Timestamp`: operates with timestamps
    * `TimeManager`: manages collections of timestamps
    * `Profiler`: measures execution time of portions of code
    * `DebugFunctions`: functions to measure memory consumption


  * `DUtilsCV`: utility functions for OpenCV data types. Classes included:
    * `Drawing`: functions to draw keypoints, data, axes...
    * `GUI`: shows images in windows and allows some user input
    * `IO`: I/O functions for storage and printing
    * `Mat`: functions to remove rows from matrices
    * `Transformations`: functions to deal with spatial transformations
    * `Types`: functions to convert between OpenCV data types


  * `DVision`: functions to solve computer vision tasks. Classes included:
    * `BRIEF`: implementation of the BRIEF descriptor
    * `FSolver`: implementation of the RANSAC + 8-point algorithm to compute fundamental matrices between images
    * `HSolver`: implementation of the RANSAC + DLT algorithm to compute homographies between images
    * `ImageFunctions`: functions to get patches from images
    * `BundleCamera`: reads/writes camera files created by the Bundle software
    * `PMVSCamera`, `PatchFile`, `PLYFile`: read/write data created by the PMVS software
    * `PixelPointFile`, `Matches`: read/write multi-purpose pixel and 3D data files
