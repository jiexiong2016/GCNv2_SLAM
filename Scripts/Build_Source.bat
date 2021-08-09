echo Build type %build_type%
set root_dir=%cd%
mkdir "Build/%build_type%/Source"
pushd "Build/%build_type%/Source"
cmake.exe %root_dir%\Source -DCMAKE_BUILD_TYPE=%build_type% -G"NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE=%toolchain_file%
nmake
popd