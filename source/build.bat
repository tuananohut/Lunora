@echo off
mkdir ..\build
pushd ..\build
pwd
cl -FC -Zi ..\..\Lunora\source\engine.cpp ..\data\resource.res
popd
