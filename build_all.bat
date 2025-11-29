@echo off

ECHO "Building everything..."

PUSHD "c:\dev\Lunora\Lunora\Engine"
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% %% exit)

PUSHD "c:\dev\Lunora\Lunora\testbed"
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% %% exit)

PUSHD "c:\dev\Lunora"

ECHO "All assemblies built successfully."
