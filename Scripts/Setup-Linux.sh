#!/bin/bash

pushd ..
Vendor\Binaries\Premake\Windows\premake5.exe --file=Build.lua vs2022
Vendor/Binaries/Premake/Linux/premake5 --cc=clang --file=Build.lua gmake2
popd
