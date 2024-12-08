@echo off
setlocal enabledelayedexpansion

set "folder_path=%~dp0"
echo Current shader folder path: !folder_path!

for %%f in ("%folder_path%\*.spv") do (
	del %%f
	set "filename=%%~nf"
	echo successfully deleted ------- !filename!.spv
)

echo.

set "compileCounter=0"

for %%a in ("%folder_path%\*.vert") do (
	set "vertname=%%~na_vert.spv"
	glslangValidator.exe -V -g "%%a" -o "!folder_path!\!vertname!"
	echo add !vertname!
	set /a compileCounter+=1
)
for %%b in ("%folder_path%\*.frag") do (
	set "fragname=%%~nb_frag.spv"
	glslangValidator.exe -V -g "%%b" -o "!folder_path!\!fragname!"
	echo add !fragname!
	set /a compileCounter+=1
)

echo.

if !compileCounter! equ 0 (
    echo Error: No shader files were compiled.
    exit /b 1  
) else (
    echo Successfully compiled !compileCounter! shader files.
)


endlocal
::pause