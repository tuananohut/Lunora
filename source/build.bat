@echo off
call ..\misc\shell.bat
mkdir ..\build
pushd ..\build
pwd
cl -Zi ..\..\Lunora\source\engine.cpp ..\data\resource.res
popd