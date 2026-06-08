@echo off
setlocal

set INPUT=%~1
set QUANTUM=%~2
if "%INPUT%"=="" set INPUT=examples\basic.txt
if "%QUANTUM%"=="" set QUANTUM=4

echo =^> Building CPU Scheduling Simulator...

cmake -S . -B build
if errorlevel 1 exit /b 1

cmake --build build --config Release
if errorlevel 1 exit /b 1

echo =^> Running scheduler...

if exist build\Release\scheduler.exe (
  build\Release\scheduler.exe %INPUT% %QUANTUM%
) else if exist build\scheduler.exe (
  build\scheduler.exe %INPUT% %QUANTUM%
) else (
  echo Could not find scheduler.exe after build.
  exit /b 1
)
