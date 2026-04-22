#!/bin/bash

pushd ..
Vendor/Binaries/Premake/premake5 --cc=clang --file=Build.lua gmake2
popd
