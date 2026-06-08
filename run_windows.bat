@echo off
setlocal

if "%~1"=="" (
  echo Usage: run_windows.bat ^<input_file^> ^<time_quantum^>
  exit /b 1
)

if "%~2"=="" (
  echo Usage: run_windows.bat ^<input_file^> ^<time_quantum^>
  exit /b 1
)

cmake -S . -B build
if errorlevel 1 exit /b 1

cmake --build build --config Release
if errorlevel 1 exit /b 1

if exist build\Release\scheduler.exe (
  build\Release\scheduler.exe %1 %2
) else if exist build\scheduler.exe (
  build\scheduler.exe %1 %2
) else (
  echo Could not find scheduler.exe after build.
  exit /b 1
)
