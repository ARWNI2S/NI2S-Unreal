@echo off
:: Definir variables
set ProjectName=NI2SUnreal
set UprojectFile=%ProjectName%.uproject
set BuildCommand=UnrealBuildTool.exe %ProjectName%Server Win64 Development -Project="%cd%\%UprojectFile%" -WaitMutex -NoHotReloadFromIDE

:: Ejecutar comando de compilación
echo Compilando el servidor dedicado para %ProjectName%...
%BuildCommand%

:: Esperar a que el proceso finalice
pause
