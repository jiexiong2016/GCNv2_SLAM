rem Check if the file exists which tells us that the third party has been built

set third_party_build_tag=%cd%\Develop\%build_type%\3rd_party_built_tag.txt

if exist %third_party_build_tag% (
    echo .
    echo Third party libraries built already; skipping
    echo .
    exit /b 0
)

echo Build type %build_type%
set root_dir=%cd%
mkdir "Build/%build_type%/Source/ThirdParty"
pushd "Build/%build_type%/Source/ThirdParty"
cmake.exe %root_dir%\Source\ThirdParty -DCMAKE_BUILD_TYPE=%build_type% -G"NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE=%toolchain_file%
nmake
popd

rem Add the empty file which confirms it's been built

type nul > %third_party_build_tag%