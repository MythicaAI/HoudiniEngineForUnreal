@echo off

if "%~1"=="" (
    echo Usage: %0 [Unreal Engine Installation Directory]
    exit /b 1
)

for /f "tokens=2 delims==" %%I in ('wmic os get localdatetime /value') do set dt=%%I
set Timestamp=0
set BuildDir=%CD%\Builds\Build_%Timestamp%

call "%~1\Engine\Build\BatchFiles\RunUAT.bat" BuildPlugin -Plugin="%CD%\HoudiniEngine.uplugin" -Package="%BuildDir%\HoudiniEngine" -nocompile -nocompileuat

del "%BuildDir%\HoudiniEngine\Binaries\Win64\*.pdb"

powershell Compress-Archive -Path "%BuildDir%\HoudiniEngine" -DestinationPath "%BuildDir%\HoudiniEngine.zip"