#!/bin/bash

# Check the build type
if [ "$#" -ne 1 ];
then
  build_type=Debug
else
  build_type=$1
fi

if [ "$build_type" != "Debug" ] && [ "$build_type" != "Release" ]
then
  echo Unknown build type ${build_type}
  exit 1
fi

./Scripts/Build_ThirdParty.sh ${build_type}
./Scripts/Build_Source.sh ${build_type}
